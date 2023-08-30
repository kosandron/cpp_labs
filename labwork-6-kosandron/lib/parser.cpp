#include "parser.h"
#include <iostream>
#include <fstream>

omfl::Variable::Variable(omfl::Variable* parent) {
    this->parent = parent;
    this->status = omfl::array;
}

omfl::Variable::Variable() {
    parent = nullptr;
    status = section;
    read_status = true;
}

void omfl::Variable::wrongFormat() {
    this->read_status = false;
}

void omfl::Variable::changeStatus(const omfl::LineType& new_status) {
    this->status = new_status;
}

void omfl::Variable::printStructure(uint16_t k) const{
    for (int i = 0 ; i < this->array.size(); i++) {
        for (int j = 0; j < k; j++)
            std::cout << " ";
        if (array[i].status == section){
            std::cout << '[' << array[i].name << ']';
        } else {
            std::cout << array[i].name;
        }

        if (array[i].array.size() != 0) {
            array[i].printStructure(k + 1);
        }
    }
}

const omfl::Variable& omfl::Variable::Get(const std::string& param, int16_t pos) const{
    std::string_view stroka = param;
    int16_t next_point_position = stroka.find('.', pos);
    std::string variable_name = static_cast<std::string>(stroka.substr(pos, (next_point_position > 0 ? next_point_position : stroka.size()) - pos));
    if (next_point_position == std::string::npos) {
        for (size_t i = 0; i < this->array.size(); i++) {
            if (variable_name == this->array[i].name) {
                //std::cout << "Hura" << '\n';
                return this->array[i];
            }
        }
    } else {
        for (size_t i = 0; i < this->array.size(); i++) {
            if (variable_name == this->array[i].name) {
                return this->array[i].Get(param, next_point_position + 1);
            }
        }
    }
}

omfl::Variable& omfl::Variable::ChangeSection(const std::string& param, int16_t pos) {
    std::string_view stroka = param;
    int16_t next_point_position = stroka.find('.', pos);
    std::string variable_name = static_cast<std::string>(stroka.substr(pos, (next_point_position > 0 ? next_point_position : param.size()) - pos));
    bool flag = false;
    if (next_point_position == std::string::npos) {
        for (size_t i = 0; i < this->array.size(); i++) {
            if (variable_name == this->array[i].name) {
                flag = true;
                return this->array[i];
            }
        }
        if (!flag) {
            //std::cout << "Created " << variable_name << '\n';
            omfl::Variable a;
            a.name = variable_name;
            this->array.push_back(a);
            return (this->array.back());
        }
    } else {
        for (size_t i = 0; i < this->array.size(); i++) {
            if (variable_name == this->array[i].name) {
                flag = true;
                return this->array[i].ChangeSection(param, next_point_position + 1);
            }
        }
        if (!flag) {
            omfl::Variable a;
            a.name = variable_name;
            this->array.push_back(a);
            return (this->array.back()).ChangeSection(param, next_point_position + 1);
        }
    }
}

bool newElement(std::string param, const omfl::Variable* place) {
    for (size_t i = 0; i < place->array.size(); i++) {
        if (param == place->array[i].name) {
            return false;
        }
    }
    return true;
}

omfl::Variable* omfl::Variable::GoUp() const {
    return this->parent;
}

