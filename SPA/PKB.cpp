#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;


#include "PKB.h"

PKB* PKB::instance = NULL;

PKB* PKB::getPKBInstance() {
	if (instance == 0) {
		instance = new PKB();
	}
	return instance;
}

void PKB::clearPKB() {
	instance = NULL;
}

PKB::PKB() {
}

/**
public APIs for Parser:
*/
// set the synonyms
int PKB::insertVar(string varName) {
	int varIndex = varTable.insertVar(varName);
	return varIndex;
}

int PKB::insertStmt(int i, string type, int p) {
	int stmtIndex = stmtTable.insertStmt(i, type);
	procTable.insertStmtToProc(p, i);
	return stmtIndex;
}

int PKB::insertProc(string name) {
	int procIndex = procTable.insertProc(name);
	return procIndex;
}

int PKB::insertConst(int c) {
	return constTable.insertConst(c);
}

int PKB::setControlVariable(int s, int v) {
	return stmtTable.setControlVar(s, v);
}

int PKB::setFollows(int stmt1, int stmt2) {
	int index = -1;
	int id1 = followsTable.setFollows(stmt1, stmt2);
	if (id1 == -1) {
		return index;
	} else {
		return stmt1;
	}
}

int PKB::setParent(int stmt1, int stmt2) {
	int index = -1;
	int id1 = parentTable.setParent(stmt1, stmt2);
	if (id1 == -1) {
		return index;
	}
	else {
		return stmt1;
	}
}

int PKB::setModifies(int s, int vindex) {
	return modifiesTable.setModifies(s, vindex);
}

int PKB::setUses(int s, int vindex) {
	return usesTable.setUses(s, vindex);
}

int PKB::setProcModifies(int proc, int var) {
	return modifiesTable.setProcModifies(proc, var);
}

int PKB::setProcUses(int proc, int var) {
	return usesTable.setProcUses(proc, var);
}

int PKB::insertPatternExpression(int stmtNum, string expression) {
	int exprIndex = patternTable.insertPattern(stmtNum, expression);
	return exprIndex;
}

int PKB::setNext(int s1, int s2) {
	return nextTable.setNext(s1, s2);
}

int PKB::setCalls(int s, int p1, int p2) {
	callsTable.setStmtCallsProc(s, p2);
	return callsTable.setCalls(p1, p2);
}

/**
public APIs for DE:
*/
vector<pair<int, int>> PKB::getFollowsPairsList() {
	return followsTable.getFollowsPairs();
}

vector<pair<int, int>> PKB::getParentPairsList() {
	return parentTable.getParentPairs();
}

vector<pair<int, int>> PKB::getCallsPairsList() {
	return callsTable.getCallsPairs();
}

int PKB::getProcinCallStmt(int s) {
	return callsTable.getProcinCallStmt(s);
}

/**
public APIs for QE:
*/
// methods to convert between int/int and string representations:
string PKB::getProcedureRepresentation(int p) {
	return procTable.getProcName(p);
}

int PKB::getProcedure(string r) {
	return procTable.getProcIndex(r);
}

string PKB::getVariableRepresentation(int v) {
	return varTable.getVarName(v);
}

int PKB::getVariable(std::string r) {
	return varTable.getVarIndex(r);
}

string PKB::getStmtRepresentation(int s) {
	return to_string(s);
}

int PKB::getStmt(std::string r) {
	stringstream geek(r);
	int x = 0;
	geek >> x;
	return x;
}

string PKB::getConstantRepresentation(int c) {
	return to_string(c);
}

int PKB::getConstant(std::string cName) {
	stringstream geek(cName);
	int x = 0;
	geek >> x;
	return x;
}

// get all of given synonym type methods:

unordered_set<int> PKB::getAllConstants() {
	vector<int> constList = constTable.getConstTable();
	unordered_set<int> result(constList.begin(), constList.end());
	return result;
}

unordered_set<int> PKB::getAllStmts() {
	unordered_set<int> result;
	vector<pair<int, string>> table = stmtTable.getStmtTable();
	for (auto it = table.begin(); it != table.end(); ++it) {
		result.insert(it->first);
	}
	return result;
}

unordered_set<int> PKB::getAllAssigns() {
	unordered_set<int> result;
	vector<pair<int, string>> table = stmtTable.getStmtTable();
	for (auto it = table.begin(); it != table.end(); ++it) {
		if (it->second == "ASSIGN") {
			result.insert(it->first);
		}
	}
	return result;
}

