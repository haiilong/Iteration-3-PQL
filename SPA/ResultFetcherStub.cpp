#include "ResultFetcherStub.h"

std::string ResultFetcherStub::getProcedureRepresentation(int p) {
	++totalMethodCalls;
	++procedureRepresentationCalls;
	return std::to_string(p);
}
int ResultFetcherStub::getProcedure(std::string pStr) {
	++totalMethodCalls;
	++procedureCalls;
	return std::stoi(pStr);
}
std::string ResultFetcherStub::getVariableRepresentation(int v) {
	++totalMethodCalls;
	++variableRepresentationCalls;
	return std::to_string(v);
}
int ResultFetcherStub::getVariable(std::string vStr) {
	++totalMethodCalls;
	++variableCalls;
	return std::stoi(vStr);
}
std::string ResultFetcherStub::getStmtRepresentation(int s) {
	++totalMethodCalls;
	++stmtRepresentationCalls;
	return std::to_string(s);
}

int ResultFetcherStub::getStmt(std::string sStr) {
	++totalMethodCalls;
	++stmtCalls;
	return std::stoi(sStr);
}

std::string ResultFetcherStub::getConstantRepresentation(int c) {
	++totalMethodCalls;
	++constantRepresentationCalls;
	return std::to_string(c);
}

int ResultFetcherStub::getConstant(std::string cStr) {
	++totalMethodCalls;
	++constantCalls;
	return std::stoi(cStr);
}

// get all of given synonym type methods
std::unordered_set<int> ResultFetcherStub::getAllProcedures() {
	++totalMethodCalls;
	++allProceduresCalls;
	return allProcedures;
}
std::unordered_set<int> ResultFetcherStub::getAllVariables() {
	++totalMethodCalls;
	++allVariablesCalls;
	return allVariables;
}
std::unordered_set<int> ResultFetcherStub::getAllStmts() {
	++totalMethodCalls;
	++allStmtsCalls;
	return allStmts;
}
std::unordered_set<int> ResultFetcherStub::getAllAssigns() {
	++totalMethodCalls;
	++allAssignsCalls;
	return allAssigns;
}
std::unordered_set<int>ResultFetcherStub::getAllIfs() {
	++totalMethodCalls;
	++allIfsCalls;
	return allIfs;
}
std::unordered_set<int> ResultFetcherStub::getAllWhiles() {
	++totalMethodCalls;
	++allWhilesCalls;
	return allWhiles;
}

std::unordered_set<int> ResultFetcherStub::getAllCalls() {
	++totalMethodCalls;
	++allCallsCalls;
	return allCalls;
}

std::unordered_set<int> ResultFetcherStub::getAllCalledProcedure()
{
	++totalMethodCalls;
	return allCalledProcedure;
}

int ResultFetcherStub::getProcinCallStmt(int s)
{
	++totalMethodCalls;
	return procinCallStmt;
}

std::unordered_set<int> ResultFetcherStub::getAllConstants() {
	++totalMethodCalls;
	++allConstantsCalls;
	return allConstants;
}

// this value should only be used in methods that return int;
// methods that return ResultVector should return an empty unordered_set.
static const int NO_RESULT = -1;

// Parent methods
/* <Stmt, Stmt> */
std::unordered_set<std::pair<int, int>, pair_hash> ResultFetcherStub::getParentPairs() {
	++totalMethodCalls;
	++parentPairsCalls;
	return parentPairs;
}
int ResultFetcherStub::getParentOf(int s) {
	++totalMethodCalls;
	++parentOfCalls;
	return parentOf;
}
std::unordered_set<int> ResultFetcherStub::getChildrenOf(int s) {
	++totalMethodCalls;
	++childrenOfCalls;
	return childrenOf;
}
// Parent* methods
std::unordered_set<std::pair<int, int>, pair_hash> ResultFetcherStub::getTransParentPairs() {
	++totalMethodCalls;
	++transParentPairsCalls;
	return transParentPairs;
}
std::unordered_set<int> ResultFetcherStub::getTransParentOf(int s) {
	++totalMethodCalls;
	++transParentOfCalls;
	return transParentOf;
}
std::unordered_set<int> ResultFetcherStub::getTransChildrenOf(int s) {
	++totalMethodCalls;
	++transChildrenOfCalls;
	return transChildrenOf;
}

