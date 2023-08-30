#pragma once

#include <exception>

class DBException: public std::exception {
private:
    std::string error;
public:
    DBException(const std::string& error) {
        this->error = error;
    }

    const char* what() const noexcept override {
        return error.c_str();
    }
};
