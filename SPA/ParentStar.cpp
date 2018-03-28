#include "ParentStar.h"
using namespace std;

vector<int> ParentStar::getParentStar(int s) {
	vector<int> NO_RESULT = {};
	unordered_map<int, vector<int>>::const_iterator it = childrenStarTable.find(s);
	if (it == childrenStarTable.end()) {
		// s does not have any parent
		return NO_RESULT;
	} else {
		return it->second;
	}
}

vector<int> ParentStar::getChildrenStar(int s) {
	vector<int> NO_RESULT = {};
	unordered_map<int, vector<int>>::const_iterator it = parentStarTable.find(s);
	if (it == parentStarTable.end()) {
		// s does not have any child
		return NO_RESULT;
	}
	else {
		return it->second;
	}
}

vector<pair<int, int>> ParentStar::getParentStarPairs() {
	vector<pair<int, int>> result = {};
	for (auto it1 = parentStarTable.begin(); it1 != parentStarTable.end(); ++it1) {
		for (auto it2 = it1->second.begin(); it2 != it1->second.end(); ++it2) {
			result.push_back(make_pair(it1->first, *it2));
		}
	}
	return result;
}

// check whether the input vector contains int i
bool ParentStar::contains(vector<int> v, int i) {
	for (auto it = v.begin(); it != v.end(); ++it) {
		if (*it == i) {
			return true;
		}
	}
	return false;
}
