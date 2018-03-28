#pragma once
#include <unordered_map>
#include <vector>

class Parent {
private:
	std::unordered_map<int, std::vector<int>> parentTable;
	std::unordered_map<int, int> childrenTable;

public:
	int setParent(int s1, int s2);
	int getParent(int s);
	std::vector<int> getChildren(int s);
	std::vector<std::pair<int, int>> getParentPairs();
};