unordered_set<int> PKB::getAllIfs() {
	unordered_set<int> result;
	vector<pair<int, string>> table = stmtTable.getStmtTable();
	for (auto it = table.begin(); it != table.end(); ++it) {
		if (it->second == "IF") {
			result.insert(it->first);
		}
	}
	return result;
}

unordered_set<int> PKB::getAllWhiles() {
	unordered_set<int> result;
	vector<pair<int, string>> table = stmtTable.getStmtTable();
	for (auto it = table.begin(); it != table.end(); ++it) {
		if (it->second == "WHILE") {
			result.insert(it->first);
		}
	}
	return result;
}

unordered_set<int> PKB::getAllCalls() {
	unordered_set<int> result;
	vector<pair<int, string>> table = stmtTable.getStmtTable();
	for (auto it = table.begin(); it != table.end(); ++it) {
		if (it->second == "CALL") {
			result.insert(it->first);
		}
	}
	return result;
}

unordered_set<int> PKB::getAllVariables() {
	unordered_set<int> result;
	int size = varTable.getSize();
	for (int i = 1; i <= size; i++) {
		result.insert(i);
	}
	return result;
}

unordered_set<int> PKB::getAllProcedures() {
	unordered_set<int> result;
	int size = procTable.getSize();
	for (int i = 1; i <= size; i++) {
		result.insert(i);
	}
	return result;
}

unordered_set<int> PKB::getAllCalledProcedure() {
	vector<int> list = callsTable.getAllCalledProc();
	unordered_set<int> result(list.begin(), list.end());
	return result;
}

//get statement types
bool PKB::isAssign(int s) {
	vector<pair<int, string>> table = stmtTable.getStmtTable();
	for (auto it = table.begin(); it != table.end(); ++it) {
		if (it->first == s && it->second == "ASSIGN") {
			return true;
		}
	}
	return false;
}

bool PKB::isIf(int s) {
	vector<pair<int, string>> table = stmtTable.getStmtTable();
	for (auto it = table.begin(); it != table.end(); ++it) {
		if (it->first == s && it->second == "IF") {
			return true;
		}
	}
	return false;
}

bool PKB::isWhile(int s) {
	vector<pair<int, string>> table = stmtTable.getStmtTable();
	for (auto it = table.begin(); it != table.end(); ++it) {
		if (it->first == s && it->second == "WHILE") {
			return true;
		}
	}
	return false;
}

bool PKB::isCall(int s) {
	vector<pair<int, string>> table = stmtTable.getStmtTable();
	for (auto it = table.begin(); it != table.end(); ++it) {
		if (it->first == s && it->second == "CALL") {
			return true;
		}
	}
	return false;
}

bool PKB::isStmt(int s) {
	vector<pair<int, string>> table = stmtTable.getStmtTable();
	for (auto it = table.begin(); it != table.end(); ++it) {
		if (it->first == s) {
			return true;
		}
	}
	return false;
}

bool PKB::isProgLine(int s) {
	//TODO:
	vector<pair<int, string>> table = stmtTable.getStmtTable();
	for (auto it = table.begin(); it != table.end(); ++it) {
		if (it->first == s) {
			return true;
		}
	}
	return false;
}

unordered_set<int> PKB::getAllIfswithControlVar(int v) {
	unordered_set<int> result;
	vector<int> list = stmtTable.getStmtwithControlVar(v);
	for (auto it = list.begin(); it != list.end(); ++it) {
		if (isIf(*it)) {
			result.insert(*it);
		}
	}
	return result;
}

unordered_set<int> PKB::getAllWhileswithControlVar(int v) {
	unordered_set<int> result;
	vector<int> list = stmtTable.getStmtwithControlVar(v);
	for (auto it = list.begin(); it != list.end(); ++it) {
		if (isWhile(*it)) {
			result.insert(*it);
		}
	}
	return result;
}

unordered_set<pair<int, int>, pair_hash> PKB::getAllIfControlVarPairs() {
	unordered_set<pair<int, int>, pair_hash> result;
	vector<pair<int, int>> pairs = stmtTable.getStmtControlVarPairs();
	for (auto it = pairs.begin(); it != pairs.end(); ++it) {
		if (isIf(it->first)) {
			result.insert(*it);
		}
	}
	return result;
}