omfl::Variable omfl::parse(const std::string& str) {
    omfl::Variable parser;

    omfl::Variable* temp_place = &parser;
    std::string section_name = "";
    std::string key_name = "";
    std::string value = "";
    bool flag = false;
    bool wait_value = false;
    uint16_t ravno_count = 0;
    int right_scope = 0;
    omfl::ReadType status = omfl::void_type;
    omfl::LineType line_status = omfl::not_defined;

    for (size_t i = 0; i < str.size(); i++) {
        if (str[i] == '\n') { //  add value to database (endl) - главное значение в строке
            if (line_status == omfl::key_value) {
                omfl::Variable a(temp_place);
                a.name = key_name;
                a.value = value;
                a.changeStatus(key_value); //  просто статус переменной
                if (a.IsKey() && (a.IsBool() || a.IsFloat() || a.IsInt() || a.IsString()) && newElement(key_name, temp_place)) {
                    temp_place->array.push_back(a);
                } else {
                    parser.wrongFormat();
                }
                if (ravno_count != 1) {
                    parser.wrongFormat();
                }

            } else if (line_status == omfl::section) { //  секция
                if (IsSection(value)) {
                    temp_place = &(parser.ChangeSection(value));
                } else {
                    parser.wrongFormat();
                }
            }

            key_name = "";
            value = "";
            status = omfl::void_type;
            line_status = omfl::not_defined;
            ravno_count = 0;
            flag = false;
            continue;
        }

        if (line_status == not_defined && status == void_type && str[i] == ' ') { //  лишние пробелы в начале
            continue;
        }

        if (str[i] == '#') { //  комментарий - первый по приоритету
            status = omfl::comment;
            continue;
        }

        if (status == omfl::comment) { //  comment - если комментарий - игнорим все
            continue;
        }

        if (status == omfl::string_type) { //  end of string and reading of string - читаем строку
            value += str[i];
            if (str[i] != '\"') {
                continue;
            } else {
                status = omfl::void_type;
                if (line_status == omfl::array) {
                    status = omfl::array_type;
                }
                continue;
            }
        }

        if (str[i] == '[') { //  begin/end of section/array
            if (key_name == "") { // условие секции
                status = omfl::section_type;
                line_status = omfl::section;
                continue;
            } else {
                right_scope++; //  проверка правильной скобочной последовательности
                if (right_scope < 0) {
                    parser.wrongFormat();
                }
                status = omfl::array_type; // статусы массивные
                line_status = omfl::array;
                omfl::Variable a(temp_place);
                if (flag == false) { //  defense of name for underarrays
                    a.name = key_name;
                    flag = true;
                }
                if (!newElement(key_name, temp_place) && key_name != "" || ravno_count != 1) {
                    parser.wrongFormat();
                }
                temp_place->array.push_back(a); //  переход на секцию ниже
                temp_place = &temp_place->array.back();
                continue;
            }
        } else if (str[i] == ']') {
            if (line_status == array) {
                right_scope--;
                if (right_scope < 0) {
                    parser.wrongFormat();
                }
                if (value != "") {
                    omfl::Variable a;
                    a.value = value;
                    if ((a.IsBool() || a.IsFloat() || a.IsInt() || a.IsString())) {
                        temp_place->array.push_back(a);
                    } else {
                        parser.wrongFormat();
                    }
                    value = "";
                    status = array_type;
                    wait_value = false;
                } else if (wait_value) {
                    parser.wrongFormat(); //  a = [9, 7], - not correct format
                }

                if (temp_place->array.size() == 0) {
                    parser.wrongFormat();
                }
                temp_place = temp_place->GoUp(); //  переход на директорию выше
                status = omfl::array_type;
                if (i + 1 < str.size() && str[i + 1] == ',' && right_scope > 0) { //  ], - такой случай
                    i++;
                    wait_value = true;
                }
                continue;
            } else {
                status = omfl::void_type; //  конец имени секции
            }
        }

        if (status == section_type) { //  запись секции
            value += str[i];
        }


        if (str[i] == ',' and line_status == array and status != string_type) { //  adding of variable to array
            omfl::Variable a;
            a.value = value;
            if ((a.IsBool() || a.IsFloat() || a.IsInt() || a.IsString())) {
                temp_place->array.push_back(a);
            } else {
                parser.wrongFormat();
            }

            value = "";
            status = array_type;
            continue;
        }

        if (status == array_type && str[i] != ' ' && value == "") { //  begin of reading variable (element) for array
            value += str[i];
            status = value_type;
            continue;
        }

        if (line_status == omfl::not_defined) { //  begin of reading key - до этого ни один if не сработал
            status = omfl::key_type;
            line_status = key_value;
        }


        if (str[i] == ' ' or str[i] == '=') { //  read keys and values
            if (str[i] == '=') {
                ravno_count++;
            }
            if (status == omfl::void_type) {
                continue;
            } else if (status == omfl::key_type || status == omfl::value_type) { //  конец записи ключа/значения
                status = omfl::void_type;
            }
        }

        if (line_status == key_value && key_name != "" && value != "" && status == void_type && str[i] != ' ') { //  defense from strange words aka key1 = value1 gergerg
            parser.wrongFormat();
        }

        if (key_name != "") {
            if (status == omfl::void_type && str[i] != ' ' && str[i] != '=') { //  go to read variable (start of declaration)
                if (ravno_count != 1) {
                    parser.wrongFormat();
                }
                status = omfl::value_type;
                line_status = omfl::key_value;
                if (str[i] == '\"') {
                    status = omfl::string_type;
                    value += '\"';
                }
            }
        }

        if (status == omfl::key_type) { //  reading of key or value
            key_name += str[i];
        } else if (status == omfl::value_type) {
            value += str[i];
        }
    }

    if (line_status == omfl::key_value) {
        omfl::Variable a(temp_place);
        a.name = key_name;
        a.value = value;
        a.changeStatus(key_value);
        if (a.IsKey() && (a.IsBool() || a.IsFloat() || a.IsInt() || a.IsString()) && newElement(key_name, temp_place)) {
            temp_place->array.push_back(a);
        } else {
            parser.wrongFormat();
        }
        if (ravno_count != 1) {
            parser.wrongFormat();
        }

    } else if (line_status == omfl::section) {
        temp_place = &parser.ChangeSection(key_name);
    }

    if (right_scope > 0) {
        parser.wrongFormat();
    }

    return parser;
}