// Follows methods
std::unordered_set<std::pair<int, int>, pair_hash> ResultFetcherStub::getFollowsPairs() {
	++totalMethodCalls;
	++followsPairsCalls;
	return followsPairs;
}
int ResultFetcherStub::getFollowing(int s) {
	++totalMethodCalls;
	++followingCalls;
	return following;
}
int ResultFetcherStub::getFollowedBy(int s) {
	++totalMethodCalls;
	++followedByCalls;
	return followedBy;
}
// Follows* methods
std::unordered_set<std::pair<int, int>, pair_hash> ResultFetcherStub::getTransFollowsPairs() {
	++totalMethodCalls;
	++transFollowsPairsCalls;
	return transFollowsPairs;
}
std::unordered_set<int> ResultFetcherStub::getTransFollowing(int s) {
	++totalMethodCalls;
	++transFollowingCalls;
	return transFollowing;
}
std::unordered_set<int> ResultFetcherStub::getTransFollowedBy(int s) {
	++totalMethodCalls;
	++transFollowedByCalls;
	return transFollowedBy;
}

std::unordered_set<std::pair<int, int>, pair_hash> ResultFetcherStub::getModifiesStmtPairs() {
	++totalMethodCalls;
	++modifiesStmtPairsCalls;
	return modifiesStmtPairs;
}

std::unordered_set<std::pair<int, int>, pair_hash> ResultFetcherStub::getModifiesProcedurePairs() {
	++totalMethodCalls;
	++modifiesProcedurePairsCalls;
	return modifiesProcedurePairs;
}

std::unordered_set<int> ResultFetcherStub::getStmtModifying(int v) {
	++totalMethodCalls;
	++stmtModifyingCalls;
	return stmtModifying;
}

std::unordered_set<int> ResultFetcherStub::getProcedureModifying(int v) {
	++totalMethodCalls;
	++procedureModifyingCalls;
	return procedureModifying;
}

std::unordered_set<int> ResultFetcherStub::getModifiedByStmt(int s) {
	++totalMethodCalls;
	++modifiedByStmtCalls;
	return modifiedByStmt;
}

std::unordered_set<int> ResultFetcherStub::getModifiedByProcedure(int s) {
	++totalMethodCalls;
	++modifiedByProcedureCalls;
	return modifiedByProcedure;
}

std::unordered_set<std::pair<int, int>, pair_hash> ResultFetcherStub::getUsesStmtPairs() {
	++totalMethodCalls;
	++usesStmtPairsCalls;
	return usesStmtPairs;
}

std::unordered_set<std::pair<int, int>, pair_hash> ResultFetcherStub::getUsesProcedurePairs() {
	++totalMethodCalls;
	++usesProcedurePairsCalls;
	return usesProcedurePairs;
}

std::unordered_set<int> ResultFetcherStub::getStmtUsing(int v) {
	++totalMethodCalls;
	++stmtUsingCalls;
	return stmtUsing;
}

std::unordered_set<int> ResultFetcherStub::getProcedureUsing(int v) {
	++totalMethodCalls;
	++procedureUsingCalls;
	return procedureUsing;
}


std::unordered_set<int> ResultFetcherStub::getUsedByStmt(int s) {
	++totalMethodCalls;
	++usedByStmtCalls;
	return usedByStmt;
}

std::unordered_set<int> ResultFetcherStub::getUsedByProcedure(int s) {
	++totalMethodCalls;
	++usedByProcedureCalls;
	return usedByProcedure;
}

//Calls method
std::unordered_set<std::pair<int, int>, pair_hash> ResultFetcherStub::getCallsProcedurePairs()
{
	++totalMethodCalls;
	++callsProcedurePairsCalls;
	return callsProcedurePairs;
}

