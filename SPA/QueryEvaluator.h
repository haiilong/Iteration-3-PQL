#pragma once

#include "ResultFetcher.h"
#include "ResultTable.h"
#include "QueryObject.h"
#include "Pair.h"

class QueryEvaluator {
public:
	QueryEvaluator(ResultFetcher &resultFetcher) : resultFetcher(resultFetcher), noResults(false) {};
	const std::list<std::string> evaluate(QueryObject &qo);
	void evaluateFollows(Clause clause);
	void evaluateTransFollows(Clause clause);
	void evaluateParent(Clause clause);
	void evaluateTransParent(Clause clause);
	void evaluateModifies(Clause clause);
	void evaluateUses(Clause clause);
	void evaluateCalls(Clause clause);
	void evaluateTransCalls(Clause clause);
	void evaluateNext(Clause clause);
	//void evaluateTransNext(Clause clause);
	void evaluatePatternAssign(Clause clause);
	void evaluatePatternIf(Clause clause);
	void evaluatePatternWhile(Clause clause);
	void removeWrongPairs(std::unordered_set<std::pair<int, int>, pair_hash>& tempSet, Clause::Type type1, Clause::Type type2);
	void removeWrongType(std::unordered_set<int>& tempSet, Clause::Type type);
	std::unordered_set<int> getAllFirstResult(std::unordered_set<std::pair<int, int>, pair_hash> tempSet);
	std::unordered_set<int> getAllSecondResult(std::unordered_set<std::pair<int, int>, pair_hash> tempSet);
	std::unordered_set<std::pair<int, int>, pair_hash> getIntersection1(std::unordered_set<std::pair<int, int>, pair_hash> list1, std::unordered_set<int> list2);
	std::unordered_set<int> getIntersection2(std::unordered_set<int> list1, std::unordered_set<int> list2);
	bool isSameType(Clause::Type type, int temp);
	std::string getStringRepresentation(int intRepresentation, Clause::Type type);
	int getIntRepresentation(std::string stringRepresentation, Clause::Type type);
	std::list<std::string> falseResult(Clause::Type type);

private:
	ResultFetcher &resultFetcher;
	ResultTable resultTable;
	void clear();
	bool noResults;

	/*
	ResultTableIndex resultTables;

	void filterType(const std::string &name, const QueryObject::argType &type);

	void evaluateClause(
		const QueryObject::clauseType clauseType,
		const std::vector<std::string> argNames,
		const std::vector<QueryObject::argType> argTypes);

	std::list<std::string> evaluateSingleSelect(
		const std::string name,
		const QueryObject::argType type
	);

	void tryEvaluateZeroLiteralClause(
		const std::vector<std::string> &argNames,
		const std::vector<QueryObject::argType> &argTypes,
		std::vector<std::pair<Result, Result>> (ResultFetcher::*getPairs)());
	void tryEvaluateFirstArgLiteralClause(
		const std::vector<std::string> &argNames,
		const std::vector<QueryObject::argType> &argTypes,
		Result(ResultFetcher::* litToResult)(std::string name),
		std::vector<Result>(ResultFetcher::* getSecondResult)(Result first));
	void tryEvaluateFirstArgLiteralClause(
		const std::vector<std::string> &argNames,
		const std::vector<QueryObject::argType> &argTypes,
		Result(ResultFetcher::* litToResult)(std::string name),
		Result(ResultFetcher::* getSecondResult)(Result first));
	void tryEvaluateBothLiteralClauseByFirst(
		const std::vector<std::string>& argNames,
		const std::vector<QueryObject::argType>& argTypes,
		Result(ResultFetcher::* arg0LitToResult)(std::string name),
		Result(ResultFetcher::* arg1LitToResult)(std::string name),
		std::vector<Result>(ResultFetcher::* getSecondResult)(Result first));
	void tryEvaluateBothLiteralClauseBySecond(
		const std::vector<std::string>& argNames,
		const std::vector<QueryObject::argType>& argTypes,
		Result(ResultFetcher::* arg0LitToResult)(std::string name),
		Result(ResultFetcher::* arg1LitToResult)(std::string name),
		Result(ResultFetcher::* getFirstResult)(Result second));
	void tryEvaluateSecondArgLiteralClause(
		const std::vector<std::string> &argNames,
		const std::vector<QueryObject::argType> &argTypes,
		Result(ResultFetcher::* litToResult)(std::string name),
		std::vector<Result>(ResultFetcher::* getFirstResult)(Result second));
	void tryEvaluateSecondArgLiteralClause(
		const std::vector<std::string> &argNames,
		const std::vector<QueryObject::argType> &argTypes,
		Result(ResultFetcher::* litToResult)(std::string name),
		Result(ResultFetcher::* getFirstResult)(Result second));
	void evaluatePatternClause(
		const std::vector<std::string> &argNames,
		const std::vector<QueryObject::argType> &argTypes);
		*/
};