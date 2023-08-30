#pragma once

#include <iostream>
#include <cstring>
#include <sstream>
#include <vector>
#include <fstream>

class Column {
public:
    enum DataType {
        Int, Varchar, Bool, Double, Float
    };
private:
    std::string name;
    bool is_primary_key = false;
    bool is_foreign_key = false;
    bool not_null = false;
    uint64_t count = 0;
    std::vector<std::string> ceils;
    DataType type_;
    Column* reference = nullptr;
    //friend table;
public:
    Column(const std::string& column_name, DataType type,  bool primary_key, bool foreight_key)
            : name(column_name)
            , type_(type)
            , is_primary_key(primary_key)
            , is_foreign_key(foreight_key){}
    Column(const std::string& column_name, DataType type,  bool primary_key, bool foreight_key, bool is_not_null)
            : name(column_name)
            , type_(type)
            , is_primary_key(primary_key)
            , is_foreign_key(foreight_key)
            , not_null(is_not_null){}
    Column(const std::string& column_name, DataType type, const std::vector<std::string>& ceil, bool primary_key, bool foreight_key, bool is_not_null)
            : name(column_name)
            , type_(type)
            , ceils(ceil)
            , is_primary_key(primary_key)
            , is_foreign_key(foreight_key)
            , not_null(is_not_null) {
        count = ceil.size();
    }
    Column(const std::string& column_name, DataType type,  bool primary_key, bool foreight_key, Column* ref, bool is_not_null)
            : name(column_name)
            , type_(type)
            , is_primary_key(primary_key)
            , is_foreign_key(foreight_key)
            , reference(ref)
            , not_null(is_not_null){}
    std::string getName() {
        return name;
    }

    uint64_t size() {
        return count;
    }

    DataType type() {
        return type_;
    };

    int getInt(uint64_t index) {
        return std::stoi(ceils[index].c_str());
    }

    float getFloat(uint64_t index) {
        return std::stof(ceils[index].c_str());
    }

    double getDouble(uint64_t index) {
        return std::stod(ceils[index].c_str());
    }

    std::string getString(uint64_t index) {
        return ceils[index];
    }

    bool getBool(uint64_t index) {
        if (ceils[index] == "true" or ceils[index] == "TRUE") {
            return true;
        } else {
            return false;
        }
    }

    bool isNull(uint64_t index) {
        if (ceils[index] == "null" or ceils[index] == "NULL") {
            return true;
        } else {
            return false;
        }
    }

    void make_not_null() {
        not_null = true;
    }

    //чекнуть на null
    void setValue(uint64_t index, const std::string& value) {
        ceils[index] = value;
    }

    void newRow(const std::string& value) {
        ceils.emplace_back(value);
        count++;
    }

    //getters
    bool getPrimaryKey() {
        return is_primary_key;
    }
    bool getForeignKey() {
        return is_foreign_key;
    }

    std::vector<std::string> getColumn() {
        return ceils;
    }

    void setColumn(const std::vector<std::string>& value) {
        ceils = value;
    }

    bool isNotNull() {
        return not_null;
    }
    ~Column() = default;
};
