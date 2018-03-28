#pragma once

#include <string>
#include <vector>
using namespace std;


class StringUtility {
public:
    static vector<string> split(const string& s, string delimiter);
	static vector<string> splitAndKeepDelimiters(const string& s, string delimiters);
    static vector<string> splitAndKeepDelimitersSeparately(const string &s, string delimiters);
    static vector<string> breakOn(const string& s, string delimiters);
    static string strip(string String, string delimiters);
	static string trim(const string& str);

    /** tokenize a string s, with knownTokens to break a string into tokens
    / @param string s the string to break up
    / @param string knownTokens a string where each char represents a known token
    / @return a vector<string> representing the tokens
    /* e.g. 
    /* tokenize("varA =varB+varC;", "{};+-=*") 
    /* -> returns {"varA", "=", "varB", "+", "varC", ";"}
    **/
    static vector<string> tokenize(const string& s, string knownTokens);
};

