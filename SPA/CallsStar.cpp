#include "CallsStar.h"
using namespace std;

// get all procs that *call p 
std::vector<int> CallsStar::getCallsStar(int p) {
	vector<int> NO_RESULT = {};
	unordered_map<int, vector<int>>::const_iterator it = callsStarReverseTable.find(p);
	if (it == callsStarReverseTable.end()) {
		return NO_RESULT;
	} else {
		return it->second;
	}
}

// get all procs that *called by p
std::vector<int> CallsStar::getCalledByStar(int p) {
	vector<int> NO_RESULT = {};
	unordered_map<int, vector<int>>::const_iterator it = callsStarTable.find(p);
	if (it == callsStarTable.end()) {
		return NO_RESULT;
	} else {
		return it->second;
	}
}

std::vector<std::pair<int, int>> CallsStar::getCallsStarPairs() {
	vector<pair<int, int>> result = {};
	for (auto it = callsStarTable.begin(); it != callsStarTable.end(); ++it) {
		for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
			result.push_back(make_pair(it->first, *it2));
		}
	}
	return result;
}
