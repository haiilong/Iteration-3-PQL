#include "FollowsStar.h"
using namespace std;

vector<int> FollowsStar::getFollowsStar(int s) {
	vector<int> NO_RESULT = {};
	unordered_map<int, vector<int>>::const_iterator it = followsStarTable.find(s);
	if (it != followsStarTable.end()) {
		// stmt s is in the table
		return followsStarTable[s];
	}
	return NO_RESULT;
}

vector<int> FollowsStar::getFollowedByStar(int s) {
	vector<int> NO_RESULT = {};
	unordered_map<int, vector<int>>::const_iterator it = followsStarReverseTable.find(s);
	if (it != followsStarReverseTable.end()) {
		// stmt s is in the table
		return followsStarReverseTable[s];
	}
	return NO_RESULT;
}

std::vector<pair<int, int>> FollowsStar::getFollowsStarPairs() {
	vector<pair<int, int>> result = {};
	for (auto it1 = followsStarTable.begin(); it1 != followsStarTable.end(); ++it1) {
		for (auto it2 = it1->second.begin(); it2 != it1->second.end(); ++it2) {
			result.push_back(make_pair(it1->first, *it2));
		}
	}
    return result;
}

// check whether the input vector contains int i
bool FollowsStar::contains(vector<int> v, int i) {
	for (auto it = v.begin(); it != v.end(); ++it) {
		if (*it == i) {
			return true;
		}
	}
	return false;
}
