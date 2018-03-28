#include "Uses.h"
using namespace std;


int Uses::setUses(int s1, int vindex) {
	unordered_map<int, vector<int>>::const_iterator it1 = usesTable.find(s1);
	unordered_map<int, vector<int>>::const_iterator it2 = usedByTable.find(vindex);

	if (it1 == usesTable.end()) {
		vector<int> v = { vindex };
		usesTable[s1] = v;
	}
	else {
		vector<int> v = usesTable[s1];
		if (!Utility::contains(v, vindex)) {
			usesTable[s1].push_back(vindex);
		}
	}

	if (it2 == usedByTable.end()) {
		vector<int> v = { s1 };
		usedByTable[vindex] = v;
	}
	else {
		vector<int> v = usedByTable[vindex];
		if (!Utility::contains(v, s1)) {
			usedByTable[vindex].push_back(s1);
		}
	}
	return s1;
}

vector<int> Uses::getUsing(int vindex) {
	vector<int> NO_RESULT = {};
	unordered_map<int, vector<int>>::const_iterator it = usedByTable.find(vindex);
	if (it != usedByTable.end()) {
		return it->second;
	}
	else {
		return NO_RESULT;
	}
}

vector<int> Uses::getUsedBy(int s) {
	vector<int> NO_RESULT = {};
	unordered_map<int, vector<int>>::const_iterator it = usesTable.find(s);
	if (it != usesTable.end()) {
		return it->second;
	}
	else {
		return NO_RESULT;
	}
}

vector<pair<int, int>> Uses::getUsesPairs() {
	vector<pair<int, int>> result = {};
	for (auto it = usesTable.begin(); it != usesTable.end(); ++it) {
		for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
			pair<int, int> p = make_pair(it->first, *it2);
			result.push_back(p);
		}
	}
	return result;
}

int Uses::setProcUses(int p, int v) {
	unordered_map<int, vector<int>>::const_iterator it1 = procUsesTable.find(p);
	unordered_map<int, vector<int>>::const_iterator it2 = usedByProcTable.find(v);

	if (it1 == procUsesTable.end()) {
		vector<int> vList = { v };
		procUsesTable[p] = vList;
	}
	else {
		if (!Utility::contains(procUsesTable[p], v)) {
			procUsesTable[p].push_back(v);
		}
	}

	if (it2 == usedByProcTable.end()) {
		vector<int> pList = { p };
		usedByProcTable[v] = pList;
	}
	else {
		if (!Utility::contains(usedByProcTable[v], p)) {
			usedByProcTable[v].push_back(p);
		}
	}
	return p;
}

vector<int> Uses::getProcUsing(int v) {
	vector<int> NO_RESULT = {};
	unordered_map<int, vector<int>>::const_iterator it = usedByProcTable.find(v);
	if (it != usedByProcTable.end()) {
		return it->second;
	}
	else {
		return NO_RESULT;
	}
}

vector<int> Uses::getUsedByProc(int p) {
	vector<int> NO_RESULT = {};
	unordered_map<int, vector<int>>::const_iterator it = procUsesTable.find(p);
	if (it != procUsesTable.end()) {
		return it->second;
	}
	else {
		return NO_RESULT;
	}
}

vector<pair<int, int>> Uses::getProcUsesPairs() {
	vector<pair<int, int>> result = {};
	for (auto it = procUsesTable.begin(); it != procUsesTable.end(); ++it) {
		for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
			pair<int, int> p = make_pair(it->first, *it2);
			result.push_back(p);
		}
	}
	return result;
}