omfl::Variable omfl::parse(const std::filesystem::path& path) {
    omfl::Variable parser;

    omfl::Variable* temp_place = &parser;
    std::string section_name = "";
    std::string key_name = "";
    std::string value = "";
    bool flag = false;
    bool pass = false;
    bool wait_value = false;
    uint16_t ravno_count = 0;
    int right_scope = 0;
    omfl::ReadType status = omfl::void_type;
    omfl::LineType line_status = omfl::not_defined;

    char symbol;
    std::ifstream f(path);
    while (f.get(symbol)) {
        if (pass && symbol == ',') {
            pass = false;
            wait_value = true;
            continue;
        }

        if (symbol == '\n') { //  add value to database (endl)
            if (line_status == omfl::key_value) {
                omfl::Variable a(temp_place);
                a.name = key_name;
                a.value = value;
                a.changeStatus(key_value);
                if (a.IsKey() && (a.IsBool() || a.IsFloat() || a.IsInt() || a.IsString()) && newElement(key_name, temp_place)) {
                    temp_place->array.push_back(a);
                } else {
                    parser.wrongFormat();
                }
                if (ravno_count != 1) {
                    parser.wrongFormat();
                }

            } else if (line_status == omfl::section) {
                if (IsSection(value)) {
                    temp_place = &(parser.ChangeSection(value));
                } else {
                    parser.wrongFormat();
                }
            }

            key_name = "";
            value = "";
            status = omfl::void_type;
            line_status = omfl::not_defined;
            ravno_count = 0;
            flag = false;
            continue;
        }

        if (line_status == not_defined && status == void_type && symbol == ' ') {
            continue;
        }

        if (symbol == '#') {
            status = omfl::comment;
            continue;
        }

        if (status == omfl::comment) { //  comment
            continue;
        }

        if (status == omfl::string_type) { //  end of string and reading of string
            value += symbol;
            if (symbol != '\"') {
                continue;
            } else {
                status = omfl::void_type;
                if (line_status == omfl::array) {
                    status = omfl::array_type;
                }
                continue;
            }
        }

        if (symbol == '[') { //  begin/end of section/array
            if (key_name == "") {
                status = omfl::section_type;
                line_status = omfl::section;
                continue;
            } else {
                right_scope++;
                if (right_scope < 0) {
                    parser.wrongFormat();
                }
                status = omfl::array_type;
                line_status = omfl::array;
                omfl::Variable a(temp_place);
                if (flag == false) { //  defense of name for underarrays
                    a.name = key_name;
                    flag = true;
                }
                if (!newElement(key_name, temp_place) && key_name != "" || ravno_count != 1) {
                    parser.wrongFormat();
                }
                temp_place->array.push_back(a);
                temp_place = &temp_place->array.back();
                continue;
            }
        } else if (symbol == ']') {
            if (line_status == array) {
                right_scope--;
                if (right_scope < 0) {
                    parser.wrongFormat();
                }
                if (value != "") {
                    omfl::Variable a;
                    a.value = value;
                    if ((a.IsBool() || a.IsFloat() || a.IsInt() || a.IsString())) {
                        temp_place->array.push_back(a);
                    } else {
                        parser.wrongFormat();
                    }
                    value = "";
                    status = array_type;
                    wait_value = false;
                } else if (wait_value) {
                    parser.wrongFormat(); //  a = [9, 7], - not correct format
                }
                //переход на директорию выше
                if (temp_place->array.size() == 0) {
                    parser.wrongFormat();
                }
                temp_place = temp_place->GoUp();
                status = omfl::array_type;
                pass = true;
                continue;
            } else {
                status = omfl::void_type;
            }
        }

        if (status == section_type) {
            value += symbol;
        }


        if (symbol == ',' and line_status == array and status != string_type) { //  adding of variable to array
            omfl::Variable a;
            a.value = value;
            if ((a.IsBool() || a.IsFloat() || a.IsInt() || a.IsString())) {
                temp_place->array.push_back(a);
            } else {
                parser.wrongFormat();
            }

            value = "";
            status = array_type;
            continue;
        }

        if (status == array_type && symbol != ' ' && value == "") { //  begin of reading variable (element) for array
            value += symbol;
            status = value_type;
            continue;
        }

        if (line_status == omfl::not_defined) { //  begin of reading key
            status = omfl::key_type;
            line_status = key_value;
        }


        if (symbol == ' ' or symbol == '=') { //  read keys and values
            if (symbol == '=') {
                ravno_count++;
            }
            if (status == omfl::void_type) {
                continue;
            } else if (status == omfl::key_type || status == omfl::value_type) {
                status = omfl::void_type;
            }
        }

        if (line_status == key_value && key_name != "" && value != "" && status == void_type && symbol != ' ') { //  defense from strange words
            parser.wrongFormat();
        }

        if (key_name != "") {
            if (status == omfl::void_type && symbol != ' ' && symbol != '=') { //  go to read variable (start of declaration)
                if (ravno_count != 1) {
                    parser.wrongFormat();
                }
                status = omfl::value_type;
                line_status = omfl::key_value;
                if (symbol == '\"') {
                    status = omfl::string_type;
                    value += '\"';
                }
            }
        }

        if (status == omfl::key_type) { //  reading of key or value
            key_name += symbol;
        } else if (status == omfl::value_type) {
            value += symbol;
        }
    }

    if (line_status == omfl::key_value) {
        omfl::Variable a(temp_place);
        a.name = key_name;
        a.value = value;
        a.changeStatus(key_value);
        if (a.IsKey() && (a.IsBool() || a.IsFloat() || a.IsInt() || a.IsString()) && newElement(key_name, temp_place)) {
            temp_place->array.push_back(a);
        } else {
            parser.wrongFormat();
        }
        if (ravno_count != 1) {
            parser.wrongFormat();
        }

    } else if (line_status == omfl::section) {
        temp_place = &parser.ChangeSection(key_name);
    }

    if (right_scope > 0) {
        parser.wrongFormat();
    }

    return parser;
}

