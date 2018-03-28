#pragma once
#include <string>
#include <unordered_map>
#include "Utility.h"

class ProcTable {
private:
	std::unordered_map<int, std::string> procTable;
	std::unordered_map<std::string, int> procTableIndex;
	std::unordered_map<int, std::vector<int>> procStmtTable;

public:
	int insertProc(std::string name);
	int insertStmtToProc(int p, int s);
	int getSize();
	std::string getProcName(int id);
	int getProcIndex(std::string name);
};

