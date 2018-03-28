#include "ConstTable.h"
using namespace std;

vector<int> ConstTable::getConstTable() {
	return constTable;
}

int ConstTable::insertConst(int c) {
	int index = -1;
	// check whether c is in the table:
	for (auto it = constTable.begin(); it != constTable.end(); ++it) {
		if (*it == c) {
			return index;
		}
	}
	constTable.push_back(c);
	return c;
}

int ConstTable::getSize() {
	return constTable.size();
}