//  Valid metods
bool omfl::Variable::valid() const {
    return this->read_status;
}

 bool omfl::IsSection(const std::string &name) {
     if (name.size() == 0) {
         return false;
     }

     if (name[0] == '.' or name[name.size() - 1] == '.') {
         return false;
     }

     if (name.find("..") != std::string::npos) {
         return false;
     }

     for (int i = 0; i < name.size(); i++) {
         if (!(name[i] == '_' || name[i] == '-' || name[i] == '.' || (name[i] >= '0' && name[i] <= '9') || (name[i] >= 'a' && name[i] <= 'z') || (name[i] >= 'A' && name[i] <= 'Z'))) { //  see ASCII Table
             return false;
         }
     }
     return true;
}

bool omfl::Variable::IsInt() const{
    if (value.size() == 0) {
        return false;
    }
    if (value.size() == 1 and (value[0] == '+' or value[0] == '-')) {
        return false;
    }

    int start_index = 0;
    if (value[0] == '+' or value[0] == '-') {
        start_index = 1;
    }
    for (int i = start_index; i < value.size(); i++) {
        if (!std::isdigit(value[i])) {
            return false;
        }
    }
    return true;
}

bool omfl::Variable::IsFloat() const {
    if (value.size() == 0) {
        return false;
    }
    if (value.size() == 1 and (value[0] == '.' or value[0] == '+' or value[0] == '-')) {
        return false;
    }
    if (std::count(value.begin(), value.end(), '.') != 1 || std::count(value.begin(), value.end(), '+') + std::count(value.begin(), value.end(), '-') > 1) {
        return false;
    }
    uint16_t point_position = value.find('.');
    if (value[value.size()-1] == '.' or value[0] == '.') {
        return false;
    }

    int start_index = 0;
    if (value[0] == '+' or value[0] == '-') {
        start_index = 1;
    }
    int k = 0;
    for (int i = start_index; i < point_position; i++) {
        if (!std::isdigit(value[i])) {
            return false;
        }
        k++;
    }
    if (k == 0) {
        return false;
    }
    k = 0;
    for (int i = point_position + 1; i < value.size(); i++) {
        if (!std::isdigit(value[i])) {
            return false;
        }
        k++;
    }
    if (k == 0) {
        return false;
    }

    return true;
}


