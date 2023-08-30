#include "ArgParser.h"

void ArgumentParser::ArgParser::ChangeFlag(const std::string& param) {
    for (size_t i = 0; i < bool_values_.size(); i++) {
        if (bool_values_[i].name_ == param) {
            bool_values_[i].value_ = true;
            if (bool_values_[i].link_ != nullptr) {
                *bool_values_[i].link_ = true;
            }
            return;
        }
    }
}

void ArgumentParser::ArgParser::ChangeFlag(char param) {
    for (size_t i = 0; i < bool_values_.size(); i++) {
        if (bool_values_[i].short_flag_ == param) {
            bool_values_[i].value_ = true;
            if (bool_values_[i].link_ != nullptr) {
                *bool_values_[i].link_ = true;
            }
            return;
        }
    }
}

void ArgumentParser::ArgParser::ChangeInt(const std::string& param, int value) {
    for (size_t i = 0; i < int_values_.size(); i++) {
        if (int_values_[i].name_ == param) {
            if (int_values_[i].is_vector_) {
                int_values_[i].values_.push_back(value);
                if (int_values_[i].link_ != nullptr) {
                    int_values_[i].link_->push_back(value);
                }
            } else {
                int_values_[i].value_ = value;
                if (int_values_[i].one_element_link_ != nullptr) {
                    *int_values_[i].one_element_link_ = value;
                }
            }
            int_values_[i].wait_elements_count_--;
            return;
        }
    }
}

void ArgumentParser::ArgParser::ChangeInt(char flag, int value) {
    for (size_t i = 0; i < int_values_.size(); i++) {
        if (int_values_[i].flag_ == flag) {
            if (int_values_[i].is_vector_) {
                int_values_[i].values_.push_back(value);
                if (int_values_[i].link_ != nullptr) {
                    int_values_[i].link_->push_back(value);
                }
            } else {
                int_values_[i].value_ = value;
                if (int_values_[i].one_element_link_ != nullptr) {
                    *int_values_[i].one_element_link_ = value;
                }
            }
            int_values_[i].wait_elements_count_--;
            return;
        }
    }
}

void ArgumentParser::ArgParser::ChangeString(const std::string& param, const std::string& value) {
    for (size_t i = 0; i < strings_.size(); i++) {
        if (strings_[i].name_ == param) {
            if (strings_[i].is_vector_) {
                strings_[i].values_.push_back(value);
                if (strings_[i].link_ != nullptr) {
                    strings_[i].link_->push_back(value);
                }
            } else {
                strings_[i].value_ = value;
                if (strings_[i].one_element_link_ != nullptr) {
                    *strings_[i].one_element_link_ = value;
                }
            }
            strings_[i].wait_elements_count_--;
            return;
        }
    }
}

void ArgumentParser::ArgParser::ChangeString(char flag, const std::string& value) {
    for (size_t i = 0; i < strings_.size(); i++) {
        if (strings_[i].flag_ == flag) {
            if (strings_[i].is_vector_) {
                strings_[i].values_.push_back(value);
                if (strings_[i].link_ != nullptr) {
                    strings_[i].link_->push_back(value);
                }
            } else {
                strings_[i].value_ = value;
                if (strings_[i].one_element_link_ != nullptr) {
                    *strings_[i].one_element_link_ = value;
                }
            }
            strings_[i].wait_elements_count_--;
            return;
        }
    }
}

ArgumentParser::Type ArgumentParser::ArgParser::paramType(const std::string& param) {
    bool is_flag = (param.size() == 1);
    for (size_t i = 0; i < int_values_.size(); i++) {
        if (is_flag) {
            if (int_values_[i].flag_ == param[0]) {
                return Type::integer;
            }
        } else {
            if (int_values_[i].name_ == param) {
                return Type::integer;
            }
        }
    }

    return Type::string;
}

