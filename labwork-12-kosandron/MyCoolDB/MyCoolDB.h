#pragma once
#include "table.h"
#include "DBExeptions.h"
#include "splitFunctions.h"

#include <set>
#include <algorithm>
#include <fstream>
#include <filesystem>

class MyCoolDB {
    std::map<std::string, table> tables;
    friend table;
public:
    uint64_t size();
    table request(const std::string& request_line);
    void load(std::filesystem::path path);
    void save(std::filesystem::path path);
    void addTable(const std::string& table_name, const table& new_table);
    table parse(const std::vector<std::string>& words);
    void drop(const std::string& table_name);
    void insert(const std::string& table_name, const std::vector<std::pair<std::string, std::string>>& values);
    void deleteLines(const std::string& table_name, const std::vector<uint64_t> indexes);
    std::set<uint64_t> conclusion(const std::string& table_name, const std::string& lhs, const std::string& rhs, const std::string& operation);
    std::set<uint64_t> where(const std::string& table_name, const std::vector<std::string> words, uint32_t index);
    };