unordered_set<pair<int, int>, pair_hash> PKB::getAllWhileControlVarPairs() {
	unordered_set<pair<int, int>, pair_hash> result;
	vector<pair<int, int>> pairs = stmtTable.getStmtControlVarPairs();
	for (auto it = pairs.begin(); it != pairs.end(); ++it) {
		if (isWhile(it->first)) {
			result.insert(*it);
		}
	}
	return result;
}


// Follows methods:
unordered_set<pair<int, int>, pair_hash> PKB::getFollowsPairs() {
	vector<pair<int, int>> pairList = followsTable.getFollowsPairs();
	unordered_set<pair<int, int>, pair_hash> result(pairList.begin(), pairList.end());
	return result;

}

int PKB::getFollowing(int s) {
	int result = followsTable.getFollows(s);
	if (result < 0) {
		return NO_RESULT;
	} else {
		return result;
	}
}

int PKB::getFollowedBy(int s) {
	int result = followsTable.getFollowedBy(s);
	if (result < 0) {
		return NO_RESULT;
	}
	else {
		return result;
	}
}

// Follows* methods
unordered_set<pair<int, int>, pair_hash> PKB::getTransFollowsPairs() {
	vector<pair<int, int>> pairList = followsStarTable.getFollowsStarPairs();
	unordered_set<pair<int, int>, pair_hash> result(pairList.begin(), pairList.end());
	return result;
}
unordered_set<int> PKB::getTransFollowing(int s) {
	vector<int> list = followsStarTable.getFollowsStar(s);
	unordered_set<int> result(list.begin(), list.end());
	return result;

}
unordered_set<int> PKB::getTransFollowedBy(int s) {
	vector<int> list = followsStarTable.getFollowedByStar(s);
	unordered_set<int> result(list.begin(), list.end());
	return result;
}

// Parent methods
unordered_set<pair<int, int>, pair_hash> PKB::getParentPairs() {
	vector<pair<int, int>> pairList = parentTable.getParentPairs();
	unordered_set<pair<int, int>, pair_hash> result(pairList.begin(), pairList.end());
	return result;
}

int PKB::getParentOf(int s) {
	int result = parentTable.getParent(s);
	if (result < 0) {
		return NO_RESULT;
	} else {
		return result;
	}
}

unordered_set<int> PKB::getChildrenOf(int s) {
	vector<int> list = parentTable.getChildren(s);
	unordered_set<int> result(list.begin(), list.end());
	return result;
}

// Parent* methods
unordered_set<pair<int, int>, pair_hash> PKB::getTransParentPairs() {
	vector<pair<int, int>> pairList = parentStarTable.getParentStarPairs();
	unordered_set<pair<int, int>, pair_hash> result(pairList.begin(), pairList.end());
	return result;
}

unordered_set<int> PKB::getTransParentOf(int s) {
	vector<int> list = parentStarTable.getParentStar(s);
	unordered_set<int> result(list.begin(), list.end());
	return result;
}

unordered_set<int> PKB::getTransChildrenOf(int s) {
	vector<int> list = parentStarTable.getChildrenStar(s);
	unordered_set<int> result(list.begin(), list.end());
	return result;
}

// Pattern expression matching methods
bool PKB::hasSubexpr(int a, PatternExpression expr) {
	string expression = patternTable.getExpression(a);
	size_t found = expression.find(expr);
	return (found != std::string::npos);
}

//get all assign statement that has expr as a substring on the right hand side
unordered_set<int> PKB::getSubExpr(std::string expr) {
	unordered_set<int> result;
	unordered_set<int> assigns = getAllAssigns();
	for (auto it = assigns.begin(); it != assigns.end(); ++it) {
		if (hasSubexpr(*it, expr)) {
			result.insert(*it);
		}
	}
	return result;
}

//get all assign statement that has expr as "exact string" on the right hand side
unordered_set<int> PKB::getExactExpr(std::string expr) {
	unordered_set<int> result;
	unordered_set<int> assigns = getAllAssigns();
	for (auto it = assigns.begin(); it != assigns.end(); ++it) {
		if (patternTable.getExpression(*it) == expr) {
			result.insert(*it);
		}
	}
	return result;
}

// Modifies methods
unordered_set<pair<int, int>, pair_hash> PKB::getModifiesStmtPairs() {
	vector<pair<int, int>> pairList = modifiesTable.getModifiesPairs();
	unordered_set<pair<int, int>, pair_hash> result(pairList.begin(), pairList.end());
	return result;
}

unordered_set<int> PKB::getStmtModifying(int v) {
	vector<int> list = modifiesTable.getModifying(v);
	unordered_set<int> result(list.begin(), list.end());
	return result;
}

