#pragma once

#include "../parser/parser.h"
#include "../coding/coding.h"

#include <cstdio>
#include <cstdint>
#include <cstdlib>
//#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

uint64_t gcd (uint64_t a, uint64_t b);
uint64_t lcm (uint64_t a, uint64_t b);

void readHeader(uint64_t& file_size, std::string& file_name, uint64_t& name_size,  uint8_t& control_bytes_count, std::ifstream& arch);
void createArchive(const std::string& archive_name, const std::vector<std::string>& add_files, uint8_t control_bytes);
void extractArchive(const std::string& archive_name, const std::vector<std::string>& extract_files);
void addFile(const std::string& archive_name, const std::string& file_name, uint8_t control_bytes_count);
void deleteFile(const std::string& archive_name, const std::string& file_name);
void mergeArchives(const std::string& first_archive_name, const std::string& second_archive_name, const std::string& result_archive_name);
void printList(const std::string& archive_name);
