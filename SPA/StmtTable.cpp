#include "StmtTable.h"
using namespace std;

struct InvalidReferenceException : public exception {
	const char * what() const throw () {
		return "Invalid reference index.";
	}
};

vector<pair<int, string>> StmtTable::getStmtTable() {
	return stmtTable;
}

int StmtTable::insertStmt(int n, string s) {
	int index = -1;
	pair<int, string> stmtPair = make_pair(n, s);

	// check whether stmt n is already in the stmtTable
	for (int i = 0; i < stmtTable.size(); i++) {
		if (stmtTable[i].first == n) {
			return index;
		}
	}
	// insert stmt pair in the table
	stmtTable.push_back(stmtPair);
	index = n;
	return index;
}

int StmtTable::setControlVar(int s, int v) {
	int index = -1;
	unordered_map<int, int>::const_iterator it = controlVarTable.find(s);
	unordered_map<int, vector<int>>::const_iterator it2 = controlVarReverseTable.find(v);
	if (it == controlVarTable.end()) {
		index = s;
		controlVarTable[s] = v;
	}
	if (it2 == controlVarReverseTable.end()) {
		vector<int> containerLst = { s };
		controlVarReverseTable[v] = containerLst;
	} else {
		if (!Utility::contains(it2->second, s)) {
			controlVarReverseTable[v].push_back(s);
		}
	}
	return index;
}

int StmtTable::getSize() {
	return stmtTable.size();
}

string StmtTable::getStmtType(int n) {
	// check whether stmt n is in the stmtTable
	for (unsigned i = 0; i < stmtTable.size(); i++) {
		if (stmtTable[i].first == n) {
			return stmtTable[i].second;
		}
	}
	throw InvalidReferenceException();
}

vector<int> StmtTable::getStmtwithControlVar(int v) {
	vector<int> NO_RESULT = {};
	unordered_map<int, vector<int>>::const_iterator it = controlVarReverseTable.find(v);
	if (it != controlVarReverseTable.end()) {
		return it->second;
	} else {
		return NO_RESULT;
	}
}

vector<pair<int, int>> StmtTable::getStmtControlVarPairs() {
	vector<pair<int, int>> result = {};
	for (auto it = controlVarTable.begin(); it != controlVarTable.end(); ++it) {
		result.push_back(make_pair(it->first, it->second));
	}
	return result;
}

