#include "Parent.h"
using namespace std;


int Parent::setParent(int s1, int s2) {
	int index = -1;
	unordered_map<int, vector<int>>::const_iterator it1 = parentTable.find(s1);
	unordered_map<int, int>::const_iterator it2 = childrenTable.find(s2);
	if (it2 == childrenTable.end()) {
		// stmt s2 are not in the table, insert it into children table
		index = s2;
		childrenTable[s2] = s1;
		// check whether s1 is in the parent table:
		if (it1 == parentTable.end()) {
			vector<int> child = { s2 };
			parentTable[s1] = child;
		} else {
			// update the children vector of s1
			parentTable[s1].push_back(s2);
		}
	}
	return index;
}

int Parent::getParent(int s) {
	int NO_RESULT = -1;
	unordered_map<int, int>::const_iterator it = childrenTable.find(s);
	if (it == childrenTable.end()) {
		// s does not have a parent
		return NO_RESULT;
	} else {
		return it->second;
	}
}

vector<int> Parent::getChildren(int s) {
	vector<int> NO_RESULT = {};
	unordered_map<int, vector<int>>::const_iterator it = parentTable.find(s);
	if (it == parentTable.end()) {
		// s does not have any child
		return NO_RESULT;
	}
	else {
		return it->second;
	}
}

vector<pair<int, int>> Parent::getParentPairs() {
	vector<pair<int, int>> result = {};
	for (auto it = childrenTable.begin(); it != childrenTable.end(); ++it) {
		result.push_back(make_pair(it->second, it->first));
	}
	return result;
}
