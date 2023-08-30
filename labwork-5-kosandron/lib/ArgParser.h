#pragma once

#include <cstdint>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

namespace ArgumentParser {

enum Type {
       string = 1,
       integer = 2
};

class StringArgs {
public:
    int64_t wait_elements_count_ = 0;
    bool is_vector_ = false;
    char flag_ = ' ';
    std::string name_ = "";
    std::string value_ = "";
    std::string* one_element_link_ = nullptr;
    std::vector <std::string> values_;
    std::vector <std::string>* link_ = nullptr;
    std::string description_ = "";

    StringArgs& Default(const std::string& default_value);
    StringArgs& StoreValue(std::string& store_value);
    StringArgs& StoreValues(std::vector<std::string>& value_);
    StringArgs& MultiValue();
    StringArgs& MultiValue(uint64_t min_count);
    StringArgs& Positional();
};

class FlagArgs {
public:
    char short_flag_ = ' ';
    std::string name_ = "";
    bool value_ = false;
    bool* link_ = nullptr;
    std::string description_ = "";

    FlagArgs& Default(bool value_);
    FlagArgs& StoreValue(bool& value_);
};

class IntArgs {
public:
    int64_t wait_elements_count_ = 0;
    bool is_vector_ = false;
    char flag_ = ' ';
    std::string name_ = "";
    /*static*/ int value_ = 0;
    int* one_element_link_ = nullptr;
    std::vector <int> values_;
    std::vector <int>* link_ = nullptr;
    std::string description_ = "";

    IntArgs& Default(int value_);
    IntArgs& StoreValue(int& value_);
    IntArgs& StoreValues(std::vector<int>& value_);
    IntArgs& MultiValue();
    IntArgs& MultiValue(uint64_t min_count);
    IntArgs& Positional();
};

class ArgParser {
private:
    std::string help_flag_ = "-";
    std::string help_param_ = "--";
    bool help_ = false;
    std::string help_description_ = "";
    std::string name_;
public:
    std::vector<StringArgs> strings_;
    std::vector<FlagArgs> bool_values_;
    std::vector<IntArgs> int_values_;


    ArgParser(const std::string& a_name) {
        name_ = a_name;
    }

    Type paramType(const std::string& param);
    bool Parse (const std::vector<std::string>& input_parameters);
    bool Parse (int argc, char** argv);

    void ChangeFlag(const std::string& param);
    void ChangeFlag(char param);
    void ChangeInt(const std::string& param, int value);
    void ChangeInt(char flag, int value);
    void ChangeString(const std::string& param, const std::string& value);
    void ChangeString(char flag, const std::string& value);

    StringArgs& AddStringArgument(const std::string& param);
    StringArgs& AddStringArgument(char chr, const std::string& param);
    StringArgs& AddStringArgument(const std::string& param, const std::string& description_);
    StringArgs& AddStringArgument(char chr, const std::string& param, const std::string& description_);
    std::string GetStringValue(const std::string& param) const;
    std::string GetStringValue(const std::string& param, uint64_t position) const;

    IntArgs& AddIntArgument(const std::string& param);
    IntArgs& AddIntArgument(char chr, const std::string& param);
    IntArgs& AddIntArgument(const std::string& param, const std::string& description_);
    IntArgs& AddIntArgument(char chr, const std::string& param, const std::string& description_);
    int GetIntValue(const std::string& param) const;
    int GetIntValue(const std::string& param, uint64_t position) const;

    FlagArgs& AddFlag(char chr, const std::string& flag_name);
    FlagArgs& AddFlag(const std::string& flag_name);
    FlagArgs& AddFlag(const std::string& flag_name, const std::string& description_);
    FlagArgs& AddFlag(char chr, const std::string& flag_name, const std::string& description_);
    bool GetFlag(const std::string& flag_name) const;

    void AddHelp(char chr, const std::string& help_name, const std::string& description_);
    bool Help() const;
    std::string HelpDescription() const;
};

} // namespace ArgumentParser