unordered_set<int> PKB::getModifiedByStmt(int s) {
	vector<int> list = modifiesTable.getModifiedBy(s);
	unordered_set<int> result(list.begin(), list.end());
	return result;
}

unordered_set<pair<int, int>, pair_hash> PKB::getModifiesProcedurePairs() {
	vector<pair<int, int>> pairList = modifiesTable.getProcModifiesPairs();
	unordered_set<pair<int, int>, pair_hash> result(pairList.begin(), pairList.end());
	return result;
}

unordered_set<int> PKB::getProcedureModifying(int v) {
	vector<int> list = modifiesTable.getProcModifying(v);
	unordered_set<int> result(list.begin(), list.end());
	return result;
}

unordered_set<int> PKB::getModifiedByProcedure(int p) {
	vector<int> list = modifiesTable.getModifiedByProc(p);
	unordered_set<int> result(list.begin(), list.end());
	return result;
}

// Uses methods
unordered_set<pair<int, int>, pair_hash> PKB::getUsesStmtPairs() {
	vector<pair<int, int>> pairList = usesTable.getUsesPairs();
	unordered_set<pair<int, int>, pair_hash> result(pairList.begin(), pairList.end());
	return result;
}

unordered_set<int> PKB::getStmtUsing(int v) {
	vector<int> list = usesTable.getUsing(v);
	unordered_set<int> result(list.begin(), list.end());
	return result;
}

unordered_set<int> PKB::getUsedByStmt(int s) {
	vector<int> list = usesTable.getUsedBy(s);
	unordered_set<int> result(list.begin(), list.end());
	return result;
}

unordered_set<pair<int, int>, pair_hash> PKB::getUsesProcedurePairs() {
	vector<pair<int, int>> pairList = usesTable.getProcUsesPairs();
	unordered_set<pair<int, int>, pair_hash> result(pairList.begin(), pairList.end());
	return result;
}

unordered_set<int> PKB::getProcedureUsing(int v) {
	vector<int> list = usesTable.getProcUsing(v);
	unordered_set<int> result(list.begin(), list.end());
	return result;
}

unordered_set<int> PKB::getUsedByProcedure(int p) {
	vector<int> list = usesTable.getUsedByProc(p);
	unordered_set<int> result(list.begin(), list.end());
	return result;
}

// Next methods
unordered_set<pair<int, int>, pair_hash> PKB::getNextPairs() {
	vector<pair<int, int>> pairList = nextTable.getNextPairs();
	unordered_set<pair<int, int>, pair_hash> result(pairList.begin(), pairList.end());
	return result;
}

unordered_set<int> PKB::getNext(int s) {
	vector<int> list = nextTable.getNext(s);
	unordered_set<int> result(list.begin(), list.end());
	return result;
}

unordered_set<int> PKB::getPrevious(int s) {
	vector<int> list = nextTable.getPrevious(s);
	unordered_set<int> result(list.begin(), list.end());
	return result;
}

// Calls methods
unordered_set<pair<int, int>, pair_hash> PKB::getCallsProcedurePairs() {
	vector<pair<int, int>> pairList = callsTable.getCallsPairs();
	unordered_set<pair<int, int>, pair_hash> result(pairList.begin(), pairList.end());
	return result;
}

unordered_set<int> PKB::getProcedureCalling(int s) {
	vector<int> list = callsTable.getCalls(s);
	unordered_set<int> result(list.begin(), list.end());
	return result;
}

unordered_set<int> PKB::getCalledByProcedure(int s) {
	vector<int> list = callsTable.getCalledBy(s);
	unordered_set<int> result(list.begin(), list.end());
	return result;
}

// Calls* methods
unordered_set<pair<int, int>, pair_hash> PKB::getTransCallsProcedurePairs() {
	vector<pair<int, int>> pairList = callsStarTable.getCallsStarPairs();
	unordered_set<pair<int, int>, pair_hash> result(pairList.begin(), pairList.end());
	return result;
}

// get all p such that Calls*(p, s) is true
unordered_set<int> PKB::getProcedureTransCalling(int s) {
	vector<int> list = callsStarTable.getCallsStar(s);
	unordered_set<int> result(list.begin(), list.end());
	return result;
}

// get all p such that Calls*(s, p) is true;
unordered_set<int> PKB::getTransCalledByProcedure(int s) {
	vector<int> list = callsStarTable.getCalledByStar(s);
	unordered_set<int> result(list.begin(), list.end());
	return result;
}

