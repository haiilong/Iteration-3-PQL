#include "Calls.h"
using namespace std;

int Calls::setCalls(int p1, int p2) {
	unordered_map<int, vector<int>>::const_iterator it1 = callsTable.find(p1);
	unordered_map<int, vector<int>>::const_iterator it2 = callsReverseTable.find(p2);
	if (it1 == callsTable.end()) {
		vector<int> v = { p2 };
		callsTable[p1] = v;
	} else {
		if (!Utility::contains(it1->second, p2)) {
			callsTable[p1].push_back(p2);
		}
	}

	if (it2 == callsReverseTable.end()) {
		vector<int> v = { p1 };
		callsReverseTable[p2] = v;
	} else {
		if (!Utility::contains(it2->second, p1)) {
			callsReverseTable[p2].push_back(p1);
		}
	}
	return p1;
}

int Calls::setStmtCallsProc(int s, int p) {
	int index = -1;
	unordered_map<int, int>::const_iterator it = stmtCallsTable.find(s);
	if (it == stmtCallsTable.end()) {
		stmtCallsTable[s] = p;
	}
	return index;
}

int Calls::getProcinCallStmt(int s) {
	int NO_RESULT = -1;
	unordered_map<int, int>::const_iterator it = stmtCallsTable.find(s);
	if (it != stmtCallsTable.end()) {
		return it->second;
	} else {
		return NO_RESULT;
	}
}

// get all procs that call s
vector<int> Calls::getCalls(int p) {
	vector<int> NO_RESULT = {};
	unordered_map<int, vector<int>>::const_iterator it = callsReverseTable.find(p);
	if (it == callsReverseTable.end()) {
		return NO_RESULT;
	} else {
		return it->second;
	}
}

// get all procs that called by s
vector<int> Calls::getCalledBy(int p) {
	vector<int> NO_RESULT = {};
	unordered_map<int, vector<int>>::const_iterator it = callsTable.find(p);
	if (it == callsTable.end()) {
		return NO_RESULT;
	} else {
		return it->second;
	}
}

vector<pair<int, int>> Calls::getCallsPairs() {
	vector<pair<int, int>> result = {};
	for (auto it = callsTable.begin(); it != callsTable.end(); ++it) {
		for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
			result.push_back(make_pair(it->first, *it2));
		}
	}
	return result;
}

vector<int> Calls::getAllCalledProc() {
	vector<int> result = {};
	for (auto it = callsReverseTable.begin(); it != callsReverseTable.end(); ++it) {
		result.push_back(it->first);
	}
	return result;
}
