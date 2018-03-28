#include "ProcTable.h"
using namespace std;

struct InvalidReferenceException : public exception {
	const char * what() const throw () {
		return "Invalid reference index.";
	}
};

int ProcTable::insertProc(string name) {
	int index = -1;
	unordered_map<string, int>::const_iterator iterator = procTableIndex.find(name);
	if (iterator == procTableIndex.end()) {
		// procName is not in the table, inserts it into table and returns its index
		index = procTable.size() + 1;
		procTable[index] = name;
		procTableIndex[name] = index;
	} else {
		// procName is in the table, returns its index
		index = iterator->second; 
	}
	return index;
}

int ProcTable::insertStmtToProc(int p, int s) {
	unordered_map<int, vector<int>>::const_iterator it = procStmtTable.find(p);
	if (it == procStmtTable.end()) {
		vector<int> v = { s };
		procStmtTable[p] = v;
	} else {
		if (!Utility::contains(it->second, s)) {
			procStmtTable[p].push_back(s);
		}
	}
	return s;
}

int ProcTable::getSize() {
	return procTable.size();
}

string ProcTable::getProcName(int id) {
	int index = procTable.size();
	if (id > index || id < 1) {
		throw InvalidReferenceException();
	}
	else {
		return procTable[id];
	}
}

int ProcTable::getProcIndex(string name) {
	int index = -1;
	unordered_map<string, int>::const_iterator iterator = procTableIndex.find(name);
	if (iterator != procTableIndex.end()) {
		// procName is in the table
		index = procTableIndex[name];
	}
	return index;
}
