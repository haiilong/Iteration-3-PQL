#pragma once
#include <unordered_map>
#include <vector>

class ParentStar {
	/**
	Populating parent* table will be done by Design Extractor
	*/
private:

public:
	std::unordered_map<int, std::vector<int>> parentStarTable;
	std::unordered_map<int, std::vector<int>> childrenStarTable;

	std::vector<int> getParentStar(int s);
	std::vector<int> getChildrenStar(int s);
	std::vector<std::pair<int, int>> getParentStarPairs();
	bool contains(std::vector<int> v, int i);
};

