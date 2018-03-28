#pragma once
#include <unordered_map>
#include <vector>

class Follows {
private:
	std::unordered_map<int, int> followsTable;
	std::unordered_map<int, int> followsReverseTable;

public:
	int setFollows(int s1, int s2);
	int getFollows(int s);
	int getFollowedBy(int s);
	std::vector<std::pair<int, int>> getFollowsPairs();

};

