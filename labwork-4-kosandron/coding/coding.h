#pragma once

#include <cstdio>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

void coder(const uint8_t* line, uint64_t line_size, uint8_t control_bites_count, uint8_t* code);
uint64_t errorIndex(uint8_t control_bites_count, const std::vector<uint8_t>& code, uint64_t bite_position);
std::string decoder(const std::string& message, uint8_t control_bites_count);
void converter(const std::string& message, uint64_t pack_size, uint8_t control_bytes_count, std::ofstream& out);
