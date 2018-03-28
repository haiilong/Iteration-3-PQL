#pragma once
#include <unordered_map>
#include <vector>

#include "Utility.h"

class Uses {
private:
	std::unordered_map<int, std::vector<int>> usesTable; // key is stmt number
	std::unordered_map<int, std::vector<int>> usedByTable; // key is var index

	std::unordered_map<int, std::vector<int>> procUsesTable; // key is proc index
	std::unordered_map<int, std::vector<int>> usedByProcTable; // key is var index

public:
	// stmt uses
	int setUses(int s1, int vindex);
	std::vector<int> getUsing(int vindex);
	std::vector<int> getUsedBy(int s);
	std::vector<std::pair<int, int>> getUsesPairs();

	// proc uses
	int setProcUses(int p, int v);
	std::vector<int> getProcUsing(int v);
	std::vector<int> getUsedByProc(int p);
	std::vector<std::pair<int, int>> getProcUsesPairs();
};

