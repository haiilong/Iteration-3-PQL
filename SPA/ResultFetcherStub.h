#pragma once

#include <string>
#include <unordered_set>
#include <unordered_set>

#include "ResultFetcher.h" // TODO: refactor to including the ResultsFetcher interface
#include "Pair.h"


class ResultFetcherStub :
	public ResultFetcher {
public:
	std::string getProcedureRepresentation(int p);
	int getProcedure(std::string pStr);
	std::string getVariableRepresentation(int v) override;
	int getVariable(std::string vStr) override;
	std::string getStmtRepresentation(int s) override;
	int getStmt(std::string sStr) override;
	std::string getConstantRepresentation(int c) override;
	int getConstant(std::string cStr) override;

	std::unordered_set<int> getAllProcedures() override;
	std::unordered_set<int> getAllVariables() override;
	std::unordered_set<int> getAllStmts() override;
	std::unordered_set<int> getAllAssigns() override;
	std::unordered_set<int> getAllIfs() override;
	std::unordered_set<int> getAllWhiles() override;
	std::unordered_set<int> getAllConstants() override;
	std::unordered_set<int> getAllCalls() override;
	std::unordered_set<int> getAllCalledProcedure() override;

	//get procedure called by call stmt s
	int getProcinCallStmt(int s) override;

	//statement type
	bool isAssign(int s) override;
	bool isIf(int s) override;
	bool isWhile(int s) override;
	bool isCall(int s) override;
	bool isStmt(int s) override;
	bool isProgLine(int s) override;

	//if/while pattern methods
	std::unordered_set<int> getAllIfswithControlVar(int v) override;
	std::unordered_set<int> getAllWhileswithControlVar(int v) override;
	std::unordered_set<std::pair<int, int>, pair_hash> getAllIfControlVarPairs() override;
	std::unordered_set<std::pair<int, int>, pair_hash> getAllWhileControlVarPairs() override;

	// Parent methods
	/* <Stmt, Stmt> */
	std::unordered_set<std::pair<int, int>, pair_hash> getParentPairs() override;
	int getParentOf(int s) override;
	std::unordered_set<int> getChildrenOf(int s) override;
	// Parent* methods
	std::unordered_set<std::pair<int, int>, pair_hash> getTransParentPairs() override;
	std::unordered_set<int> getTransParentOf(int s) override;
	std::unordered_set<int> getTransChildrenOf(int s) override;

	// Follows methods
	std::unordered_set<std::pair<int, int>, pair_hash> getFollowsPairs() override;
	int getFollowing(int s) override;
	int getFollowedBy(int s) override;
	// Follows* methods
	std::unordered_set<std::pair<int, int>, pair_hash> getTransFollowsPairs() override;
	std::unordered_set<int> getTransFollowing(int s) override;
	std::unordered_set<int> getTransFollowedBy(int s) override;

	// Modifies methods
	/* <Stmt, Variable> */
	std::unordered_set<std::pair<int, int>, pair_hash> getModifiesStmtPairs() override;
	/* <Procedure, Variable> */
	std::unordered_set<std::pair<int, int>, pair_hash> getModifiesProcedurePairs() override;
	std::unordered_set<int> getStmtModifying(int v) override;
	std::unordered_set<int> getProcedureModifying(int v) override;
	std::unordered_set<int> getModifiedByStmt(int s) override;
	std::unordered_set<int> getModifiedByProcedure(int s) override;

	// Uses methods
	/* <Procedure, Variable> */
	//std::unordered_set<std::pair<int, int>, pair_hash> getProcedureUsesPairs() const;
	/* <Stmt, Variable> */
	std::unordered_set<std::pair<int, int>, pair_hash> getUsesStmtPairs() override;
	std::unordered_set<std::pair<int, int>, pair_hash> getUsesProcedurePairs() override;
	//std::unordered_set<int> getProcedureUsing(int v) const;
	std::unordered_set<int> getStmtUsing(int v) override;
	std::unordered_set<int> getProcedureUsing(int v) override;
	std::unordered_set<int> getUsedByStmt(int s) override;
	std::unordered_set<int> getUsedByProcedure(int s) override;

	//Call methods: only between procedures
	std::unordered_set<std::pair<int, int>, pair_hash> getCallsProcedurePairs() override;
	std::unordered_set<int> getProcedureCalling(int s) override;
	std::unordered_set<int> getCalledByProcedure(int s) override;
	// Calls* methods
	std::unordered_set<std::pair<int, int>, pair_hash> getTransCallsProcedurePairs() override;
	std::unordered_set<int> getProcedureTransCalling(int s) override;
	std::unordered_set<int> getTransCalledByProcedure(int s) override;

	// Next methods
	virtual std::unordered_set<std::pair<int, int>, pair_hash> getNextPairs() override;
	virtual std::unordered_set<int> getNext(int s) override;
	virtual std::unordered_set<int> getPrevious(int s) override;

	// Pattern expression matching methods
	//get all assign statement that has expr as a substring on the right hand side
	virtual std::unordered_set<int> getSubExpr(std::string expr) override;
	//get all assign statement that has expr as "exact string" on the right hand side
	virtual std::unordered_set<int> getExactExpr(std::string expr) override;

	// Internal return variables
	std::string procedureRepresentation;
	mutable int procedureRepresentationCalls = 0;
	mutable int procedureCalls = 0;
	std::string variableRepresentation;
	mutable int variableRepresentationCalls = 0;
	mutable int variableCalls = 0;
	mutable int stmtRepresentationCalls = 0;
	mutable int stmtCalls = 0;
	mutable int constantCalls = 0;
	mutable int constantRepresentationCalls = 0;

	mutable int procinCallStmt = 0;

	std::unordered_set<int> allProcedures;
	mutable int allProceduresCalls = 0;
	std::unordered_set<int> allVariables;
	mutable int allVariablesCalls = 0;
	std::unordered_set<int> allStmts;
	mutable int allStmtsCalls = 0;
	std::unordered_set<int> allAssigns;
	mutable int allAssignsCalls = 0;
	std::unordered_set<int> allIfs;
	mutable int allIfsCalls = 0;
	std::unordered_set<int> allWhiles;
	mutable int allWhilesCalls = 0;
	std::unordered_set<int> allCalls;
	mutable int allCallsCalls = 0;
	std::unordered_set<int> allConstants;
	mutable int allConstantsCalls = 0;
	std::unordered_set<int> allCalledProcedure;

	//statement type
	mutable bool isAssignType = 0;
	mutable bool isIfType = 0;
	mutable bool isWhileType = 0;
	mutable bool isCallType = 0;
	mutable bool isStmtType = 0;
	mutable bool isProgLineType = 0;

	// Parent methods
	/* <Stmt, Stmt> */
	std::unordered_set<std::pair<int, int>, pair_hash> parentPairs;
	mutable int parentPairsCalls = 0;
	mutable int parentOf = 0;
	mutable int parentOfCalls = 0;
	std::unordered_set<int> childrenOf;
	mutable int childrenOfCalls = 0;
	// Parent* methods
	std::unordered_set<std::pair<int, int>, pair_hash> transParentPairs;
	mutable int transParentPairsCalls = 0;
	std::unordered_set<int> transParentOf;
	mutable int transParentOfCalls = 0;
	std::unordered_set<int> transChildrenOf;
	mutable int transChildrenOfCalls = 0;

	// Follows methods
	std::unordered_set<std::pair<int, int>, pair_hash> followsPairs;
	mutable int followsPairsCalls = 0;
	mutable int following = 0;
	mutable int followingCalls = 0;
	mutable int followedBy = 0;
	mutable int followedByCalls = 0;
	// Follows* methods
	std::unordered_set<std::pair<int, int>, pair_hash> transFollowsPairs;
	mutable int transFollowsPairsCalls = 0;
	std::unordered_set<int> transFollowing;
	mutable int transFollowingCalls = 0;
	std::unordered_set<int> transFollowedBy;
	mutable int transFollowedByCalls = 0;

	// Modifies methods
	/* <Procedure, Variable> */
	/* <Stmt, Variable> */
	std::unordered_set<std::pair<int, int>, pair_hash> modifiesStmtPairs;
	mutable int modifiesStmtPairsCalls = 0;
	std::unordered_set<std::pair<int, int>, pair_hash> modifiesProcedurePairs;
	mutable int modifiesProcedurePairsCalls = 0;
	std::unordered_set<int> procedureModifying;
	mutable int procedureModifyingCalls = 0;
	std::unordered_set<int> stmtModifying;
	mutable int stmtModifyingCalls = 0;
	std::unordered_set<int> modifiedByStmt;
	mutable int modifiedByStmtCalls = 0;
	std::unordered_set<int> modifiedByProcedure;
	mutable int modifiedByProcedureCalls = 0;

	// Uses methods
	/* <Procedure, Variable> */
	/* <Stmt, Variable> */
	std::unordered_set<std::pair<int, int>, pair_hash> usesStmtPairs;
	mutable int usesStmtPairsCalls = 0;
	std::unordered_set<std::pair<int, int>, pair_hash> usesProcedurePairs;
	mutable int usesProcedurePairsCalls = 0;
	std::unordered_set<int> procedureUsing;
	mutable int procedureUsingCalls = 0;
	std::unordered_set<int> stmtUsing;
	mutable int stmtUsingCalls = 0;
	std::unordered_set<int> usedByStmt;
	mutable int usedByStmtCalls = 0;
	std::unordered_set<int> usedByProcedure;
	mutable int usedByProcedureCalls = 0;

	//Call methods: only between procedures
	std::unordered_set<std::pair<int, int>, pair_hash> callsProcedurePairs;
	mutable int callsProcedurePairsCalls = 0;
	std::unordered_set<int> procedureCalling;
	mutable int procedureCallingCalls = 0;
	std::unordered_set<int> calledByProcedure;
	mutable int calledByProcedureCalls = 0;

	// Calls* methods
	std::unordered_set<std::pair<int, int>, pair_hash> transCallsProcedurePairs;
	mutable int transCallsProcedurePairsCalls = 0;
	std::unordered_set<int> procedureTransCalling;
	mutable int procedureTransCallingCalls = 0;
	std::unordered_set<int> transCalledByProcedure;
	mutable int transCalledByProcedureCalls = 0;

	// Next methods
	std::unordered_set<std::pair<int, int>, pair_hash> nextPairs;
	mutable int nextPairsCalls = 0;
	std::unordered_set<int> next;
	mutable int nextCalls = 0;
	std::unordered_set<int> previous;
	mutable int previousCalls = 0;

	// Pattern expression matching methods
	mutable int subExprCalls = 0;
	mutable std::unordered_set<int> patternWhitelist;
	mutable int exactExprCalls = 0;

	mutable int totalMethodCalls = 0;

	//pattern methods
	std::unordered_set<int> allIfswithControlVar;
	std::unordered_set<int> allWhileswithControlVar;
	std::unordered_set<std::pair<int, int>, pair_hash> allIfControlVarPairs;
	std::unordered_set<std::pair<int, int>, pair_hash> allWhileControlVarPairs;
};
