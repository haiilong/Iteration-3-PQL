#include "patternTable.h"
using namespace std;

struct InvalidReferenceException : public exception {
	const char * what() const throw () {
		return "Invalid reference index.";
	}
};

int PatternTable::insertPattern(int stmtNumber, string expression) {
	int index = -1;
	unordered_map<int, string>::const_iterator iterator = patterntable.find(stmtNumber);
	if (iterator == patterntable.end()) {
		// the stmt is not in the table, inserts it into table and returns its index
		index = stmtNumber;
		patterntable[index] = expression;
	}
	return index;
}

string PatternTable::getExpression(int stmtNumber) {
	int index = -1;
	unordered_map<int, string>::const_iterator iterator = patterntable.find(stmtNumber);
	if (iterator == patterntable.end()) {
		// the stmt is not in the table
		throw InvalidReferenceException();
	} else {
		return patterntable[stmtNumber];
	}
	
}
