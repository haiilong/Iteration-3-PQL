#pragma once
#include <string>
#include <unordered_map>

class PatternTable {
private:
	std::unordered_map<int, std::string> patterntable;

public:
	int insertPattern(int stmtNumber, std::string expression);
	std::string getExpression(int stmtNumber);
};