bool ArgumentParser::ArgParser::Parse(const std::vector<std::string>& input_parameters) {
    Type positional_type;
    uint64_t position_index = 0;
    bool positional = false;
    for (size_t i = 0; i < strings_.size(); i++) {
        if (strings_[i].is_vector_) {
            positional_type = Type::string;
            position_index = i;
            positional = true;
            break;
        }
    }
    if (positional_type != Type::string) {
        for (size_t i = 0; i < int_values_.size(); i++) {
            if (int_values_[i].is_vector_) {
                positional_type = Type::integer;
                position_index = i;
                positional = true;
                break;
            }
        }
    }

    std::string param = "";
    std::string value = "";
    for (size_t i = 1; i < input_parameters.size(); i++) {
        if (input_parameters[i] == help_flag_ or input_parameters[i] == help_param_) {
            help_ = true;
            return true;
        }

        std::string_view input_line = input_parameters[i];
        uint64_t line_size = input_line.size();
        if (input_line.find('=')  == std::string::npos) {

            if (positional && (line_size == 1 or (line_size > 1 && std::isdigit(input_parameters[i][1])))) {
                if (positional_type == Type::integer) {
                    int_values_[position_index].values_.push_back(std::stoi(static_cast<std::string>(input_line)));
                    if (int_values_[position_index].link_ != nullptr) {
                        int_values_[position_index].link_->push_back(std::stoi(static_cast<std::string>(input_line)));
                    }
                    int_values_[position_index].wait_elements_count_--;
                } else {
                    strings_[position_index].values_.push_back(static_cast<std::string>(input_line));
                    if (strings_[position_index].link_ != nullptr) {
                        strings_[position_index].link_->push_back(static_cast<std::string>(input_line));
                    }
                    strings_[position_index].wait_elements_count_--;
                }
            } else {
                if (line_size > 2 && input_line[0] == '-' && input_line[1] == '-') {
                    param = static_cast<std::string>(input_line.substr(2, std::string_view ::npos));
                    ChangeFlag(param);
                } else {
                    for (size_t j = 1; j < line_size; j++) {
                        ChangeFlag(input_line[j]);
                    }
                }
            }
        } else {
            uint64_t ravno_position = input_line.find('=');
            if (input_line[1] == '-') {
                param = static_cast<std::string>(input_line.substr(2, ravno_position - 2)); //  makes a -param
            } else {
                param = static_cast<std::string>(input_line.substr(1, ravno_position - 1));
            }
            value = static_cast<std::string>(input_line.substr(ravno_position + 1, std::string_view ::npos));
            if (paramType(param) == Type::integer) {
                if (param.size() == 1) {
                    ChangeInt(param[0], std::stoi(value));
                } else {
                    ChangeInt(param, std::stoi(value));
                }
            } else {
                if (param.size() == 1) {
                    ChangeString(param[0], value);
                } else {
                    ChangeString(param, value);
                }
            }
        }
    }

    for (size_t i = 0; i < int_values_.size(); i++) {
        if (int_values_[i].wait_elements_count_ > 0) {
            return false;
        }
    }
    for (size_t i = 0; i < strings_.size(); i++) {
        if (strings_[i].wait_elements_count_ > 0) {
            return false;
        }
    }

    return true;
}

