#pragma once
#include <string>
#include <vector>
#include <utility>
#include <unordered_map>

#include "Utility.h"

class StmtTable {
private:
	/**
	stmtTable is a vector of pairs, with stmt number as the first, and stmt type as 
	the second
	stmt type refers to: ASSIGN, IF, WHILE, CALL
	*/
	std::vector<std::pair<int, std::string>> stmtTable;
	std::unordered_map<int, int> controlVarTable; // key container stmt, value: v index
	std::unordered_map<int, std::vector<int>> controlVarReverseTable; // key v index, value: container stmt list
public:
	std::vector<std::pair<int, std::string>> getStmtTable();
	int setControlVar(int s, int v);
	int insertStmt(int, std::string);
	int getSize();
	std::string getStmtType(int n);

	std::vector<int> getStmtwithControlVar(int v);
	std::vector<std::pair<int, int>> getStmtControlVarPairs();
};

