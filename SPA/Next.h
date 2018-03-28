#pragma once
#include <unordered_map>
#include <vector>
#include "Utility.h"

class Next {
private:
	std::unordered_map<int, std::vector<int>> nextTable;
	std::unordered_map<int, std::vector<int>> previousTable;

public:
	int setNext(int s1, int s2);
	std::vector<int> getNext(int s);
	std::vector<int> getPrevious(int s);
	std::vector<std::pair<int, int>> getNextPairs();
};

