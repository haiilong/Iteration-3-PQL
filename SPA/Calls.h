#pragma once
#include <unordered_map>
#include <vector>
#include "Utility.h"

class Calls {
private:
	std::unordered_map<int, std::vector<int>> callsTable;
	std::unordered_map<int, std::vector<int>> callsReverseTable;
	std::unordered_map<int, int> stmtCallsTable; // key: stmt number, value: proc index

public:
	int setCalls(int p1, int p2);
	int setStmtCallsProc(int s, int p);
	int getProcinCallStmt(int s);
	std::vector<int> getCalls(int p); // get all procs that call p
	std::vector<int> getCalledBy(int p); // get all procs that called by p
	std::vector<std::pair<int, int>> getCallsPairs();
	std::vector<int> getAllCalledProc();
};

