#include "VarTable.h"
using namespace std;

struct InvalidReferenceException : public exception {
	const char * what() const throw () {
		return "Invalid reference index.";
	}
};

int VarTable::insertVar(string varName){
	int index = -1;
	unordered_map<string, int>::const_iterator iterator = varTableIndex.find(varName);
	if (iterator == varTableIndex.end()) {
		// varName is not in the table, inserts it into table and returns its index
		index = varTable.size() + 1;
		varTable[index] = varName;
		varTableIndex[varName] = index;
		return index;
	} else {
		// var is in the table, return the var index
		index = iterator->second;
	}
	return index;
}

int VarTable::getSize() {
	return varTable.size();
}

string VarTable::getVarName(int id) {
	int index = varTable.size();
	if (id > index || id < 1) {
		throw InvalidReferenceException();
	} else {
		return varTable[id];
	}
}

int VarTable::getVarIndex(string varName) {
	int index = -1;
	unordered_map<string, int>::const_iterator iterator = varTableIndex.find(varName);
	if (iterator != varTableIndex.end()) {
		// varName is in the table
		index = varTableIndex[varName];
	}
	return index;
}
