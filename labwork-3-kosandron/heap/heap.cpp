#include "heap.h"
#include "../bmp/bmp.h"

#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>

std::string generateFileName(const std::string path, uint64_t number) {
    return path + std::to_string(number) + ".bmp";
}

void deleteField(List L, uint16_t length, uint16_t width) {
    for (int i = 0; i < length - 1; i++) {
        for (int j = 0; j < width - 2; j++) {
            L.left_up_corner_->right = L.left_up_corner_->right->right;
            delete L.left_up_corner_->right->left;
        }
        delete L.left_up_corner_->right;
        L.left_up_corner_ = L.left_up_corner_->down;
        delete L.left_up_corner_->up;
    }
    delete L.left_up_corner_->right;
    delete L.left_up_corner_;
}

void newConnectionUpDown(Node* up_element, Node* down_element) {
    up_element->down = down_element;
    down_element->up = up_element;
}

void newConnectionLeftRight(Node* left_element, Node* right_element) {
    left_element->right = right_element;
    right_element->left = left_element;
}

void List::create(const Properties& options) {
    std::ifstream f(options.inputFile);
    std::vector<std::vector<uint64_t>> a(options.length, std::vector<uint64_t>(options.width, 0));

    while (!f.eof()) {
        uint16_t x, y;
        uint64_t value;
        f >> x >> y >> value;
        a[y][x] = value;
    }

    Node* begin = new Node();
    begin->value = a[0][0];
    left_up_corner_ = begin;
    Node* last = begin;
    for (int i = 1; i < options.width; i++) { //  first row
        Node *new_element = new Node;
        newConnectionLeftRight(last, new_element);
        new_element->value = a[0][i];
        last = new_element;
    }
    right_up_corner_ = last;

    Node* previous_element = begin;
    for (int i = 1; i < options.length; i++) {
        Node* begin_of_line = new Node();
        begin_of_line->value = a[i][0];
        newConnectionUpDown(previous_element, begin_of_line);
        last = begin_of_line;

        for (int j = 1; j < options.width; j++) {
            previous_element = previous_element->right;
            Node *new_element = new Node;
            newConnectionLeftRight(last, new_element);
            newConnectionUpDown(previous_element, new_element);
            new_element->value = a[i][j];

            last = new_element;
        }
        previous_element = begin_of_line;
    }

    left_down_corner_ = previous_element;
    right_down_corner_ = last;

}

void List::newRowUp(uint16_t width) {
    Node* begin_of_new_line = new Node();
    Node* element_downer = left_up_corner_;
    newConnectionUpDown(begin_of_new_line, element_downer);
    Node* last = begin_of_new_line;

    for (int i = 1; i < width; i++) {
        element_downer = element_downer->right;
        Node* new_element = new Node();
        newConnectionLeftRight(last, new_element);
        newConnectionUpDown(new_element, element_downer);

        last = new_element;
    }

    left_up_corner_ = begin_of_new_line;
    right_up_corner_ = last;
}

void List::newRowDown(uint16_t width) {
    Node* begin_of_new_line = new Node();
    Node* elementUpper = left_down_corner_;
    newConnectionUpDown(elementUpper, begin_of_new_line);
    Node* last = begin_of_new_line;

    for (int i = 1; i < width; i++) {
        elementUpper = elementUpper->right;
        Node* new_element = new Node();
        newConnectionLeftRight(last, new_element);
        newConnectionUpDown(elementUpper, new_element);

        last = new_element;
    }

    left_down_corner_ = begin_of_new_line;
    right_down_corner_ = last;
}

void List::newRowLeft(uint16_t length) {
    Node* begin_of_new_column = new Node();
    Node* element_righter = left_up_corner_;
    newConnectionLeftRight(begin_of_new_column, element_righter);
    Node* last = begin_of_new_column;

    for (int i = 1; i < length; i++) {
        element_righter = element_righter->down;
        Node* new_element = new Node();
        newConnectionUpDown(last, new_element);
        newConnectionUpDown(new_element, element_righter);

        last = new_element;
    }

    left_up_corner_ = begin_of_new_column;
    left_down_corner_ = last;
}

void List::newRowRight(uint16_t length) {
    Node* begin_of_new_column = new Node();
    Node* element_lefter = right_up_corner_;
    newConnectionLeftRight(element_lefter, begin_of_new_column);
    Node* last = begin_of_new_column;

    for (int i = 1; i < length; i++) {
        element_lefter = element_lefter->down;
        Node* new_element = new Node();
        newConnectionUpDown(last, new_element);
        newConnectionLeftRight(element_lefter, new_element);

        last = new_element;
    }

    right_up_corner_ = begin_of_new_column;
    right_down_corner_ = last;
}

void heapBreak(List& L, Properties& options) {
    bool end;
    uint64_t iteration = 0;
    do {
        end = true;
        Node* first_element_of_line = L.left_up_corner_;
        uint16_t new_length = options.length;
        uint16_t new_width = options.width;
        Node* element;
        for (int i = 0; i  < options.length; i++) {
            element = first_element_of_line;
            for (int j = 0; j < options.width; j++) {
                if (element->value > 3) {
                    if (element->up == nullptr) {
                        L.newRowUp(new_width);
                        new_length++;
                    }
                    if (element->left == nullptr) {
                        L.newRowLeft(new_length);
                        new_width++;
                    }
                    if (element->down == nullptr) {
                        L.newRowDown(new_width);
                        new_length++;
                    }
                    if (element->right == nullptr) {
                        L.newRowRight(new_length);
                        new_width++;
                    }

                    element->up->add++;
                    element->left->add++;
                    element->down->add++;
                    element->right->add++;
                    element->value -= 4;

                    end = false;
                }
                element = element->right;
            }
            first_element_of_line = first_element_of_line->down;
        }
        options.length = new_length;
        options.width = new_width;

        first_element_of_line = L.left_up_corner_;
        for (int i = 0; i < options.length; i++) {
            element = first_element_of_line;
            for (int j = 0; j < options.width; j++) {
                element->value = element->value + element->add; //  updating
                element->add = 0;
                element = element->right;
            }
            first_element_of_line = first_element_of_line->down;
        }

        iteration++;
        if (options.freq != 0 && iteration % options.freq == 0) { //  output-block
            std::string file_name = generateFileName(options.outputFile, iteration / options.freq);
            createBMP(L, options.length, options.width, file_name);
        }
    } while (!end && iteration < options.maxIter);

    createBMP(L, options.length, options.width, "finalImage.bmp"); //  output of final image

    deleteField(L, options.length, options.width);
}
