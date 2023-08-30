#include "bmp.h"

#include <cstring>
#include <fstream>

const uint8_t kBitesPerPixel = 4;
const uint8_t kSizeHeader = 14;
const uint8_t kSizeInfoHeader = 40;
const uint8_t kColorsCount = 5;
const uint8_t kBytesPerColorInTable = 4;
const uint8_t kBaseBMPSizeWithColorTable = kSizeHeader + kSizeInfoHeader + kBytesPerColorInTable * kColorsCount;

void createBMP(const List& L, uint16_t length, uint16_t width, const std::string file_name) {

    std::ofstream file;
    file.open(file_name, std::ios::binary);

    uint16_t file_width = width;
    if (file_width % 8 != 0) {
        file_width = file_width + 8 - file_width % 8;
    }

    size_t fileSize = kBaseBMPSizeWithColorTable + ((file_width) * length) / 2;
    static uint8_t fileHeader[] = {
            0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0
    };
    fileHeader[0] = static_cast<uint8_t>('B');
    fileHeader[1] = static_cast<uint8_t>('M');
    fileHeader[2] = static_cast<uint8_t>(fileSize);
    fileHeader[3] = static_cast<uint8_t>(fileSize >> 8);
    fileHeader[4] = static_cast<uint8_t>(fileSize >> 16);
    fileHeader[5] = static_cast<uint8_t>(fileSize >> 24);
    fileHeader[10] = static_cast<uint8_t>(kBaseBMPSizeWithColorTable);

    file.write(reinterpret_cast<char*>(fileHeader), kSizeHeader);

    static uint8_t infoHeader[] = {
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0
    };
    infoHeader[0] = static_cast<uint8_t>(kSizeInfoHeader);
    infoHeader[4] = static_cast<uint8_t>(width);
    infoHeader[5] = static_cast<uint8_t>(width>> 8);
    infoHeader[6] = static_cast<uint8_t>(width >> 16);
    infoHeader[7] = static_cast<uint8_t>(width >> 24);
    infoHeader[8] = static_cast<uint8_t>(length);
    infoHeader[9] = static_cast<uint8_t>(length >> 8);
    infoHeader[10] = static_cast<uint8_t>(length >> 16);
    infoHeader[11] = static_cast<uint8_t>(length >> 24);
    infoHeader[12] = 1;
    infoHeader[14] = kBitesPerPixel;
    infoHeader[32] = kColorsCount;
    infoHeader[36] = kColorsCount;

    file.write(reinterpret_cast<char*>(infoHeader), kSizeInfoHeader);

    uint8_t color[kBytesPerColorInTable];
    color[0] = 255;
    color[1] = 255;
    color[2] = 255;
    color[3] = 0;
    file.write(reinterpret_cast<char*>(color), kBytesPerColorInTable);

    color[0] = 0;
    color[1] = 255;
    color[2] = 0;
    file.write(reinterpret_cast<char*>(color), kBytesPerColorInTable);

    color[0] = 255;
    color[1] = 0;
    color[2] = 70;
    file.write(reinterpret_cast<char*>(color), kBytesPerColorInTable);

    color[0] = 0;
    color[1] = 255;
    color[2] = 255;
    file.write(reinterpret_cast<char*>(color), kBytesPerColorInTable);

    color[0] = 0;
    color[1] = 0;
    color[2] = 0;
    file.write(reinterpret_cast<char*>(color), kBytesPerColorInTable);

    Node* first_element_of_line = L.left_up_corner_;
    Node* x = first_element_of_line;
    uint64_t first_in_pair, second_in_pair;
    uint8_t printed;
    for (uint16_t i = 0; i < length; i++) {
        for (uint16_t j = 0; j < file_width; j += 2) {
            if (j >= width) {
                first_in_pair = 0;
                second_in_pair = 0;
            } else if (j + 1 >= width) {
                first_in_pair = x->value;
                second_in_pair = 0;
            } else {
                first_in_pair = x->value;
                second_in_pair = x->right->value;
            }

            printed = (first_in_pair << 4);
            if (second_in_pair > 3) {
                printed += 4;
            } else {
                printed += second_in_pair;
            }
            file << printed;

            if (x->right != nullptr) {
                x = x->right;
            }
            if (x->right != nullptr) {
                x = x->right;
            }
        }
        first_element_of_line = first_element_of_line->down;
        x = first_element_of_line;
    }

    file.close();
}
