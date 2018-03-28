#pragma once

#include <utility>
#include <unordered_set>

#include "Pair.h"

using PatternExpression = std::string;

class ResultFetcher {
public:
	// this value should only be used in methods that return int;
	// methods that return ResultVector should return an empty vector.
	static const int NO_RESULT = -1;

	// methods to convert int to string representations
	virtual std::string getVariableRepresentation(int v) = 0;
	virtual int getVariable(std::string vName) = 0;
	virtual std::string getStmtRepresentation(int s) = 0;
	virtual int getStmt(std::string sName) = 0;
	virtual std::string getConstantRepresentation(int c) = 0;
	virtual int getConstant(std::string cName) = 0;
	virtual std::string getProcedureRepresentation(int p) = 0;
	virtual int getProcedure(std::string pName) = 0;

	// get all of given synonym type methods
	virtual std::unordered_set<int> getAllProcedures() = 0;
	virtual std::unordered_set<int> getAllVariables() = 0;
	virtual std::unordered_set<int> getAllStmts() = 0;
	virtual std::unordered_set<int> getAllAssigns() = 0;
	virtual std::unordered_set<int> getAllIfs() = 0;
	virtual std::unordered_set<int> getAllCalls() = 0;
	virtual std::unordered_set<int> getAllWhiles() = 0;
	virtual std::unordered_set<int> getAllConstants() = 0;

	// get all procedures that are called by call statement
	virtual std::unordered_set<int> getAllCalledProcedure() = 0;

	//get statement types
	virtual bool isAssign(int s) = 0;
	virtual bool isIf(int s) = 0;
	virtual bool isWhile(int s) = 0;
	virtual bool isCall(int s) = 0;
	virtual bool isStmt(int s) = 0;
	virtual bool isProgLine(int s) = 0;

	virtual std::unordered_set<int> getAllIfswithControlVar(int v) = 0;
	virtual std::unordered_set<int> getAllWhileswithControlVar(int v) = 0;
	virtual std::unordered_set<std::pair<int, int>, pair_hash> getAllIfControlVarPairs() = 0;
	virtual std::unordered_set<std::pair<int, int>, pair_hash> getAllWhileControlVarPairs() = 0;


	// Modifies methods

	virtual std::unordered_set<std::pair<int, int>, pair_hash> getModifiesStmtPairs() = 0;
	/* <Procedure, Variable> */
	virtual std::unordered_set<std::pair<int, int>, pair_hash> getModifiesProcedurePairs() = 0;
	virtual std::unordered_set<int> getStmtModifying(int v) = 0;
	virtual std::unordered_set<int> getProcedureModifying(int v) = 0;
	virtual std::unordered_set<int> getModifiedByStmt(int s) = 0;
	virtual std::unordered_set<int> getModifiedByProcedure(int p) = 0;

	// Uses methods
	/* <Stmt, Variable> */
	virtual std::unordered_set<std::pair<int, int>, pair_hash> getUsesStmtPairs() = 0;
	/* <Procedure, Variable> */
	virtual std::unordered_set<std::pair<int, int>, pair_hash> getUsesProcedurePairs() = 0;
	virtual std::unordered_set<int> getStmtUsing(int v) = 0;
	virtual std::unordered_set<int> getProcedureUsing(int v) = 0;
	virtual std::unordered_set<int> getUsedByStmt(int s) = 0;
	virtual std::unordered_set<int> getUsedByProcedure(int p) = 0;

	// Parent methods
	/* <Stmt, Stmt> */
	virtual std::unordered_set<std::pair<int, int>, pair_hash> getParentPairs() = 0;
	virtual int getParentOf(int s) = 0;
	virtual std::unordered_set<int> getChildrenOf(int s) = 0;
	// Parent* methods
	virtual std::unordered_set<std::pair<int, int>, pair_hash> getTransParentPairs() = 0;
	virtual std::unordered_set<int> getTransParentOf(int s) = 0;
	virtual std::unordered_set<int> getTransChildrenOf(int s) = 0;

	// Follows methods
	virtual std::unordered_set<std::pair<int, int>, pair_hash> getFollowsPairs() = 0;
	virtual int getFollowing(int s) = 0;
	virtual int getFollowedBy(int s) = 0;
	// Follows* methods
	virtual std::unordered_set<std::pair<int, int>, pair_hash> getTransFollowsPairs() = 0;
	virtual std::unordered_set<int> getTransFollowing(int s) = 0;
	virtual std::unordered_set<int> getTransFollowedBy(int s) = 0;

	// Next methods
	virtual std::unordered_set<std::pair<int, int>, pair_hash> getNextPairs() = 0;
	virtual std::unordered_set<int> getNext(int s) = 0;
	virtual std::unordered_set<int> getPrevious(int s) = 0;
	
	/*
	// Next* methods
	virtual std::unordered_set<std::pair<int, int>> getTransNextPairs() = 0;
	virtual std::unordered_set<int> getTransNext(int s) = 0;
	virtual std::unordered_set<int> getTransPrevious(int s) = 0;
	*/

	//Call: only between procedures
	virtual std::unordered_set<std::pair<int, int>, pair_hash> getCallsProcedurePairs() = 0;
	virtual std::unordered_set<int> getProcedureCalling(int s) = 0;
	virtual std::unordered_set<int> getCalledByProcedure(int s) = 0;
	virtual int getProcinCallStmt(int s) = 0;
	
	// Calls*
	virtual std::unordered_set<std::pair<int, int>, pair_hash> getTransCallsProcedurePairs() = 0;
	// get all p such that Calls*(p, s) is true
	virtual std::unordered_set<int> getProcedureTransCalling(int s) = 0;
	// get all p such that Calls*(s, p) is true;
	virtual std::unordered_set<int> getTransCalledByProcedure(int s) = 0;
		
	// Pattern: expr is already postfix
	//get all assign statement that has expr as a substring on the right hand side
	virtual std::unordered_set<int> getSubExpr(std::string expr) = 0;
	//get all assign statement that has expr as "exact string" on the right hand side
	virtual std::unordered_set<int> getExactExpr(std::string expr) = 0;
};
