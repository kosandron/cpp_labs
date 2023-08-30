#include <iostream>
#include <cstring>
#include <sstream>
#include <vector>
#include <fstream>

std::vector<std::string> SplitString(const std::string& str, char delimiter = ' ');

std::vector<std::string> SplitEqual(const std::vector<std::string>& strings);

std::vector<std::string> SplitScopes(const std::vector<std::string>& strings);

void join_parse(const std::vector<std::string>& words, size_t index, std::string& second_table, std::string& column1, std::string& column2);
