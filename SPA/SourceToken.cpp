#include "SourceToken.h"
#include <regex>


SourceToken::SourceToken(RawToken rawToken) {
    this->rawToken = rawToken;
    std::regex var_name("^[a-zA-Z][0-9a-zA-Z]*$");
    std::regex constant("[0-9]+");

    if(rawToken == "procedure") {
        type = PROCEDURE_TOKEN;
    }
    else if(rawToken == "if") {
        type = IF_TOKEN;
    }
    else if(rawToken == "then") {
        type = THEN_TOKEN;
    }
    else if(rawToken == "call") {
        type = CALL_TOKEN;
    }
    else if(rawToken == "else") {
        type = ELSE_TOKEN;
    }
    else if(rawToken == "while") {
        type = WHILE_TOKEN;
    }
    else if(rawToken == "(") {
        type = OPEN_PARENTHESIS;
    }
    else if(rawToken == ")") {
        type = CLOSE_PARENTHESIS;
    }
    else if(rawToken == "{") {
        type = OPEN_BRACE_TOKEN;
    }
    else if(rawToken == "}") {
        type = CLOSE_BRACE_TOKEN;
    }
    else if(rawToken == "+" || rawToken == "-") {
        type = ADD_SUB_TOKEN;
    }
    else if(rawToken == "*") {
        type = MULTIPLY_TOKEN;
    }
    else if(rawToken == "=") {
        type = EQUAL_TOKEN;
    }
    else if(rawToken == ";") {
        type = SEMI_COLON_TOKEN;
    }
    else if(std::regex_match(rawToken, var_name)) {
        type = VARIABLE_TOKEN;
    }
    else if(std::regex_match(rawToken, constant)) {
        type = CONSTANT_TOKEN;
    }
    else {
        type = UNKNOWN_TOKEN;
    }
}

std::string SourceToken::getTextForEnum(TokenType t) {
    switch(t) {
        case PROCEDURE_TOKEN:
            return "procedure";
        case OPEN_PARENTHESIS:
            return "(";
        case CLOSE_PARENTHESIS:
            return ")";
        case OPEN_BRACE_TOKEN:
            return "{";
        case CLOSE_BRACE_TOKEN:
            return "}";
        case VARIABLE_TOKEN:
            return "variable";
        case CONSTANT_TOKEN:
            return "constant";
        case ADD_SUB_TOKEN:
            return "+ or -";
        case MULTIPLY_TOKEN:
            return "*";
        case EQUAL_TOKEN:
            return "=";
        case SEMI_COLON_TOKEN:
            return ";";
        case IF_TOKEN:
            return "if";
        case THEN_TOKEN:
            return "then";
        case CALL_TOKEN:
            return "call";
        case ELSE_TOKEN:
            return "else";
        case WHILE_TOKEN:
            return "while";
        default:
            return "unknown";
    }
}

SourceToken::~SourceToken() {
}

TokenType SourceToken::getType() {
    return type;
}

RawToken SourceToken::getRawToken() {
    return rawToken;
}