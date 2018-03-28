#include "Next.h"
using namespace std;

int Next::setNext(int s1, int s2) {
	unordered_map<int, vector<int>>::const_iterator it1 = nextTable.find(s1);
	unordered_map<int, vector<int>>::const_iterator it2 = previousTable.find(s2);
	if (it1 == nextTable.end()) {
		vector<int> v = { s2 };
		nextTable[s1] = v;
	} else {
		if (!Utility::contains(it1->second, s2)) {
			nextTable[s1].push_back(s2);
		}
	}

	if (it2 == previousTable.end()) {
		vector<int> v = { s1 };
		previousTable[s2] = v;
	} else {
		if (!Utility::contains(it2->second, s1)) {
			previousTable[s2].push_back(s1);
		}
	}
	return s1;
}

std::vector<int> Next::getNext(int s) {
	vector<int> NO_RESULT = {};
	unordered_map<int, vector<int>>::const_iterator it = nextTable.find(s);
	if (it == nextTable.end()) {
		return NO_RESULT;
	}
	else {
		return it->second;
	}
}

std::vector<int> Next::getPrevious(int s) {
	vector<int> NO_RESULT = {};
	unordered_map<int, vector<int>>::const_iterator it = previousTable.find(s);
	if (it == previousTable.end()) {
		return NO_RESULT;
	}
	else {
		return it->second;
	}
}

std::vector<std::pair<int, int>> Next::getNextPairs() {
	vector<pair<int, int>> result = {};
	for (auto it = nextTable.begin(); it != nextTable.end(); ++it) {
		for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
			result.push_back(make_pair(it->first, *it2));
		}
	}
	return result;
}