bool ArgumentParser::ArgParser::Parse(int argc, char** argv) {
    Type positional_type;
    uint64_t position_index = 0;
    bool positional = false;
    for (size_t i = 0; i < strings_.size(); i++) {
        if (strings_[i].is_vector_) {
            positional_type = Type::string;
            position_index = i;
            positional = true;
            break;
        }
    }
    if (positional_type != Type::string) {
        for (size_t i = 0; i < int_values_.size(); i++) {
            if (int_values_[i].is_vector_) {
                positional_type = Type::integer;
                position_index = i;
                positional = true;
                break;
            }
        }
    }

    std::string param = "";
    std::string value = "";
    std::string_view input_line;

    for (size_t i = 1; i < argc; i++) {
        input_line = argv[i];
        uint64_t line_size = input_line.size();
        if (input_line == help_flag_ or input_line == help_param_) {
            help_ = true;
            return true;
        }

        if (input_line.find('=')  == std::string::npos) {

            if (positional && (line_size == 1 or (line_size > 1 && std::isdigit(input_line[1])))) {
                if (positional_type == Type::integer) {
                    int_values_[position_index].values_.push_back(std::stoi(static_cast<std::string>(input_line)));
                    if (int_values_[position_index].link_ != nullptr) {
                        int_values_[position_index].link_->push_back(std::stoi(static_cast<std::string>(input_line)));
                    }
                    int_values_[position_index].wait_elements_count_--;
                } else {
                    strings_[position_index].values_.push_back(static_cast<std::string>(input_line));
                    if (strings_[position_index].link_ != nullptr) {
                        strings_[position_index].link_->push_back(static_cast<std::string>(input_line));
                    }
                    strings_[position_index].wait_elements_count_--;
                }
            } else {
                if (line_size > 2 && input_line[0] == '-' && input_line[1] == '-') {
                    param = static_cast<std::string>(input_line.substr(2, std::string_view ::npos));
                    ChangeFlag(param);
                } else {
                    for (size_t j = 1; j < line_size; j++) {
                        ChangeFlag(input_line[j]);
                    }
                }
            }
        } else {
            uint64_t ravno_position = input_line.find('=');
            if (input_line[1] == '-') {
                param = static_cast<std::string>(input_line.substr(2, ravno_position - 2)); //  makes a -param
            } else {
                param = static_cast<std::string>(input_line.substr(1, ravno_position - 1));
            }
            value = static_cast<std::string>(input_line.substr(ravno_position + 1, std::string_view ::npos));
            if (paramType(param) == Type::integer) {
                if (param.size() == 1) {
                    ChangeInt(param[0], std::stoi(value));
                } else {
                    ChangeInt(param, std::stoi(value));
                }
            } else {
                if (param.size() == 1) {
                    ChangeString(param[0], value);
                } else {
                    ChangeString(param, value);
                }
            }
        }
    }

    for (size_t i = 0; i < int_values_.size(); i++) {
        if (int_values_[i].wait_elements_count_ > 0) {
            std::cout << int_values_[i].wait_elements_count_ << " ";
            return false;
        }
    }
    for (size_t i = 0; i < strings_.size(); i++) {
        if (strings_[i].wait_elements_count_ > 0) {
            return false;
        }
    }

    return true;
}

 //  String functions
ArgumentParser::StringArgs& ArgumentParser::ArgParser::AddStringArgument(const std::string& param) {
     StringArgs new_param;
     new_param.name_ = param;
     new_param.wait_elements_count_++;
     new_param.description_ += "     --" + param + "=<string>,  ";
     strings_.push_back(new_param);

     return strings_.back();
}

ArgumentParser::StringArgs& ArgumentParser::ArgParser::AddStringArgument(char chr, const std::string& param) {
    StringArgs new_param;
    new_param.name_ = param;
    new_param.flag_ = chr;
    new_param.wait_elements_count_++;
    new_param.description_ = ('-');
    new_param.description_ += chr;
    new_param.description_ += ",  --" + param + "=<string>,  ";
    strings_.push_back(new_param);

    return strings_.back();
}

ArgumentParser::StringArgs& ArgumentParser::ArgParser::AddStringArgument(const std::string& param, const std::string& description_) {
    StringArgs new_param;
    new_param.name_ = param;
    new_param.description_ += "     --" + param + "=<string>,  " + description_;
    new_param.wait_elements_count_++;
    strings_.push_back(new_param);

    return strings_.back();
}

ArgumentParser::StringArgs& ArgumentParser::ArgParser::AddStringArgument(char chr, const std::string& param, const std::string& description_) {
    StringArgs new_param;
    new_param.name_ = param;
    new_param.flag_ = chr;
    new_param.description_ = ('-');
    new_param.description_ += chr;
    new_param.description_ += (",  --" + param + "=<string>,  " + description_);
    new_param.wait_elements_count_++;
    strings_.push_back(new_param);

    return strings_.back();
}

ArgumentParser::StringArgs& ArgumentParser::StringArgs::Positional() {
    //positional = true;
    this->is_vector_ = true;

    return *this;
}

ArgumentParser::StringArgs& ArgumentParser::StringArgs::Default(const std::string& default_value) {
    this->value_ = default_value;
    this->wait_elements_count_--;
    this->description_ += " [default = " + default_value + "]";

    return *this;
}

