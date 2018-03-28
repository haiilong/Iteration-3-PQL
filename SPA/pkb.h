#pragma once

#include<stdio.h>
#include <iostream>
#include <vector>
#include <list>
#include <unordered_set>

#include "Pair.h"
#include "IPKB.h"
#include "ResultFetcher.h"
#include "VarTable.h"
#include "StmtTable.h"
#include "ProcTable.h"
#include "patternTable.h"
#include "Follows.h"
#include "FollowsStar.h"
#include "Parent.h"
#include "ParentStar.h"
#include "ConstTable.h"
#include "Modifies.h"
#include "Uses.h"
#include "Next.h"
#include "Calls.h"
#include "CallsStar.h"

class PKB : public IPKB, public ResultFetcher {
public:
	// fields
	VarTable varTable;
	StmtTable stmtTable;
	ProcTable procTable;
	ConstTable constTable;
	PatternTable patternTable;
	Follows followsTable;
	FollowsStar followsStarTable;
	Parent parentTable;
	ParentStar parentStarTable;
	Modifies modifiesTable;
	Uses usesTable;
	Next nextTable;
	Calls callsTable;
	CallsStar callsStarTable;

	// Return a singleton instance of PKB
	static PKB* getPKBInstance();
	
	// Clear PKB
	static void clearPKB();

	/**
	public APIs for Parser:
	*/

	// set the synonyms
	int insertVar(std::string varName);
	// stmt type refers to: ASSIGN, IF, WHILE
	int insertStmt(int stmtNum, std::string type, int p); 
	int insertProc(std::string name);
	int insertPatternExpression(int stmtNum, std::string expression);
	int insertConst(int c);
	int setControlVariable(int s, int v);

	// set the relationships:

	// set stmt2 follows stmt1
	int setFollows(int stmt1, int stmt2);
	// set stmt1 as the parent of stmt2
	int setParent(int stmt1, int stmt2);
	int setModifies(int s, int vindex);
	int setUses(int s, int vindex);
	int setNext(int s1, int s2);
	int setCalls(int s, int p1, int p2);
	int setProcModifies(int proc, int var);
	int setProcUses(int proc, int var);
	
	/**
	public APIs for DE:
	*/
	std::vector<std::pair<int, int>> getFollowsPairsList();
	std::vector<std::pair<int, int>> getParentPairsList();
	std::vector<std::pair<int, int>> getCallsPairsList();

	int getProcinCallStmt(int s);


	/**
	public APIs for QE:
	*/

	// methods to convert between int/int and string representations
	std::string getProcedureRepresentation(int p);
	int getProcedure(std::string r);
	
	std::string getVariableRepresentation(int v);
	int getVariable(std::string r);

	std::string getStmtRepresentation(int s);
	int getStmt(std::string r);

	std::string getConstantRepresentation(int c);
	int getConstant(std::string cName);

	// get all of given synonym type methods
	std::unordered_set<int> getAllProcedures();
	std::unordered_set<int> getAllCalledProcedure();
	std::unordered_set<int> getAllVariables();
	std::unordered_set<int> getAllStmts();
	std::unordered_set<int> getAllAssigns();
	std::unordered_set<int> getAllIfs();
	std::unordered_set<int> getAllWhiles();
	std::unordered_set<int> getAllCalls();
	std::unordered_set<int> getAllConstants();

	// get statement types
	bool isAssign(int s);
	bool isIf(int s);
	bool isWhile(int s);
	bool isCall(int s);
	bool isStmt(int s);
	bool isProgLine(int s);

	// control variable APIs
	std::unordered_set<int> getAllIfswithControlVar(int v);
	std::unordered_set<int> getAllWhileswithControlVar(int v);
	std::unordered_set<std::pair<int, int>, pair_hash> getAllIfControlVarPairs();
	std::unordered_set<std::pair<int, int>, pair_hash> getAllWhileControlVarPairs();

	// Parent methods
	/* <Stmt, Stmt> */
	std::unordered_set<std::pair<int, int>, pair_hash> getParentPairs();
	int getParentOf(int s);
	std::unordered_set<int> getChildrenOf(int s);

	// Parent* methods
	std::unordered_set<std::pair<int, int>, pair_hash> getTransParentPairs();
	std::unordered_set<int> getTransParentOf(int s);
	std::unordered_set<int> getTransChildrenOf(int s);

	// Follows methods
	std::unordered_set<std::pair<int, int>, pair_hash> getFollowsPairs();
	int getFollowing(int s);
	int getFollowedBy(int s);

	// Follows* methods
	std::unordered_set<std::pair<int, int>, pair_hash> getTransFollowsPairs();
	std::unordered_set<int> getTransFollowing(int s);
	std::unordered_set<int> getTransFollowedBy(int s);

	// Modifies methods
	std::unordered_set<std::pair<int, int>, pair_hash> getModifiesStmtPairs();
	std::unordered_set<int> getStmtModifying(int v);
	std::unordered_set<int> getModifiedByStmt(int s);

	std::unordered_set<std::pair<int, int>, pair_hash> getModifiesProcedurePairs();
	std::unordered_set<int> getProcedureModifying(int v);
	std::unordered_set<int> getModifiedByProcedure(int p);

	// Uses methods
	std::unordered_set<std::pair<int, int>, pair_hash> getUsesStmtPairs();
	std::unordered_set<int> getStmtUsing(int v);
	std::unordered_set<int> getUsedByStmt(int s);

	std::unordered_set<std::pair<int, int>, pair_hash> getUsesProcedurePairs();
	std::unordered_set<int> getProcedureUsing(int v);
	std::unordered_set<int> getUsedByProcedure(int p);

	// Next methods
	std::unordered_set<std::pair<int, int>, pair_hash> getNextPairs();
	std::unordered_set<int> getNext(int s);
	std::unordered_set<int> getPrevious(int s);

	//Call: only between procedures
	std::unordered_set<std::pair<int, int>, pair_hash> getCallsProcedurePairs();
	std::unordered_set<int> getProcedureCalling(int s); // get all p such that Calls(p,s) is true
	std::unordered_set<int> getCalledByProcedure(int s); // get all p such that Calls(s,p) is true

	// Calls*
	std::unordered_set<std::pair<int, int>, pair_hash> getTransCallsProcedurePairs();
	// get all p such that Calls*(p, s) is true
	std::unordered_set<int> getProcedureTransCalling(int s);
	// get all p such that Calls*(s, p) is true;
	std::unordered_set<int> getTransCalledByProcedure(int s);

	// Pattern: expr is already postfix
	//get all assign statement that has expr as a substring on the right hand side
	std::unordered_set<int> getSubExpr(std::string expr);
	//get all assign statement that has expr as "exact string" on the right hand side
	std::unordered_set<int> getExactExpr(std::string expr);
	bool hasSubexpr(int a, PatternExpression expr);

private:
    static PKB* instance;
    PKB();
};