bool omfl::Variable::IsBool() const {
    return (value == "false" or value == "true");
}

bool omfl::Variable::IsKey() const{
    if (name.size() == 0) {
        return false;
    }

    for (int i = 0; i < name.size(); i++) {
        if (!(name[i] == '_' || name[i] == '-' || (name[i] >= '0' && name[i] <= '9') || (name[i] >= 'a' && name[i] <= 'z') || (name[i] >= 'A' && name[i] <= 'Z'))) { //  see ASCII Table
            return false;
        }
    }
    return true;
}

bool omfl::Variable::IsString() const {
    return (std::count(this->value.begin(), value.end(), '"') == 2 && value[0] == '"' && value[value.size() - 1] == '"');
}

bool omfl::Variable::IsArray() const {
    return (this->status == omfl::array);
}

 //  Return value
int32_t omfl::Variable::AsInt() const {
    return std::stoi(value);
}

float omfl::Variable::AsFloat() const {
    return std::stof(value);
}

bool omfl::Variable::AsBool() const {
    return (value == "true");
}

std::string omfl::Variable::AsString() const {
    return value.substr(1, value.size() - 2);
}

const omfl::Variable& omfl::Variable::operator[](uint32_t index) const {
    if (index >= this->array.size() or index < 0) {
        static omfl::Variable a;
        a.value = "SegmentationFault";
        return a;
    }
    return this->array[index];
}

int32_t omfl::Variable::AsIntOrDefault(int32_t param) const {
    if (this->IsInt()) {
        return this->AsInt();
    } else {
        return param;
    }
}

float omfl::Variable::AsFloatOrDefault(float param) const {
    if (this->IsFloat()) {
        return this->AsFloat();
    } else {
        return param;
    }
}

bool omfl::Variable::AsBoolOrDefault(bool param) const {
    if (this->IsBool()) {
        return this->AsBool();
    } else {
        return param;
    }
}

std::string omfl::Variable::AsStringOrDefault(const std::string& param) const {
    if (this->IsString()) {
        return this->AsString();
    } else {
        return param;
    }
}
