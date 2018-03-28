#pragma once

#include <string>
#include <list>

typedef std::string RawToken;

enum TokenType {
    PROCEDURE_TOKEN,
    OPEN_PARENTHESIS,
    CLOSE_PARENTHESIS,
    OPEN_BRACE_TOKEN,
    CLOSE_BRACE_TOKEN,
    VARIABLE_TOKEN,
    CONSTANT_TOKEN,
    ADD_SUB_TOKEN,
    MULTIPLY_TOKEN,
    EQUAL_TOKEN,
    SEMI_COLON_TOKEN,
    IF_TOKEN,
    THEN_TOKEN,
    CALL_TOKEN,
    ELSE_TOKEN,
    WHILE_TOKEN,
    UNKNOWN_TOKEN
};

class SourceToken {
private:
    RawToken rawToken;
    TokenType type;

public:
    SourceToken(RawToken rawToken);
    ~SourceToken();
    TokenType getType();
    RawToken getRawToken();
    static std::string getTextForEnum(TokenType t);
};

