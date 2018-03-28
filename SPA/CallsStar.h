#pragma once
#include <unordered_map>
#include <vector>

class CallsStar {
public:
	// the tables are populated by design extractor
	std::unordered_map<int, std::vector<int>> callsStarTable;
	std::unordered_map<int, std::vector<int>> callsStarReverseTable;

	std::vector<int> getCallsStar(int p); // get all procs that *call p 
	std::vector<int> getCalledByStar(int p); // get all procs that *called by p
	std::vector<std::pair<int, int>> getCallsStarPairs();
};

