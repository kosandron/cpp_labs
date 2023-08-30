#include "../parser/parser.h"

#pragma once

#include <cstdint>
#include <string>

struct Node {
    uint64_t value = 0;
    uint64_t add = 0;
    Node* left = nullptr;
    Node* right = nullptr;
    Node* up = nullptr;
    Node* down = nullptr;
};

class List {
public:
    Node* left_up_corner_;
    Node* left_down_corner_;
    Node* right_up_corner_;
    Node* right_down_corner_;
    void create(const Properties& options);
    void newRowUp(uint16_t width);
    void newRowDown(uint16_t width);
    void newRowLeft(uint16_t length);
    void newRowRight(uint16_t length);
};

std::string generateFileName(const std::string path, uint64_t number);
void deleteField(List L, uint16_t length, uint16_t width);
void newConnectionUpDown(Node* up_element, Node* down_element);
void newConnectionLeftRight(Node* left_element, Node* right_element);
void heapBreak(List& L, Properties& options);
