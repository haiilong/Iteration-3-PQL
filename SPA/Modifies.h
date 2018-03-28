#pragma once
#include <unordered_map>
#include <vector>
#include "Utility.h"

class Modifies {
private:
	std::unordered_map<int, std::vector<int>> modifiesTable; // key is stmt number
	std::unordered_map<int, std::vector<int>> modifiedByTable; // key is var index

	std::unordered_map<int, std::vector<int>> procModifiesTable; // key is proc index
	std::unordered_map<int, std::vector<int>> modifiedByProcTable; // key is var index

public:
	// stmt modifies
	int setModifies(int s1, int vindex);
	std::vector<int> getModifying(int vindex);
	std::vector<int> getModifiedBy(int s);
	std::vector<std::pair<int, int>> getModifiesPairs();

	// proc modifies
	int setProcModifies(int p, int v);
	std::vector<int> getProcModifying(int v);
	std::vector<int> getModifiedByProc(int p);
	std::vector<std::pair<int, int>> getProcModifiesPairs();
};

