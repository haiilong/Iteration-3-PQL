#pragma once

#include <exception>
#include <string>

struct EmptyStatementListException : public std::exception {
private:
    std::string message;
public:
    EmptyStatementListException(const std::string& message);
    virtual const char* what() const throw() {
        return message.c_str();
    }
};