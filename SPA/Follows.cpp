#include "Follows.h"
using namespace std;

int Follows::setFollows(int s1, int s2) {
	int index = -1;
	unordered_map<int, int>::const_iterator it1 = followsTable.find(s1);
	unordered_map<int, int>::const_iterator it2 = followsReverseTable.find(s2);
	if (it1 == followsTable.end() && it2 == followsReverseTable.end()) {
		// stmts are not in the table, insert them into table and returns s1
		index = s1;
		followsTable[s1] = s2;
		followsReverseTable[s2] = s1;
	}
	return index;
}

int Follows::getFollows(int s) {
	int NO_RESULT = -1;
	unordered_map<int, int>::const_iterator it = followsTable.find(s);
	if (it == followsTable.end()) {
		// there's no stmt which is following s
		return NO_RESULT;
	} else {
		return followsTable[s];
	}
}

int Follows::getFollowedBy(int s) {
	int NO_RESULT = -1;
	unordered_map<int, int>::const_iterator it = followsReverseTable.find(s);
	if (it == followsReverseTable.end()) {
		// there's no stmt which is followed by s
		return NO_RESULT;
	}
	else {
		return followsReverseTable[s];
	}
}

vector<pair<int, int>> Follows::getFollowsPairs() {
	vector<pair<int, int>> followsPairs = {};
	for (auto it = followsTable.begin(); it != followsTable.end(); ++it) {
		pair<int, int> p = make_pair(it->first, it->second);
		followsPairs.push_back(p);
	}
	return followsPairs;
}
