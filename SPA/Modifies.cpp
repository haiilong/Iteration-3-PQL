#include "Modifies.h"
using namespace std;


int Modifies::setModifies(int s1, int vindex) {
	unordered_map<int, vector<int>>::const_iterator it1 = modifiesTable.find(s1);
	unordered_map<int, vector<int>>::const_iterator it2 = modifiedByTable.find(vindex);

	if (it1 == modifiesTable.end()) {
		vector<int> v = {vindex};
		modifiesTable[s1] = v;
	} else {
		vector<int> v = modifiesTable[s1];
		if (!Utility::contains(v, vindex)) {
			modifiesTable[s1].push_back(vindex);
		}	
	}

	if (it2 == modifiedByTable.end()) {
		vector<int> v = { s1 };
		modifiedByTable[vindex] = v;
	} else {
		vector<int> v = modifiedByTable[vindex];
		if (!Utility::contains(v, s1)) {
			modifiedByTable[vindex].push_back(s1);
		}
	}
	return s1;
}

vector<int> Modifies::getModifying(int vindex) {
	vector<int> NO_RESULT = {};
	unordered_map<int, vector<int>>::const_iterator it = modifiedByTable.find(vindex);
	if (it != modifiedByTable.end()) {
		return it->second;
	} else {
		return NO_RESULT;
	}
}

vector<int> Modifies::getModifiedBy(int s) {
	vector<int> NO_RESULT = {};
	unordered_map<int, vector<int>>::const_iterator it = modifiesTable.find(s);
	if (it != modifiesTable.end()) {
		return it->second;
	}
	else {
		return NO_RESULT;
	}
}

vector<pair<int, int>> Modifies::getModifiesPairs() {
	vector<pair<int, int>> result = {};
	for (auto it = modifiesTable.begin(); it != modifiesTable.end(); ++it) {
		for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
			pair<int, int> p = make_pair(it->first, *it2);
			result.push_back(p);
		}
	}
	return result;
}

int Modifies::setProcModifies(int p, int v) {
	unordered_map<int, vector<int>>::const_iterator it1 = procModifiesTable.find(p);
	unordered_map<int, vector<int>>::const_iterator it2 = modifiedByProcTable.find(v);

	if (it1 == procModifiesTable.end()) {
		vector<int> vList = { v };
		procModifiesTable[p] = vList;
	} else {
		if (!Utility::contains(procModifiesTable[p], v)) {
			procModifiesTable[p].push_back(v);
		}
	}

	if (it2 == modifiedByProcTable.end()) {
		vector<int> pList = { p };
		modifiedByProcTable[v] = pList;
	} else {
		if (!Utility::contains(modifiedByProcTable[v], p)) {
			modifiedByProcTable[v].push_back(p);
		}
	}
	return p;
}

vector<int> Modifies::getProcModifying(int v) {
	vector<int> NO_RESULT = {};
	unordered_map<int, vector<int>>::const_iterator it = modifiedByProcTable.find(v);
	if (it != modifiedByProcTable.end()) {
		return it->second;
	}
	else {
		return NO_RESULT;
	}
}

vector<int> Modifies::getModifiedByProc(int p) {
	vector<int> NO_RESULT = {};
	unordered_map<int, vector<int>>::const_iterator it = procModifiesTable.find(p);
	if (it != procModifiesTable.end()) {
		return it->second;
	}
	else {
		return NO_RESULT;
	}
}

vector<pair<int, int>> Modifies::getProcModifiesPairs() {
	vector<pair<int, int>> result = {};
	for (auto it = procModifiesTable.begin(); it != procModifiesTable.end(); ++it) {
		for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
			pair<int, int> p = make_pair(it->first, *it2);
			result.push_back(p);
		}
	}
	return result;
}



