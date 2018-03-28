#pragma once

#include <exception>
#include <string>

struct BadTokenException : public std::exception {
private:
    std::string message;
public:
    BadTokenException(const std::string& message);
    virtual const char* what() const throw() {
        return message.c_str();
    }
};