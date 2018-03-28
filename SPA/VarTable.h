#pragma once
#include <string>
#include <unordered_map>

class VarTable {
private:
	std::unordered_map<int, std::string> varTable;
	std::unordered_map<std::string, int> varTableIndex;

public:
	int insertVar(std::string varName);
	int getSize();
	std::string getVarName(int id);
	int getVarIndex(std::string varName);
};