ArgumentParser::StringArgs& ArgumentParser::StringArgs::StoreValue(std::string& store_value) {
    this->one_element_link_ = &store_value;

    return *this;
}

ArgumentParser::StringArgs& ArgumentParser::StringArgs::StoreValues(std::vector<std::string>& store_value) {
    this->link_ = &store_value;

    return *this;
}

ArgumentParser::StringArgs& ArgumentParser::StringArgs::MultiValue() {
    this->is_vector_ = true;
    this->description_ += " [repeated]";

    return *this;
}

ArgumentParser::StringArgs& ArgumentParser::StringArgs::MultiValue(uint64_t min_count) {
    this->is_vector_ = true;
    this->wait_elements_count_ = min_count;
    this->description_ += " [repeated, min args = " + std::to_string(min_count) + "]";

    return *this;
}

std::string ArgumentParser::ArgParser::GetStringValue(const std::string& param, uint64_t position) const {
    for (size_t i = 0; i < strings_.size(); i++) {
        if (strings_[i].name_ == param) {
            return strings_[i].values_[position];
        }
    }
}

 //  Int functions
ArgumentParser::IntArgs& ArgumentParser::ArgParser::AddIntArgument(const std::string& param) {
    IntArgs new_param;
    new_param.name_ = param;
    new_param.wait_elements_count_++;
    new_param.description_ += "     --" + param + "=<int>,  ";
    int_values_.push_back(new_param);

    return int_values_.back();
}

ArgumentParser::IntArgs& ArgumentParser::ArgParser::AddIntArgument(char chr, const std::string& param) {
    IntArgs new_param;
    new_param.name_ = param;
    new_param.flag_ = chr;
    new_param.wait_elements_count_++;
    new_param.description_ = ('-');
    new_param.description_ += chr;
    new_param.description_ += ",  --" + param + "=<int>,  ";
    int_values_.push_back(new_param);

    return int_values_.back();
}


ArgumentParser::IntArgs& ArgumentParser::ArgParser::AddIntArgument(const std::string& param, const std::string& description_) {
    IntArgs new_param;
    new_param.name_ = param;
    new_param.description_ += "     --" + param + "=<int>,  " + description_;
    new_param.wait_elements_count_++;
    int_values_.push_back(new_param);

    return int_values_.back();
}

ArgumentParser::IntArgs& ArgumentParser::ArgParser::AddIntArgument(char chr, const std::string& param, const std::string& description_) {
    IntArgs new_param;
    new_param.name_ = param;
    new_param.flag_ = chr;
    new_param.description_ = ('-');
    new_param.description_ += chr;
    new_param.description_ += (",  --" + param + "=<int>,  " + description_);
    new_param.wait_elements_count_++;
    int_values_.push_back(new_param);

    return int_values_.back();
}

ArgumentParser::IntArgs& ArgumentParser::IntArgs::MultiValue() {
    this->is_vector_ = true;
    this->description_ += " [repeated]";

    return *this;
}

ArgumentParser::IntArgs& ArgumentParser::IntArgs::MultiValue(uint64_t min_count) {
    this->is_vector_ = true;
    this->wait_elements_count_ = min_count;
    this->description_ += " [repeated, min args = " + std::to_string(min_count) + "]";

    return *this;
}

ArgumentParser::IntArgs& ArgumentParser::IntArgs::Positional() {
    //positional = true;
    this->is_vector_ = true;

    return *this;
}

ArgumentParser::IntArgs& ArgumentParser::IntArgs::Default(int default_value) {
    this->value_ = default_value;
    this->wait_elements_count_--;
    this->description_ += " [default = " + std::to_string(default_value) + "]";

    return *this;
}

ArgumentParser::IntArgs& ArgumentParser::IntArgs::StoreValue(int& store_value) {
    this->one_element_link_ = &store_value;

    return *this;
}

ArgumentParser::IntArgs& ArgumentParser::IntArgs::StoreValues(std::vector<int>& store_value) {
    this->link_ = &store_value;

    return *this;
}

