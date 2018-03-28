#pragma once

#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <regex>
using namespace std;

#include "SourceToken.h"

typedef std::list<SourceToken *> TokenList;
typedef std::vector<SourceToken *> TokenVector;
typedef std::list<SourceToken *>::iterator SourceTokenIterator;

class ExpressionUtility {
public:
    static bool verify(std::string expression);
    static bool verify(TokenVector tokens);
    static std::string toPostFix(TokenList tokens);
    static std::string toPostFix(std::string expression);

    static bool isMatched(string pattern, string text);


private:
    static int prec(string s);
    static bool isOperand(string s);

	static bool isTerm(TokenVector & tokens);
	static bool isFactor(TokenVector & tokens);
	static bool isVarName(string s);
	static bool isConst(string s);

};

