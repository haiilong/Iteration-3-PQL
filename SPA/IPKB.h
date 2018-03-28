#pragma once

#include <list>
#include <string>
#include <vector>
#include <unordered_set>

class IPKB {
public:
	/**
	public APIs for Parser:
	*/

	// set the synonyms
	virtual int insertVar(std::string varName) = 0;
	virtual int insertStmt(int stmtNum, std::string type, int p) = 0; //stmt type refers to: ASSIGN, IF, WHILE, CALL
	virtual int insertProc(std::string name) = 0;
	virtual int insertPatternExpression(int stmtNum, std::string expression) = 0;
	virtual int insertConst(int c) = 0;
	virtual int setControlVariable(int s, int v) = 0;

	// set the relationships
	virtual int setFollows(int stmt1, int stmt2) = 0;
	virtual int setParent(int stmt1, int stmt2) = 0;
	virtual int setModifies(int s, int vindex) = 0;
	virtual int setUses(int s, int vindex) = 0;
	virtual int setNext(int s1, int s2) = 0;
	virtual int setCalls(int s, int p1, int p2) = 0; // stmt s in p1 calls p2
	virtual int setProcModifies(int proc, int var) = 0;
	virtual int setProcUses(int proc, int var) = 0;

	virtual std::unordered_set<int> getModifiedByStmt(int s) = 0;
	virtual std::unordered_set<int> getUsedByStmt(int s) = 0;
};