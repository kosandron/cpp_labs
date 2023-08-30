#include "valid_value.h"

#include <string>
#include <cstring>
#include <algorithm>

bool IsInt(const std::string& str) {
    if (str.size() == 0) {
        return false;
    }
    if (str.size() == 1 and (str[0] == '+' or str[0] == '-')) {
        return false;
    }
    int start_index = 0;
    if (str[0] == '+' or str[0] == '-') {
        start_index = 1;
    }
    for (int i = start_index; i < str.size(); i++) {
        if (!std::isdigit(str[i])) {
            return false;
        }
    }
    return true;
}

bool IsFloat(const std::string &str) {
    if (str.size() == 1 and (str[0] == '.' or str[0] == '+' or str[0] == '-')) {
        return false;
    }
    if (std::count(str.begin(), str.end(), '.') != 1 || std::count(str.begin(), str.end(), '+') + std::count(str.begin(), str.end(), '-') > 1) {
        return false;
    }
    uint16_t point_position = str.find('.');
    if (str[str.size()-1] == '.' or str[0] == '.') {
        return false;
    }
    int start_index = 0;
    if (str[0] == '+' or str[0] == '-') {
        start_index = 1;
    }
    for (int i = start_index; i < str.size(); i++) {
        if (!std::isdigit(str[i]) && i != point_position) {
            return false;
        }
    }

    return true;
}


bool IsBool(const std::string &str) {
    return (str == "false" or str == "true");
}

bool IsKey(const std::string &str) {
    if (str.size() == 0) {
        return false;
    }

    for (int i = 0; i < str.size(); i++) {
        if (!(str[i] == 45 || str[i] == 95 || (str[i] > 47 && str[i] < 58) || (str[i] > 64 && str[i] < 91) || (str[i] > 96 && str[i] < 123))) { //  see ASCII Table
            return false;
        }
    }
    return true;
}

bool ValidValue(const std::string &str) {
    return ValidBool(str) + ValidFloat(str) + ValidInt(str) + ValidString(str) + ValidArr(str) + ValidTest(str);
}

bool IsString(const std::string &str) {
    return (std::count(str.begin(), str.end(), '"') == 2 && str[0] == '"' && str[str.size() - 1] == '"');
}