int ArgumentParser::ArgParser::GetIntValue(const std::string& param) const {
    for (size_t i = 0; i < int_values_.size(); i++) {
        if (int_values_[i].name_ == param) {
            return int_values_[i].value_;
        }
    }

    return -1;
}

int ArgumentParser::ArgParser::GetIntValue(const std::string& param, uint64_t position) const {
    for (size_t i = 0; i < int_values_.size(); i++) {
        if (int_values_[i].name_ == param) {
            return int_values_[i].values_[position];
        }
    }
}

 //  flag_ functions
ArgumentParser::FlagArgs& ArgumentParser::ArgParser::AddFlag(char chr, const std::string& flag_name) {
    FlagArgs new_param;
    new_param.name_ = flag_name;
    new_param.short_flag_ = chr;
    new_param.description_ = ('-');
    new_param.description_ += chr;
    new_param.description_ += ",  --" + flag_name + ",  ";
    bool_values_.push_back(new_param);

    return bool_values_.back();
}

ArgumentParser::FlagArgs& ArgumentParser::ArgParser::AddFlag(const std::string& flag_name) {
    FlagArgs new_param;
    new_param.name_ = flag_name;
    new_param.description_ += ",  --" + flag_name + ",  ";
    bool_values_.push_back(new_param);

    return bool_values_.back();
}

ArgumentParser::FlagArgs& ArgumentParser::ArgParser::AddFlag(const std::string& flag_name, const std::string& description_) {
    FlagArgs new_param;
    new_param.name_ = flag_name;
    new_param.description_ += "     --" + flag_name + ",  " + description_;
    bool_values_.push_back(new_param);

    return bool_values_.back();
}

ArgumentParser::FlagArgs& ArgumentParser::ArgParser::AddFlag(char chr, const std::string& flag_name, const std::string& description_) {
    FlagArgs new_param;
    new_param.name_ = flag_name;
    new_param.short_flag_ = chr;
    new_param.description_ = ('-');
    new_param.description_ += chr;
    new_param.description_ += (",  --" + flag_name + ",  " + description_);
    bool_values_.push_back(new_param);

    return bool_values_.back();
}

ArgumentParser::FlagArgs& ArgumentParser::FlagArgs::Default(bool default_value) {
    this->value_ = default_value;
    this->description_ += " [default = ";
    if (default_value) {
        this->description_ += "true]";
    } else {
        this->description_ += "false]";
    }

    return *this;
}

ArgumentParser::FlagArgs& ArgumentParser::FlagArgs::StoreValue(bool& store_value) {
    this->link_ = &store_value;

    return *this;
}

std::string ArgumentParser::ArgParser::GetStringValue(const std::string& param) const {
    for (size_t i = 0; i < strings_.size(); i++) {
        if (strings_[i].name_ == param) {
            return strings_[i].value_;
        }
    }
}



bool ArgumentParser::ArgParser::GetFlag(const std::string& flag_name) const {
    for (size_t i = 0; i < bool_values_.size(); i++) {
        if (bool_values_[i].name_ == flag_name) {
            return bool_values_[i].value_;
        }
    }

    return false;
}

 //  help_
void ArgumentParser::ArgParser::AddHelp(char chr, const std::string &help_name, const std::string &description_) {
    this->help_flag_ += chr;
    this->help_param_ += help_name;
    this->help_description_ += description_ + '\n';
}

bool ArgumentParser::ArgParser::Help() const {
    return help_;
}

std::string ArgumentParser::ArgParser::HelpDescription() const {
    std::string help_message = name_ + '\n' + help_description_ + '\n';

    for (size_t i = 0; i < int_values_.size(); i++) {
        help_message += int_values_[i].description_ + '\n';
    }
    for (size_t i = 0; i < strings_.size(); i++) {
        help_message += strings_[i].description_ + '\n';
    }
    for (size_t i = 0; i < bool_values_.size(); i++) {
        help_message += bool_values_[i].description_ + '\n';
    }
    help_message += '\n' + help_flag_ + ", " + help_param_ + " Display this help_ and exit\n";

    return help_message;
}