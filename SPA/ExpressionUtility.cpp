#include "ExpressionUtility.h"
#include "StringUtility.h"

bool ExpressionUtility::verify(std::string expression) {
    TokenVector sourceTokens;
    std::vector<RawToken> tokens = StringUtility::tokenize(expression, "{}()=+-*;");
    for(RawToken token : tokens) {
        sourceTokens.push_back(new SourceToken(token));
    }
    return verify(sourceTokens);
}

bool ExpressionUtility::verify(TokenVector tokens) {
	if (isTerm(tokens)) {
		return true;
	}
	else {
		int i;
		TokenVector newVec;
		stack<string> bracketsStack;
		for (i = tokens.size() - 1; i >= 0; i--) {
			if (tokens[i]->getRawToken() == ")") {
				bracketsStack.push(")");
			}
			else if (!bracketsStack.empty() && tokens[i]->getRawToken() == "(") {
				bracketsStack.pop();
			}
			if ((bracketsStack.empty() && tokens[i]->getRawToken() == "+") || (bracketsStack.empty() && tokens[i]->getRawToken() == "-")) {
				break;
			}
			else {
				newVec.insert(newVec.begin(), tokens[i]);
			}
		}
		if (newVec.size() == 0 || !isTerm(newVec)) {
			return false;
		}
		TokenVector newVec2;
		for (size_t j = 0; j < i; j++) {
			newVec2.push_back(tokens[j]);
		}
		if (newVec2.size() != 0 && verify(newVec2)) {
			return true;
		}
	}
	return false;
}

bool ExpressionUtility::isTerm(TokenVector &tokens) {
    if(tokens.size() == 0) return false;
	if (isFactor(tokens)) {
		return true;
	}
	else {
		size_t i;
		TokenVector newVec;
		stack<string> bracketsStack;
		for (i = tokens.size() - 1; i > 0; i--) {
			if (tokens[i]->getRawToken() == ")") {
				bracketsStack.push(")");
			}
			else if (!bracketsStack.empty() && tokens[i]->getRawToken() == "(") {
				bracketsStack.pop();
			}
			if (bracketsStack.empty() && tokens[i]->getRawToken() == "*") {
				break;
			}
			else {
				newVec.insert(newVec.begin(), tokens[i]);
			}
		}
		if (newVec.size() == 0 || !isFactor(newVec)) {
			return false;
		}
		TokenVector newVec2;
		for (size_t j = 0; j < i; j++) {
			newVec2.push_back(tokens[j]);
		}
		if (newVec2.size() != 0 && isTerm(newVec2)) {
			return true;
		}
	}
	return false;
}
bool ExpressionUtility::isFactor(TokenVector &tokens) {
    if(tokens.size() == 0) return false;
	if ((tokens.size() == 1 && isVarName(tokens[0]->getRawToken())) || (tokens.size() == 1 && isConst(tokens[0]->getRawToken()))) {
		return true;
	}
	else { 		//check if '(' expr ')'
		TokenVector newVec;
		if (tokens[0]->getRawToken() == "(" && tokens[tokens.size() - 1]->getRawToken() == ")") {
			for (size_t i = 1; i < tokens.size() - 1; i++) {
				newVec.push_back(tokens[i]);
			}
			if (verify(newVec)) {
				return true;
			}
		}
	}
	return false;
}


std::string ExpressionUtility::toPostFix(string expression) {
    std::vector<RawToken> tokens = StringUtility::tokenize(expression, "{}()=+-*;");
    stack<string> st;
    string ret = "";
    // set operator precedence

    // process each token one by one
    for(string token : tokens) {
        // if token is an operand, append it to the return string
        if(isOperand(token)) {
            ret.append(token);
            ret.append(" ");
        }
        // if token is a left parenthesis, push it to stack
        else if(token == "(") {
            st.push(token);
        }
        // if the token is a right parenthesis, pop the stack until the left parenthesis is removed
        // append each of them to return string
        else if(token == ")") {
            string topToken = st.top();
            st.pop();
            while(topToken != "(") {
                ret.append(topToken);
                ret.append(" ");
                topToken = st.top();
                st.pop();
            }
        }
        // if the token is an operand (+, -, *), pop operators on top of stack that have higher or equal precedence than token
        // and append them to return string. Then push to the stack
        else {
            while(!st.empty() && prec(st.top()) >= prec(token)) {
                ret.append(st.top());
                ret.append(" ");
                st.pop();
            }
            st.push(token);
        }
    }

    // push the rest of the stack on return string
    while(!st.empty()) {
        ret.append(st.top());
        ret.append(" ");
        st.pop();
    }
    //erase the last whitespace
    ret.pop_back();

    return " " + ret + " ";
}

// whether the text match the pattern
bool ExpressionUtility::isMatched(string pattern, string text) {
    string patternPostFix = toPostFix(pattern);
    string textPostFix = toPostFix(text);

    return patternPostFix.find(textPostFix) != std::string::npos;
}

// is factor/operand
bool ExpressionUtility::isOperand(string s) {
    regex varName("^[a-zA-Z][a-zA-Z0-9]*$");
    regex constantValue("[0-9]+");
    return regex_match(s, varName) || regex_match(s, constantValue);
}

// operator precedence
int ExpressionUtility::prec(string s) {
    if(s == "+" || s == "-") {
        return 1;
    }
    else if(s == "*") {
        return 2;
    }
    else {
        return -1;
    }
}

std::string ExpressionUtility::toPostFix(TokenList tokens) {
    string expression = "";
    for(SourceToken *token : tokens) {
        expression += (token->getRawToken());
    }
    return toPostFix(expression);
}

bool ExpressionUtility::isVarName(string s) {
	regex var_name("^[a-zA-Z][0-9a-zA-Z]*$");
	//if valid variable name
	if (regex_match(s, var_name)) {
		return true;
	}
	return false;
}

bool ExpressionUtility::isConst(string s) {
	regex constant("[0-9]+");
	//if valid constant
	if (regex_match(s, constant)) {
		return true;
	}
	return false;
}

