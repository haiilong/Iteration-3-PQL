#pragma once
#include <unordered_map>
#include <vector>

class FollowsStar {
private:

public:
    std::unordered_map<int, std::vector<int>> followsStarTable;
    std::unordered_map<int, std::vector<int>> followsStarReverseTable;

	std::vector<int> getFollowsStar(int s);
	std::vector<int> getFollowedByStar(int s);
	std::vector<std::pair<int, int>> getFollowsStarPairs();
	bool contains(std::vector<int> v, int i);
};

