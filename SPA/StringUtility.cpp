#include <algorithm>
#include <string>
#include <regex>

#include "StringUtility.h"

// File-local variable listing whitespace characters
static const std::string WHITESPACE = " \t\r\n\v\f";

// split a string by delimeter into vector<string>
vector<string> StringUtility::split(const string& s, string delimeter) {
	vector<string> tokens;
	size_t start = 0, end = 0;
	size_t delimeterLength = delimeter.length();
	while ((end = s.find(delimeter, start)) != string::npos) {
		tokens.push_back(trim(s.substr(start, end - start)));
		start = end + delimeterLength;
	}
	tokens.push_back(trim(s.substr(start)));
	return tokens;
}

// split a string by any character in delimiters and keep the delimiters (as the last element of each string)
vector<string> StringUtility::splitAndKeepDelimiters(const string & s, string delimiters) {
	// code snippet from https://stackoverflow.com/questions/17476224/c-splitting-a-string-with-multiple-delimiters-and-keep-the-delimiters-in-the
	vector<string> result;

    std::string current = "";
    for(char c : s) {
        current.push_back(c);
        if(delimiters.find(c) != std::string::npos) {
            std::string trimmed = trim(current);
            if(trimmed.length() > 0) {
                result.push_back(trimmed);
            }
            current = "";
        }
    }
    std::string trimmed = trim(current);
    if(trimmed.length() > 0) {
        result.push_back(trimmed);
    }

	return result;
}

vector<string> StringUtility::splitAndKeepDelimitersSeparately(const string & s, string delimiters) {
    return vector<string>();
}

// split a string by delimeter into vector<string>, but only once (first occurrence of delimeter)
vector<string> StringUtility::breakOn(const string& s, string delimeter) {
	vector<string> tokens;
	size_t delimeterLength = delimeter.length();
	size_t  end = s.find(delimeter);
	tokens.push_back(trim(s.substr(0, end)));
	size_t start = end + delimeterLength;

	tokens.push_back(trim(s.substr(start)));
	return tokens;
}

// strip a string by all delimeter
string StringUtility::strip(string s, string delimeter) {
	size_t end = 0;
	size_t delimeterLength = delimeter.length();
	while ((end = s.find(delimeter)) != string::npos) {
		s.erase(end, delimeterLength);
	}
	return s;
}

//trim a string(remove front and back whitespace)
// Modified: Johannes Choo: 12 Feb: handles \n and other whitespace,
//   refactor for clarity and mutation safety
//   c.f. https://stackoverflow.com/a/6500499
string StringUtility::trim(const string& str)
{
	std::string s{ str };
	s.erase(0, s.find_first_not_of(WHITESPACE));
	s.erase(s.find_last_not_of(WHITESPACE)+1);
	return s;
}

vector<string> StringUtility::tokenize(const string & s, string knownTokens) {
    std::vector<std::string> result;

    std::string current = "";
    for(char c : s) {
        if(isspace(c)) {
            if(current.length() > 0) {
                result.push_back(current);
            }
            current = "";
        } else if(knownTokens.find(c) != std::string::npos) {
            if(current.length() > 0) {
                result.push_back(current);
            }
            result.push_back(std::string(1, c));
            current = "";
        } else {
            current.push_back(c);
        }
    }
    if(current.length() > 0) {
        result.push_back(current);
    }
    
    return result;
}