std::unordered_set<int> ResultFetcherStub::getProcedureCalling(int s)
{
	++totalMethodCalls;
	++procedureCallingCalls;
	return procedureCalling;
}

std::unordered_set<int> ResultFetcherStub::getCalledByProcedure(int s)
{
	++totalMethodCalls;
	++calledByProcedureCalls;
	return calledByProcedure;
}

std::unordered_set<std::pair<int, int>, pair_hash> ResultFetcherStub::getTransCallsProcedurePairs()
{
	++totalMethodCalls;
	++transCallsProcedurePairsCalls;
	return transCallsProcedurePairs;
}

std::unordered_set<int> ResultFetcherStub::getProcedureTransCalling(int s)
{
	++totalMethodCalls;
	++procedureTransCallingCalls;
	return procedureTransCalling;
}

std::unordered_set<int> ResultFetcherStub::getTransCalledByProcedure(int s)
{
	++totalMethodCalls;
	++transCalledByProcedureCalls;
	return transCalledByProcedure;
}

std::unordered_set<std::pair<int, int>, pair_hash> ResultFetcherStub::getNextPairs()
{
	++totalMethodCalls;
	++nextPairsCalls;
	return nextPairs;
}

std::unordered_set<int> ResultFetcherStub::getNext(int s)
{
	++totalMethodCalls;
	++nextCalls;
	return next;
}

std::unordered_set<int> ResultFetcherStub::getPrevious(int s)
{
	++totalMethodCalls;
	++previousCalls;
	return previous;
}

std::unordered_set<int> ResultFetcherStub::getSubExpr(std::string expr)
{
	return patternWhitelist;
}

std::unordered_set<int> ResultFetcherStub::getExactExpr(std::string expr)
{
	return patternWhitelist;
}

// Pattern expression matching methods
/*std::unordered_set<int> ResultFetcherStub::getSubExpr(std::string expr) {
	++totalMethodCalls;
	++subExprCalls;
	return patternWhitelist.find(expr) != patternWhitelist.end();
}*/

//return type
bool ResultFetcherStub::isAssign(int s) {
	if (std::find(allAssigns.begin(), allAssigns.end(), s) != allAssigns.end()) {
		return true;
	}
	return false;
}
bool ResultFetcherStub::isIf(int s) {
	if (std::find(allIfs.begin(), allIfs.end(), s) != allIfs.end()) {
		return true;
	}
	return false;
}bool ResultFetcherStub::isWhile(int s) {
	if (std::find(allWhiles.begin(), allWhiles.end(), s) != allWhiles.end()) {
		return true;
	}
	return false;
}
bool ResultFetcherStub::isStmt(int s) {
	if (std::find(allStmts.begin(), allStmts.end(), s) != allStmts.end()) {
		return true;
	}
	return false;
}
bool ResultFetcherStub::isCall(int s) {
	if (std::find(allCalls.begin(), allCalls.end(), s) != allCalls.end()) {
		return true;
	}
	return false;
}
bool ResultFetcherStub::isProgLine(int s) {
	if (std::find(allStmts.begin(), allStmts.end(), s) != allStmts.end()) {
		return true;
	}
	return false;
}

std::unordered_set<int> ResultFetcherStub::getAllIfswithControlVar(int v)
{
	++totalMethodCalls;
	return allIfswithControlVar;
}

std::unordered_set<int> ResultFetcherStub::getAllWhileswithControlVar(int v)
{
	++totalMethodCalls;
	return allWhileswithControlVar;
}

std::unordered_set<std::pair<int, int>, pair_hash> ResultFetcherStub::getAllIfControlVarPairs()
{
	++totalMethodCalls;
	return allIfControlVarPairs;
}

std::unordered_set<std::pair<int, int>, pair_hash> ResultFetcherStub::getAllWhileControlVarPairs()
{
	++totalMethodCalls;
	return allWhileControlVarPairs;
}


