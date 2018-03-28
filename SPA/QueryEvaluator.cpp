#include<unordered_set>
#include<set>

#include "Clause.h"
#include "QueryEvaluator.h"
#include "QueryObject.h"
#include "ResultFetcher.h"

struct FalseClauseException : public std::exception {
private:
	std::string message;
public:
	FalseClauseException(const std::string& message);
	virtual const char* what() const throw() {
		return message.c_str();
	}
};

FalseClauseException::FalseClauseException(const std::string& message) : message(message) {
}

const std::list<std::string> QueryEvaluator::evaluate(QueryObject & qo) {
	clear();
	std::vector<Clause> clause = qo.getClauses();
	Clause selectClause = qo.getSelectClause();
	size_t i;		
	list<int> result;
	bool atLeastOneSynonym = false;
	try {
		for (size_t i = 0; i < clause.size(); i++) {
			switch (clause.at(i).getRelation()) {
			case Clause::Relation::ASSIGN:
				evaluatePatternAssign(clause.at(i));
				break;
			case Clause::Relation::IF:
				evaluatePatternIf(clause.at(i));
				break;
			case Clause::Relation::WHILE:
				evaluatePatternWhile(clause.at(i));
				break;
			case Clause::Relation::AFFECTS:
				//evaluateAffects(clause);
			case Clause::Relation::AFFECTS_T:
				//evaluateTransAffects(clause);
			case Clause::Relation::CALLS:
				evaluateCalls(clause.at(i));
				break;
			case Clause::Relation::CALLS_T:
				evaluateTransCalls(clause.at(i));
				break;
			case Clause::Relation::FOLLOWS:
				evaluateFollows(clause.at(i));
				break;
			case Clause::Relation::FOLLOWS_T:
				evaluateTransFollows(clause.at(i));
				break;
			case Clause::Relation::MODIFIES:
				evaluateModifies(clause.at(i));
				break;
			case Clause::Relation::NEXT:
				evaluateNext(clause.at(i));
				break;
			case Clause::Relation::NEXT_T:
				//evaluateTransNext(clause.at(i));
				break;
			case Clause::Relation::PARENT:
				evaluateParent(clause.at(i));
				break;
			case Clause::Relation::PARENT_T:
				evaluateTransParent(clause.at(i));
				break;
			case Clause::Relation::USES:
				evaluateUses(clause.at(i));
				break;
			case Clause::Relation::WITH:
				//evaluateWith(clause.at(i));
				break;
			}
			if (noResults) {
				return falseResult(selectClause.getFirstType());
				break;
			}
		}
	}
	//false clause exception caught, return empty list
	catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
		return falseResult(selectClause.getFirstType());
	}
	if (selectClause.getFirstType() == Clause::Type::BOOLEAN) {
		return std::list<std::string>{"TRUE"};
	}
	//common synonym between select clause and clause
	for (i = 0; i < clause.size(); i++) {
		//if clause contains synonym
		if ((clause.at(i).getFirstType() != Clause::Type::IDENT && clause.at(i).getFirstType() != Clause::Type::INTEGER && clause.at(i).getFirstType() != Clause::Type::WILDCARD) || (clause.at(i).getSecondType() != Clause::Type::IDENT && clause.at(i).getSecondType() != Clause::Type::INTEGER && clause.at(i).getSecondType() != Clause::Type::WILDCARD)) {
			atLeastOneSynonym = true;
		}
		//if clause first arg is synonym and not equal to select clause synonym or if clause second arg is synonym and not equal to select clause synonym
		if ((clause.at(i).getFirstType() != Clause::Type::IDENT && clause.at(i).getFirstType() != Clause::Type::INTEGER && clause.at(i).getFirstType() != Clause::Type::WILDCARD) && clause.at(i).getFirstArg() == selectClause.getFirstArg() || (clause.at(i).getSecondType() != Clause::Type::IDENT && clause.at(i).getSecondType() != Clause::Type::INTEGER && clause.at(i).getSecondType() != Clause::Type::WILDCARD) && clause.at(i).getSecondArg() == selectClause.getFirstArg()) {
			break;
		}
	}
	//select synonym not in clauses, return all possible results of the synonym
	if(i == clause.size() || !atLeastOneSynonym) {
		unordered_set<int> resultSet;
		switch (selectClause.getFirstType()) {
		case Clause::Type::ASSIGN:
			resultSet = resultFetcher.getAllAssigns();
			break;
		case Clause::Type::IF:
			resultSet = resultFetcher.getAllIfs();
			break;
		case Clause::Type::WHILE:
			resultSet = resultFetcher.getAllWhiles();
			break;
		case Clause::Type::STMT:
		case Clause::Type::PROG_LINE:
			resultSet = resultFetcher.getAllStmts();
			break;
		case Clause::Type::PROCEDURE:
			resultSet = resultFetcher.getAllProcedures();
			break;
		case Clause::Type::CALL:
			resultSet = resultFetcher.getAllCalls();
			break;
		case Clause::Type::VARIABLE:
			resultSet = resultFetcher.getAllVariables();
			break;
		case Clause::Type::CONSTANT:
			resultSet = resultFetcher.getAllConstants();
			break;
		case Clause::Type::CALLPROC:
			resultSet = resultFetcher.getAllCalledProcedure();
			break;
		case Clause::Type::BOOLEAN:
			resultSet = { 1 };
			break;
		}
		std::unordered_set<int>::iterator it;
		for (it = resultSet.begin(); it != resultSet.end(); ++it) {
			result.push_back(*it);
		}
	}
	else {
		result = resultTable.getColumn(selectClause.getFirstArg());
		if (selectClause.getFirstType() == Clause::Type::CALLPROC) {
			std::list<int>::iterator it;
			std::list<int> resultProc;
			for (it = result.begin(); it != result.end(); ++it) {
				resultProc.push_back(resultFetcher.getProcinCallStmt(*it));
			}
		}
	}
	std::list<int>::iterator it;
	std::list<std::string> stringResult;
	for (it = result.begin(); it != result.end(); ++it) {
		stringResult.push_back(getStringRepresentation(*it, selectClause.getFirstType()));
	}
	return stringResult;

	/*//empty list returned
	if ((resultTable.noResults() && atLeastOneSynonym)) {
		if (selectClause.getFirstType() == Clause::Type::BOOLEAN) {
			return std::list<std::string>{"FALSE"};
		}
		return std::list<std::string>{};
	}
	else {
		if (selectClause.getFirstType() == Clause::Type::BOOLEAN) {
			return std::list<std::string>{"TRUE"};
		}
		std::list<int>::iterator it;
		std::list<std::string> stringResult;
		if (selectClause.getFirstType() == Clause::Type::CALLPROC){
			if (i == clause.size()) {
				for (it = result.begin(); it != result.end(); ++it) {
					stringResult.push_back(getStringRepresentation(*it, Clause::Type::PROCEDURE));
				}
				return stringResult;
			}
			else {
				for (it = result.begin(); it != result.end(); ++it) {
					stringResult.push_back(getStringRepresentation(resultFetcher.getProcinCallStmt(*it), Clause::Type::PROCEDURE));
				}
				return stringResult;
			}
		}
		for (it = result.begin(); it != result.end(); ++it) {
			stringResult.push_back(getStringRepresentation(*it, selectClause.getFirstType()));
		}
		return stringResult;
	}
	*/
}

std::list<std::string> QueryEvaluator::falseResult(Clause::Type type) {
	if (type == Clause::Type::BOOLEAN) {
		return std::list<std::string>{"FALSE"};
	}
	else {
		return std::list<std::string>{};
	}
}


void QueryEvaluator::evaluateFollows(Clause clause) {
	int firstArg = getIntRepresentation(clause.getFirstArg(), clause.getFirstType());
	int secondArg = getIntRepresentation(clause.getSecondArg(), clause.getSecondType());
	//both args int
	if (clause.getFirstType() == Clause::Type::INTEGER && clause.getSecondType()==Clause::Type::INTEGER) {
		std::unordered_set<std::pair<int, int>, pair_hash> temp = resultFetcher.getFollowsPairs();
		std::pair<int, int> checkPair(firstArg, secondArg);		
		if (temp.find(checkPair) == temp.end()) {
			throw FalseClauseException("False Clause, empty list returned");
		}
		return;
	}
	//first arg int, second arg _
	else if (clause.getFirstType() == Clause::Type::INTEGER && clause.getSecondType() == Clause::Type::WILDCARD) {
		int temp = resultFetcher.getFollowing(firstArg);
		if (temp == -1) {
			throw FalseClauseException("False Clause, empty list returned");
		}
		return;
	}
	//first arg _, second arg int
	else if (clause.getFirstType() == Clause::Type::WILDCARD && clause.getSecondType() == Clause::Type::INTEGER) {
		int temp = resultFetcher.getFollowedBy(secondArg);
		if (temp == -1) {
			throw FalseClauseException("False Clause, empty list returned");
		}
		return;
	}
	//first arg _, second arg _
	else if (clause.getFirstType() == Clause::Type::WILDCARD && clause.getSecondType() == Clause::Type::WILDCARD) {
		std::unordered_set<std::pair<int, int>, pair_hash> temp = resultFetcher.getFollowsPairs();
		if (temp.empty()) {
			throw FalseClauseException("False Clause, empty list returned");
		}
		return;
	}
	//first arg int, second arg synonym
	else if (clause.getFirstType() == Clause::Type::INTEGER) {
		int temp = resultFetcher.getFollowing(firstArg);
		std::unordered_set<int> synonymList;
		if (isSameType(clause.getSecondType(), temp)) {
			synonymList.insert(temp);
			resultTable.mergeOneSynonym(clause.getSecondArg(), synonymList);
		}
	}
	//first arg synonym, second arg int
	else if (clause.getSecondType() == Clause::Type::INTEGER) {
		int temp = resultFetcher.getFollowedBy(secondArg);
		std::unordered_set<int> synonymList;
		if (isSameType(clause.getFirstType(), temp)) {
			synonymList.insert(temp);
			resultTable.mergeOneSynonym(clause.getFirstArg(), synonymList);
		}
	}
	//first arg _, second arg synonym
	else if (clause.getFirstType() == Clause::Type::WILDCARD) {
		std::unordered_set<std::pair<int, int>, pair_hash> temp = resultFetcher.getFollowsPairs();
		std::unordered_set<int> synonymList = getAllSecondResult(temp);
		removeWrongType(synonymList, clause.getSecondType());
		resultTable.mergeOneSynonym(clause.getSecondArg(), synonymList);
	}
	//first arg synonym, second arg _
	else if (clause.getSecondType() == Clause::Type::WILDCARD) {
		std::unordered_set<std::pair<int, int>, pair_hash> temp = resultFetcher.getFollowsPairs();
		std::unordered_set<int> synonymList = getAllFirstResult(temp);
		removeWrongType(synonymList, clause.getFirstType());
		resultTable.mergeOneSynonym(clause.getFirstArg(), synonymList);
	}
	else { //both args synonym
		std::unordered_set<std::pair<int, int>, pair_hash> synonymList = resultFetcher.getFollowsPairs();
		removeWrongPairs(synonymList, clause.getFirstType(), clause.getSecondType());
		resultTable.mergeTwoSynonym(clause.getFirstArg(), clause.getSecondArg(), synonymList);
	}
	//check if table is empty
	if (resultTable.noResults()) {
		noResults = true;
	}
}


void QueryEvaluator::evaluateTransFollows(Clause clause) {
	int firstArg = getIntRepresentation(clause.getFirstArg(), clause.getFirstType());
	int secondArg = getIntRepresentation(clause.getSecondArg(), clause.getSecondType());
	//first arg int, second arg int
	if (clause.getFirstType() == Clause::Type::INTEGER && clause.getSecondType() == Clause::Type::INTEGER) {
		std::unordered_set<std::pair<int, int>, pair_hash> temp = resultFetcher.getTransFollowsPairs();
		std::pair<int, int> checkPair(firstArg, secondArg);
		if (temp.find(checkPair) == temp.end()) {
			throw FalseClauseException("False Clause, empty list returned");
		}
		return;
	}
	//first arg int, second arg _
	else if (clause.getFirstType() == Clause::Type::INTEGER && clause.getSecondType() == Clause::Type::WILDCARD) {
		std::unordered_set<int> temp = resultFetcher.getTransFollowing(firstArg);
		if (temp.empty()) {
			throw FalseClauseException("False Clause, empty list returned");
		}
		return;
	}
	//first arg _, second arg int
	else if (clause.getFirstType() == Clause::Type::WILDCARD && clause.getSecondType() == Clause::Type::INTEGER) {
		std::unordered_set<int> temp = resultFetcher.getTransFollowedBy(secondArg);
		if (temp.empty()) {
			throw FalseClauseException("False Clause, empty list returned");
		}
		return;
	}
	//first arg _, second arg _
	else if (clause.getFirstType() == Clause::Type::WILDCARD && clause.getSecondType() == Clause::Type::WILDCARD) {
		std::unordered_set<std::pair<int, int>, pair_hash> temp = resultFetcher.getTransFollowsPairs();
		if (temp.empty()) {
			throw FalseClauseException("False Clause, empty list returned");
		}
		return;
	}
	//first arg int, second arg synonym
	else if (clause.getFirstType() == Clause::Type::INTEGER) {
		std::unordered_set<int> synonymList = resultFetcher.getTransFollowing(firstArg);
		removeWrongType(synonymList, clause.getSecondType());
		resultTable.mergeOneSynonym(clause.getSecondArg(), synonymList);
	}
	//first arg synonym, second arg int
	else if (clause.getSecondType() == Clause::Type::INTEGER) {
		std::unordered_set<int> synonymList = resultFetcher.getTransFollowedBy(secondArg);
		removeWrongType(synonymList, clause.getFirstType());
		resultTable.mergeOneSynonym(clause.getFirstArg(), synonymList); 
	}
	//first arg _, second arg synonym
	else if (clause.getFirstType() == Clause::Type::WILDCARD) {
		std::unordered_set<std::pair<int, int>, pair_hash> temp = resultFetcher.getTransFollowsPairs();
		std::unordered_set<int> synonymList = getAllSecondResult(temp);
		removeWrongType(synonymList, clause.getSecondType());
		resultTable.mergeOneSynonym(clause.getSecondArg(), synonymList);
	}
	//first arg synonym, second arg _
	else if (clause.getSecondType() == Clause::Type::WILDCARD) {
		std::unordered_set<std::pair<int, int>, pair_hash> temp = resultFetcher.getTransFollowsPairs();
		std::unordered_set<int> synonymList = getAllFirstResult(temp);
		removeWrongType(synonymList, clause.getFirstType());
		resultTable.mergeOneSynonym(clause.getFirstArg(), synonymList);
	}
	else { //both args synonym
		std::unordered_set<std::pair<int, int>, pair_hash> synonymList = resultFetcher.getTransFollowsPairs();
		removeWrongPairs(synonymList, clause.getFirstType(), clause.getSecondType());
		resultTable.mergeTwoSynonym(clause.getFirstArg(), clause.getSecondArg(), synonymList);
	}
	//check if table is empty
	if (resultTable.noResults()) {
		noResults = true;
	}
}

void QueryEvaluator::evaluateParent(Clause clause) {
	int firstArg = getIntRepresentation(clause.getFirstArg(), clause.getFirstType());
	int secondArg = getIntRepresentation(clause.getSecondArg(), clause.getSecondType());
	//both args int
	if (clause.getFirstType() == Clause::Type::INTEGER && clause.getSecondType() == Clause::Type::INTEGER) {
		std::unordered_set<std::pair<int, int>, pair_hash> temp = resultFetcher.getParentPairs();
		std::pair<int, int> checkPair(firstArg, secondArg);		
		if (temp.find(checkPair) == temp.end()) {
			throw FalseClauseException("False Clause, empty list returned");
		}
		return;
	}
	//first arg int, second arg _
	else if (clause.getFirstType() == Clause::Type::INTEGER && clause.getSecondType() == Clause::Type::WILDCARD) {
		std::unordered_set<int> temp = resultFetcher.getChildrenOf(firstArg);
		if (temp.empty()) {
			throw FalseClauseException("False Clause, empty list returned");
		}
		return;
	}
	//first arg _, second arg int
	else if (clause.getFirstType() == Clause::Type::WILDCARD && clause.getSecondType() == Clause::Type::INTEGER) {
		int temp = resultFetcher.getParentOf(secondArg);
		if (temp == -1) {
			throw FalseClauseException("False Clause, empty list returned");
		}
		return;
	}
	//first arg _, second arg _
	else if (clause.getFirstType() == Clause::Type::WILDCARD && clause.getSecondType() == Clause::Type::WILDCARD) {
		std::unordered_set<std::pair<int, int>, pair_hash> temp = resultFetcher.getParentPairs();
		if (temp.empty()) {
			throw FalseClauseException("False Clause, empty list returned");
		}
		return;
	}
	//first arg int, second arg synonym
	else if (clause.getFirstType() == Clause::Type::INTEGER) {
		std::unordered_set<int> synonymList = resultFetcher.getChildrenOf(firstArg);
		removeWrongType(synonymList, clause.getSecondType());
		resultTable.mergeOneSynonym(clause.getSecondArg(), synonymList);
	}
	//first arg synonym, second arg int
	else if (clause.getSecondType() == Clause::Type::INTEGER) {
		int temp = resultFetcher.getParentOf(secondArg);
		std::unordered_set<int> synonymList;
		if (isSameType(clause.getFirstType(), temp)) {
			synonymList.insert(temp);
			resultTable.mergeOneSynonym(clause.getFirstArg(), synonymList);
		}
	}
	//first arg _, second arg synonym
	else if (clause.getFirstType() == Clause::Type::WILDCARD) {
		std::unordered_set<std::pair<int, int>, pair_hash> temp = resultFetcher.getParentPairs();
		std::unordered_set<int> synonymList = getAllSecondResult(temp);
		removeWrongType(synonymList, clause.getSecondType());
		resultTable.mergeOneSynonym(clause.getSecondArg(), synonymList);
	}
	//first arg synonym, second arg _
	else if (clause.getSecondType() == Clause::Type::WILDCARD) {
		std::unordered_set<std::pair<int, int>, pair_hash> temp = resultFetcher.getParentPairs();
		std::unordered_set<int> synonymList = getAllFirstResult(temp);
		removeWrongType(synonymList, clause.getFirstType());
		resultTable.mergeOneSynonym(clause.getFirstArg(), synonymList);
	}
	else { //both args synonym
		std::unordered_set<std::pair<int, int>, pair_hash> synonymList = resultFetcher.getParentPairs();
		removeWrongPairs(synonymList, clause.getFirstType(), clause.getSecondType());
		resultTable.mergeTwoSynonym(clause.getFirstArg(), clause.getSecondArg(), synonymList);
	}
	//check if table is empty
	if (resultTable.noResults()) {
		noResults = true;
	}
}


void QueryEvaluator::evaluateTransParent(Clause clause) {
	int firstArg = getIntRepresentation(clause.getFirstArg(), clause.getFirstType());
	int secondArg = getIntRepresentation(clause.getSecondArg(), clause.getSecondType());
	//first arg int, second arg int
	if (clause.getFirstType() == Clause::Type::INTEGER && clause.getSecondType() == Clause::Type::INTEGER) {
		std::unordered_set<std::pair<int, int>, pair_hash> temp = resultFetcher.getTransParentPairs();
		std::pair<int, int> checkPair(firstArg, secondArg);
		if (temp.find(checkPair) == temp.end()) {
			throw FalseClauseException("False Clause, empty list returned");
		}
		return;
	}
	//first arg int, second arg _
	else if (clause.getFirstType() == Clause::Type::INTEGER && clause.getSecondType() == Clause::Type::WILDCARD) {
		std::unordered_set<int> temp = resultFetcher.getTransChildrenOf(firstArg);
		if (temp.empty()) {
			throw FalseClauseException("False Clause, empty list returned");
		}
		return;
	}
	//first arg _, second arg int
	else if (clause.getFirstType() == Clause::Type::WILDCARD && clause.getSecondType() == Clause::Type::INTEGER) {
		std::unordered_set<int> temp = resultFetcher.getTransParentOf(secondArg);
		if (temp.empty()) {
			throw FalseClauseException("False Clause, empty list returned");
		}
		return;
	}
	//first arg _, second arg _
	else if (clause.getFirstType() == Clause::Type::WILDCARD && clause.getSecondType() == Clause::Type::WILDCARD) {
		std::unordered_set<std::pair<int, int>, pair_hash> temp = resultFetcher.getTransParentPairs();
		if (temp.empty()) {
			throw FalseClauseException("False Clause, empty list returned");
		}
		return;
	}
	//first arg int, second arg synonym
	else if (clause.getFirstType() == Clause::Type::INTEGER) {
		std::unordered_set<int> synonymList = resultFetcher.getTransChildrenOf(firstArg);
		removeWrongType(synonymList, clause.getSecondType());
		resultTable.mergeOneSynonym(clause.getSecondArg(), synonymList);
	}
	//first arg synonym, second arg int
	else if (clause.getSecondType() == Clause::Type::INTEGER) {
		std::unordered_set<int> synonymList = resultFetcher.getTransParentOf(secondArg);
		removeWrongType(synonymList, clause.getFirstType());
		resultTable.mergeOneSynonym(clause.getFirstArg(), synonymList);
	}
	//first arg _, second arg synonym
	else if (clause.getFirstType() == Clause::Type::WILDCARD) {
		std::unordered_set<std::pair<int, int>, pair_hash> temp = resultFetcher.getTransParentPairs();
		std::unordered_set<int> synonymList = getAllSecondResult(temp);
		removeWrongType(synonymList, clause.getSecondType());
		resultTable.mergeOneSynonym(clause.getSecondArg(), synonymList);
	}
	//first arg synonym, second arg _
	else if (clause.getSecondType() == Clause::Type::WILDCARD) {
		std::unordered_set<std::pair<int, int>, pair_hash> temp = resultFetcher.getTransParentPairs();
		std::unordered_set<int> synonymList = getAllFirstResult(temp);
		removeWrongType(synonymList, clause.getFirstType());
		resultTable.mergeOneSynonym(clause.getFirstArg(), synonymList);
	}
	else { //both args synonym
		std::unordered_set<std::pair<int, int>, pair_hash> synonymList = resultFetcher.getTransParentPairs();
		removeWrongPairs(synonymList, clause.getFirstType(), clause.getSecondType());
		resultTable.mergeTwoSynonym(clause.getFirstArg(), clause.getSecondArg(), synonymList);
	}
	//check if table is empty
	if (resultTable.noResults()) {
		noResults = true;
	}
}

void QueryEvaluator::evaluateModifies(Clause clause) {
	int firstArg = getIntRepresentation(clause.getFirstArg(), clause.getFirstType());
	int secondArg = getIntRepresentation(clause.getSecondArg(), clause.getSecondType());
	//first arg int, second arg ident
	if (clause.getFirstType() == Clause::Type::INTEGER && clause.getSecondType() == Clause::Type::IDENT) {
		std::unordered_set<int> temp = resultFetcher.getModifiedByStmt(firstArg);
		if (temp.find(secondArg) == temp.end()) {
			throw FalseClauseException("False Clause, empty list returned");
		}
		return;
	}
	//first arg int, second arg _
	else if (clause.getFirstType() == Clause::Type::INTEGER && clause.getSecondType() == Clause::Type::WILDCARD) {
		std::unordered_set<int> temp = resultFetcher.getModifiedByStmt(firstArg);
		if (temp.empty()) {
			throw FalseClauseException("False Clause, empty list returned");
		}
		return;
	}
	//first arg int, second arg synonym(variable)
	else if (clause.getFirstType() == Clause::Type::INTEGER) {
		std::unordered_set<int> synonymList = resultFetcher.getModifiedByStmt(firstArg);
		resultTable.mergeOneSynonym(clause.getSecondArg(), synonymList);
	}
	//first arg ident(proc name), second arg ident
	else if (clause.getFirstType() == Clause::Type::IDENT && clause.getSecondType() == Clause::Type::IDENT) {
		std::unordered_set<int> temp = resultFetcher.getModifiedByProcedure(firstArg);
		if (temp.find(secondArg) == temp.end()) {
			throw FalseClauseException("False Clause, empty list returned");
		}
		return;
	}
	//first arg ident(proc name), second arg _
	else if (clause.getFirstType() == Clause::Type::IDENT && clause.getSecondType() == Clause::Type::WILDCARD) {
		std::unordered_set<int> temp = resultFetcher.getModifiedByProcedure(firstArg);
		if (temp.empty()) {
			throw FalseClauseException("False Clause, empty list returned");
		}
		return;
	}
	//first arg ident(proc name), second arg synonym(variable)
	else if (clause.getFirstType() == Clause::Type::IDENT) {
		std::unordered_set<int> synonymList = resultFetcher.getModifiedByProcedure(firstArg);
		resultTable.mergeOneSynonym(clause.getSecondArg(), synonymList);
	}
	//first arg proc synonym, second arg ident
	else if (clause.getFirstType() == Clause::Type::PROCEDURE && clause.getSecondType() == Clause::Type::IDENT) {
		std::unordered_set<int> synonymList = resultFetcher.getProcedureModifying(secondArg);
		if (synonymList.empty()) {
			throw FalseClauseException("False Clause, empty list returned");
		}
		resultTable.mergeOneSynonym(clause.getFirstArg(), synonymList);
	}
	//first arg proc synonym, second arg _
	else if (clause.getFirstType() == Clause::Type::PROCEDURE && clause.getSecondType() == Clause::Type::WILDCARD) {
		std::unordered_set<std::pair<int, int>, pair_hash> temp = resultFetcher.getModifiesProcedurePairs();
		std::unordered_set<int> synonymList = getAllFirstResult(temp);
		resultTable.mergeOneSynonym(clause.getFirstArg(), synonymList);
	}
	//first arg proc synonym, second arg synonym(variable)
	else if (clause.getFirstType() == Clause::Type::PROCEDURE) {
		std::unordered_set<std::pair<int, int>, pair_hash> synonymList = resultFetcher.getModifiesProcedurePairs();
		resultTable.mergeTwoSynonym(clause.getFirstArg(), clause.getSecondArg(), synonymList);
	}
	//first arg synonym, second arg ident
	else if (clause.getSecondType() == Clause::Type::IDENT) {
		std::unordered_set<int> synonymList = resultFetcher.getStmtModifying(secondArg);
		if (synonymList.empty()) {
			throw FalseClauseException("False Clause, empty list returned");
		}
		removeWrongType(synonymList, clause.getFirstType());
		resultTable.mergeOneSynonym(clause.getFirstArg(), synonymList);
	}
	//first arg synonym, second arg _
	else if (clause.getSecondType() == Clause::Type::WILDCARD) {
		std::unordered_set<std::pair<int, int>, pair_hash> temp = resultFetcher.getModifiesStmtPairs();
		std::unordered_set<int> synonymList = getAllFirstResult(temp);
		removeWrongType(synonymList, clause.getFirstType());
		resultTable.mergeOneSynonym(clause.getFirstArg(), synonymList);
	}
	else { //first arg synonym, second arg synonym(variable)
		std::unordered_set<std::pair<int, int>, pair_hash> synonymList = resultFetcher.getModifiesStmtPairs();
		removeWrongPairs(synonymList, clause.getFirstType(), clause.getSecondType());
		resultTable.mergeTwoSynonym(clause.getFirstArg(), clause.getSecondArg(), synonymList);
	}
	//check if table is empty
	if (resultTable.noResults()) {
		noResults = true;
	}
}

void QueryEvaluator::evaluateUses(Clause clause) {
	int firstArg = getIntRepresentation(clause.getFirstArg(), clause.getFirstType());
	int secondArg = getIntRepresentation(clause.getSecondArg(), clause.getSecondType());
	//first arg int, second arg ident
	if (clause.getFirstType() == Clause::Type::INTEGER && clause.getSecondType() == Clause::Type::IDENT) {
		std::unordered_set<int> temp = resultFetcher.getUsedByStmt(firstArg);
		if (temp.find(secondArg) == temp.end()) {
			throw FalseClauseException("False Clause, empty list returned");
		}
		return;
	}
	//first arg int, second arg _
	else if (clause.getFirstType() == Clause::Type::INTEGER && clause.getSecondType() == Clause::Type::WILDCARD) {
		std::unordered_set<int> temp = resultFetcher.getUsedByStmt(firstArg);
		if (temp.empty()) {
			throw FalseClauseException("False Clause, empty list returned");
		}
		return;
	}
	//first arg int, second arg synonym(variable)
	else if (clause.getFirstType() == Clause::Type::INTEGER) {
		std::unordered_set<int> synonymList = resultFetcher.getUsedByStmt(firstArg);
		resultTable.mergeOneSynonym(clause.getSecondArg(), synonymList);
	}
	//first arg ident(proc name), second arg ident
	else if (clause.getFirstType() == Clause::Type::IDENT && clause.getSecondType() == Clause::Type::IDENT) {
		std::unordered_set<int> temp = resultFetcher.getUsedByProcedure(firstArg);
		if (temp.find(secondArg) == temp.end()) {
			throw FalseClauseException("False Clause, empty list returned");
		}
		return;
	}
	//first arg ident(proc name), second arg _
	else if (clause.getFirstType() == Clause::Type::IDENT && clause.getSecondType() == Clause::Type::WILDCARD) {
		std::unordered_set<int> temp = resultFetcher.getUsedByProcedure(firstArg);
		if (temp.empty()) {
			throw FalseClauseException("False Clause, empty list returned");
		}
		return;
	}
	//first arg ident(proc name), second arg synonym(variable)
	else if (clause.getFirstType() == Clause::Type::IDENT) {
		std::unordered_set<int> synonymList = resultFetcher.getUsedByProcedure(firstArg);
		resultTable.mergeOneSynonym(clause.getSecondArg(), synonymList);
	}
	//first arg proc synonym, second arg ident
	else if (clause.getFirstType() == Clause::Type::PROCEDURE && clause.getSecondType() == Clause::Type::IDENT) {
		std::unordered_set<int> synonymList = resultFetcher.getProcedureUsing(secondArg);
		if (synonymList.empty()) {
			throw FalseClauseException("False Clause, empty list returned");
		}
		resultTable.mergeOneSynonym(clause.getFirstArg(), synonymList);
	}
	//first arg proc synonym, second arg _
	else if (clause.getFirstType() == Clause::Type::PROCEDURE && clause.getSecondType() == Clause::Type::WILDCARD) {
		std::unordered_set<std::pair<int, int>, pair_hash> temp = resultFetcher.getUsesProcedurePairs();
		std::unordered_set<int> synonymList = getAllFirstResult(temp);
		resultTable.mergeOneSynonym(clause.getFirstArg(), synonymList);
	}
	//first arg proc synonym, second arg synonym(variable)
	else if (clause.getFirstType() == Clause::Type::PROCEDURE) {
		std::unordered_set<std::pair<int, int>, pair_hash> synonymList = resultFetcher.getUsesProcedurePairs();
		resultTable.mergeTwoSynonym(clause.getFirstArg(), clause.getSecondArg(), synonymList);
	}
	//first arg synonym, second arg ident
	else if (clause.getSecondType() == Clause::Type::IDENT) {
		std::unordered_set<int> synonymList = resultFetcher.getStmtUsing(secondArg);
		if (synonymList.empty()) {
			throw FalseClauseException("False Clause, empty list returned");
		}
		removeWrongType(synonymList, clause.getFirstType());
		resultTable.mergeOneSynonym(clause.getFirstArg(), synonymList);
	}
	//first arg synonym, second arg _
	else if (clause.getSecondType() == Clause::Type::WILDCARD) {
		std::unordered_set<std::pair<int, int>, pair_hash> temp = resultFetcher.getUsesStmtPairs();
		std::unordered_set<int> synonymList = getAllFirstResult(temp);
		removeWrongType(synonymList, clause.getFirstType());
		resultTable.mergeOneSynonym(clause.getFirstArg(), synonymList);
	}
	else { //first arg synonym, second arg synonym(variable)
		std::unordered_set<std::pair<int, int>, pair_hash> synonymList = resultFetcher.getUsesStmtPairs();
		for (std::unordered_set<std::pair<int, int>, pair_hash>::iterator it = synonymList.begin(); it != synonymList.end(); ++it) {
		}
		removeWrongPairs(synonymList, clause.getFirstType(), clause.getSecondType());
		resultTable.mergeTwoSynonym(clause.getFirstArg(), clause.getSecondArg(), synonymList);
	}
	//check if table is empty
	if (resultTable.noResults()) {
		noResults = true;
	}
}

void QueryEvaluator::evaluateCalls(Clause clause) {
	int firstArg = getIntRepresentation(clause.getFirstArg(), clause.getFirstType());
	int secondArg = getIntRepresentation(clause.getSecondArg(), clause.getSecondType());
	//first arg _, second arg _
	if (clause.getFirstType() == Clause::Type::WILDCARD && clause.getSecondType() == Clause::Type::WILDCARD) {
		std::unordered_set<std::pair<int, int>, pair_hash> temp = resultFetcher.getCallsProcedurePairs();
		if (temp.empty()) {
			throw FalseClauseException("False Clause, empty list returned");
		}
		return;
	}
	//first arg ident(proc name), second arg _
	else if (clause.getFirstType() == Clause::Type::IDENT && clause.getSecondType() == Clause::Type::WILDCARD) {
		std::unordered_set<int> temp = resultFetcher.getCalledByProcedure(firstArg);
		if (temp.empty()) {
			throw FalseClauseException("False Clause, empty list returned");
		}
		return;
	}
	//first arg procedure synonym, second arg _
	else if (clause.getFirstType() == Clause::Type::PROCEDURE && clause.getSecondType() == Clause::Type::WILDCARD) {
		std::unordered_set<std::pair<int, int>, pair_hash> temp = resultFetcher.getCallsProcedurePairs();
		std::unordered_set<int> synonymList = getAllFirstResult(temp);
		resultTable.mergeOneSynonym(clause.getFirstArg(), synonymList);
	}
	//first arg _, second arg ident(proc name)
	else if (clause.getFirstType() == Clause::Type::WILDCARD && clause.getSecondType() == Clause::Type::IDENT) {
		std::unordered_set<int> temp = resultFetcher.getProcedureCalling(secondArg);
		if (temp.empty()) {
			throw FalseClauseException("False Clause, empty list returned");
		}
		return;
	}
	//first arg ident(proc name), second arg ident(proc name)
	else if (clause.getFirstType() == Clause::Type::IDENT && clause.getSecondType() == Clause::Type::IDENT) {
		std::unordered_set<int> temp = resultFetcher.getProcedureCalling(secondArg);
		if (temp.find(firstArg) == temp.end()) {
			throw FalseClauseException("False Clause, empty list returned");
		}
		return;
	}
	//first arg procedure synonym, second arg ident(proc name)
	else if (clause.getFirstType() == Clause::Type::PROCEDURE && clause.getSecondType() == Clause::Type::IDENT) {
		std::unordered_set<int> synonymList = resultFetcher.getProcedureCalling(secondArg);
		resultTable.mergeOneSynonym(clause.getFirstArg(), synonymList);
	}
	//first arg _, second arg procedure synonym
	else if (clause.getFirstType() == Clause::Type::WILDCARD && clause.getSecondType() == Clause::Type::PROCEDURE) {
		std::unordered_set<std::pair<int, int>, pair_hash> temp = resultFetcher.getCallsProcedurePairs();
		std::unordered_set<int> synonymList = getAllSecondResult(temp);
		resultTable.mergeOneSynonym(clause.getSecondArg(), synonymList);
	}
	//first arg ident, second arg procedure synonym
	else if (clause.getFirstType() == Clause::Type::IDENT && clause.getSecondType() == Clause::Type::PROCEDURE) {
		std::unordered_set<int> synonymList = resultFetcher.getCalledByProcedure(firstArg);
		resultTable.mergeOneSynonym(clause.getSecondArg(), synonymList);
	}
	//first arg procedure synonym, second arg procedure synonym
	else{ //if (clause.getFirstType() == Clause::Type::PROCEDURE && clause.getSecondType() == Clause::Type::PROCEDURE)
		std::unordered_set<std::pair<int, int>, pair_hash> synonymList = resultFetcher.getCallsProcedurePairs();
		resultTable.mergeTwoSynonym(clause.getFirstArg(), clause.getSecondArg(), synonymList);
	}
	//check if table is empty
	if (resultTable.noResults()) {
		noResults = true;
	}
}

void QueryEvaluator::evaluateTransCalls(Clause clause) {
	int firstArg = getIntRepresentation(clause.getFirstArg(), clause.getFirstType());
	int secondArg = getIntRepresentation(clause.getSecondArg(), clause.getSecondType());
	//first arg _, second arg _
	if (clause.getFirstType() == Clause::Type::WILDCARD && clause.getSecondType() == Clause::Type::WILDCARD) {
		std::unordered_set<std::pair<int, int>, pair_hash> temp = resultFetcher.getTransCallsProcedurePairs();
		if (temp.empty()) {
			throw FalseClauseException("False Clause, empty list returned");
		}
		return;
	}
	//first arg ident(proc name), second arg _
	else if (clause.getFirstType() == Clause::Type::IDENT && clause.getSecondType() == Clause::Type::WILDCARD) {
		std::unordered_set<int> temp = resultFetcher.getTransCalledByProcedure(firstArg);
		if (temp.empty()) {
			throw FalseClauseException("False Clause, empty list returned");
		}
		return;
	}
	//first arg procedure synonym, second arg _
	else if (clause.getFirstType() == Clause::Type::PROCEDURE && clause.getSecondType() == Clause::Type::WILDCARD) {
		std::unordered_set<std::pair<int, int>, pair_hash> temp = resultFetcher.getTransCallsProcedurePairs();
		std::unordered_set<int> synonymList = getAllFirstResult(temp);
		resultTable.mergeOneSynonym(clause.getFirstArg(), synonymList);
	}
	//first arg _, second arg ident(proc name)
	else if (clause.getFirstType() == Clause::Type::WILDCARD && clause.getSecondType() == Clause::Type::IDENT) {
		std::unordered_set<int> temp = resultFetcher.getProcedureTransCalling(secondArg);
		if (temp.empty()) {
			throw FalseClauseException("False Clause, empty list returned");
		}
		return;
	}
	//first arg ident(proc name), second arg ident(proc name)
	else if (clause.getFirstType() == Clause::Type::IDENT && clause.getSecondType() == Clause::Type::IDENT) {
		std::unordered_set<int> temp = resultFetcher.getProcedureTransCalling(secondArg);
		if (temp.find(firstArg) == temp.end()) {
			throw FalseClauseException("False Clause, empty list returned");
		}
		return;
	}
	//first arg procedure synonym, second arg ident(proc name)
	else if (clause.getFirstType() == Clause::Type::PROCEDURE && clause.getSecondType() == Clause::Type::IDENT) {
		std::unordered_set<int> synonymList = resultFetcher.getProcedureTransCalling(secondArg);
		resultTable.mergeOneSynonym(clause.getFirstArg(), synonymList);
	}
	//first arg _, second arg procedure synonym
	else if (clause.getFirstType() == Clause::Type::WILDCARD && clause.getSecondType() == Clause::Type::PROCEDURE) {
		std::unordered_set<std::pair<int, int>, pair_hash> temp = resultFetcher.getTransCallsProcedurePairs();
		std::unordered_set<int> synonymList = getAllSecondResult(temp);
		resultTable.mergeOneSynonym(clause.getSecondArg(), synonymList);
	}
	//first arg ident, second arg procedure synonym
	else if (clause.getFirstType() == Clause::Type::IDENT && clause.getSecondType() == Clause::Type::PROCEDURE) {
		std::unordered_set<int> synonymList = resultFetcher.getTransCalledByProcedure(firstArg);
		resultTable.mergeOneSynonym(clause.getSecondArg(), synonymList);

	}
	//first arg procedure synonym, second arg procedure synonym
	else { //if (clause.getFirstType() == Clause::Type::PROCEDURE && clause.getSecondType() == Clause::Type::PROCEDURE)
		std::unordered_set<std::pair<int, int>, pair_hash> synonymList = resultFetcher.getTransCallsProcedurePairs();
		resultTable.mergeTwoSynonym(clause.getFirstArg(), clause.getSecondArg(), synonymList);
	}
	//check if table is empty
	if (resultTable.noResults()) {
		noResults = true;
	}
}

void QueryEvaluator::evaluateNext(Clause clause) {
	int firstArg = getIntRepresentation(clause.getFirstArg(), clause.getFirstType());
	int secondArg = getIntRepresentation(clause.getSecondArg(), clause.getSecondType());
	//first arg _, second arg _
	if (clause.getFirstType() == Clause::Type::WILDCARD && clause.getSecondType() == Clause::Type::WILDCARD) {
		std::unordered_set<std::pair<int, int>, pair_hash> temp = resultFetcher.getNextPairs();
		if (temp.empty()) {
			throw FalseClauseException("False Clause, empty list returned");
		}
		return;
	}
	//first arg int, second arg _
	else if (clause.getFirstType() == Clause::Type::INTEGER && clause.getSecondType() == Clause::Type::WILDCARD) {
		std::unordered_set<int> temp = resultFetcher.getNext(firstArg);
		if (temp.empty()) {
			throw FalseClauseException("False Clause, empty list returned");
		}
		return;
	}
	//first arg synonym, second arg _
	else if (clause.getSecondType() == Clause::Type::WILDCARD) {
		std::unordered_set<std::pair<int, int>, pair_hash> temp = resultFetcher.getNextPairs();
		std::unordered_set<int> synonymList = getAllFirstResult(temp);
		removeWrongType(synonymList, clause.getFirstType());
		resultTable.mergeOneSynonym(clause.getFirstArg(), synonymList);
	}
	//first arg _, second arg int
	else if (clause.getFirstType() == Clause::Type::WILDCARD && clause.getSecondType() == Clause::Type::INTEGER) {
		std::unordered_set<int> temp = resultFetcher.getPrevious(secondArg);
		if (temp.empty()) {
			throw FalseClauseException("False Clause, empty list returned");
		}
		return;
	}
	//first arg int, second arg int
	else if (clause.getFirstType() == Clause::Type::INTEGER && clause.getSecondType() == Clause::Type::INTEGER) {
		std::unordered_set<int> temp = resultFetcher.getPrevious(secondArg);
		if (temp.find(firstArg) == temp.end()) {
			throw FalseClauseException("False Clause, empty list returned");
		}
		return;
	}
	//first arg synonym, second arg int
	else if (clause.getSecondType() == Clause::Type::INTEGER) {
		std::unordered_set<int> synonymList = resultFetcher.getPrevious(secondArg);
		removeWrongType(synonymList, clause.getFirstType());
		resultTable.mergeOneSynonym(clause.getFirstArg(), synonymList);
	}
	//first arg _, second arg synonym
	else if (clause.getFirstType() == Clause::Type::WILDCARD) {
		std::unordered_set<std::pair<int, int>, pair_hash> temp = resultFetcher.getNextPairs();
		std::unordered_set<int> synonymList = getAllSecondResult(temp);
		removeWrongType(synonymList, clause.getSecondType());
		resultTable.mergeOneSynonym(clause.getSecondArg(), synonymList);
	}
	//first arg int, second arg synonym
	else if (clause.getFirstType() == Clause::Type::INTEGER) {
		std::unordered_set<int> synonymList = resultFetcher.getNext(firstArg);
		removeWrongType(synonymList, clause.getSecondType());
		resultTable.mergeOneSynonym(clause.getSecondArg(), synonymList);
	}
	//first arg synonym, second arg synonym
	else { //if (clause.getFirstType() == Clause::Type::PROCEDURE)
		std::unordered_set<std::pair<int, int>, pair_hash> synonymList = resultFetcher.getNextPairs();
		removeWrongPairs(synonymList, clause.getFirstType(), clause.getSecondType());
		resultTable.mergeTwoSynonym(clause.getFirstArg(), clause.getSecondArg(), synonymList);
	}
	//check if table is empty
	if (resultTable.noResults()) {
		noResults = true;
	}
}

/*void QueryEvaluator::evaluateTransNext(Clause clause) {
	int firstArg = getIntRepresentation(clause.getFirstArg(), clause.getFirstType());
	int secondArg = getIntRepresentation(clause.getSecondArg(), clause.getSecondType());
	//first arg _, second arg _
	if (clause.getFirstType() == Clause::Type::WILDCARD && clause.getSecondType() == Clause::Type::WILDCARD) {
		std::unordered_set<std::pair<int, int>, pair_hash> temp = resultFetcher.getTransNextPairs(firstArg, secondArg);
		if (temp.empty()) {
			throw FalseClauseException("False Clause, empty list returned");
		}
		return;
	}
	//first arg int, second arg _
	else if (clause.getFirstType() == Clause::Type::INTEGER && clause.getSecondType() == Clause::Type::WILDCARD) {
		std::unordered_set<int> temp = resultFetcher.getTransNext(firstArg);
		if (temp.empty()) {
			throw FalseClauseException("False Clause, empty list returned");
		}
		return;
	}
	//first arg synonym, second arg _
	else if (clause.getSecondType() == Clause::Type::WILDCARD) {
		std::unordered_set<std::pair<int, int>, pair_hash> temp = resultFetcher.getTransNextPairs();
		std::unordered_set<int> synonymList = getAllFirstResult(temp);
		removeWrongType(synonymList, clause.getFirstType());
		resultTable.mergeOneSynonym(clause.getFirstArg(), synonymList);
	}
	//first arg _, second arg int
	else if (clause.getFirstType() == Clause::Type::WILDCARD && clause.getSecondType() == Clause::Type::INTEGER) {
		std::unordered_set<int> temp = resultFetcher.getTransPrevious(secondArg);
		if (temp.empty()) {
			throw FalseClauseException("False Clause, empty list returned");
		}
		return;
	}
	//first arg int, second arg int
	else if (clause.getFirstType() == Clause::Type::INTEGER && clause.getSecondType() == Clause::Type::INTEGER) {
		std::unordered_set<int> temp = resultFetcher.getTransPrevious(secondArg);
		if (temp.find(firstArg) == temp.end()) {
			throw FalseClauseException("False Clause, empty list returned");
		}
		return;
	}
	//first arg synonym, second arg int
	else if (clause.getSecondType() == Clause::Type::INTEGER) {
		std::unordered_set<int> synonymList = resultFetcher.getTransPrevious(secondArg);
		removeWrongType(synonymList, clause.getFirstType());
		resultTable.mergeOneSynonym(clause.getFirstArg(), synonymList);
	}
	//first arg _, second arg synonym
	else if (clause.getFirstType() == Clause::Type::WILDCARD) {
		std::unordered_set<std::pair<int, int>, pair_hash> temp = resultFetcher.getTransNextPairs();
		std::unordered_set<int> synonymList = getAllSecondResult(temp);
		removeWrongType(synonymList, clause.getSecondType());
		resultTable.mergeOneSynonym(clause.getSecondArg(), synonymList);
	}
	//first arg int, second arg synonym
	else if (clause.getFirstType() == Clause::Type::INTEGER) {
		std::unordered_set<int> synonymList = resultFetcher.getTranstNext(firstArg);
		removeWrongType(synonymList, clause.getSecondType());
		resultTable.mergeOneSynonym(clause.getSecondArg(), synonymList);
	}
	//first arg synonym, second arg synonym
	else { //if (clause.getFirstType() == Clause::Type::PROCEDURE)
		std::unordered_set<std::pair<int, int>, pair_hash> synonymList = resultFetcher.getTransNextPairs();
		removeWrongPairs(synonymList, clause.getFirstType(), clause.getSecondType());
		resultTable.mergeTwoSynonym(clause.getFirstArg(), clause.getSecondArg(), synonymList);
	}
	//check if table is empty
	if (resultTable.noResults()) {
		noResults = true;
	}
}
*/


void QueryEvaluator::evaluatePatternAssign(Clause clause) {
	//first arg is assign
	int firstArg = getIntRepresentation(clause.getFirstArg(), clause.getFirstType());
	int secondArg = getIntRepresentation(clause.getSecondArg(), clause.getSecondType());
	int thirdArg = getIntRepresentation(clause.getSecondArg(), clause.getSecondType());
	//second arg is wildcard, third arg is wildcard
	if (clause.getSecondType() == Clause::Type::WILDCARD && clause.getThirdType() == Clause::Type::WILDCARD) {
		unordered_set<int> synonymList = resultFetcher.getAllAssigns();
		resultTable.mergeOneSynonym(clause.getFirstArg(), synonymList);
	}
	//second arg is variable synonym, third arg is wildcard
	else if (clause.getSecondType() == Clause::Type::VARIABLE && clause.getThirdType() == Clause::Type::WILDCARD) {
		unordered_set<pair<int, int>, pair_hash> synonymList = resultFetcher.getModifiesStmtPairs();
		removeWrongPairs(synonymList, clause.getFirstType(), clause.getSecondType());
		resultTable.mergeTwoSynonym(clause.getFirstArg(), clause.getSecondArg(), synonymList);
	}
	//second arg is ident, third arg is wildcard
	else if (clause.getSecondType() == Clause::Type::IDENT && clause.getThirdType() == Clause::Type::WILDCARD) {
		unordered_set<int> synonymList = resultFetcher.getStmtModifying(secondArg);
		removeWrongType(synonymList, clause.getFirstType());
		resultTable.mergeOneSynonym(clause.getFirstArg(), synonymList);
	}
	//second arg is wildcard, third arg is subexpr("_expr_")
	else if (clause.getSecondType() == Clause::Type::WILDCARD && clause.getThirdType() == Clause::Type::SUBEXPRESSION) {
		unordered_set<int> synonymList = resultFetcher.getSubExpr(clause.getThirdArg());
		resultTable.mergeOneSynonym(clause.getFirstArg(), synonymList);
	}
	//second arg is variable synonym, third arg is subexpr
	else if (clause.getSecondType() == Clause::Type::VARIABLE && clause.getThirdType() == Clause::Type::SUBEXPRESSION) {
		unordered_set<pair<int, int>, pair_hash> synonymList1 = resultFetcher.getModifiesStmtPairs();
		removeWrongPairs(synonymList1, clause.getFirstType(), clause.getSecondType());

		unordered_set<int> synonymList2 = resultFetcher.getSubExpr(clause.getThirdArg());
		unordered_set<pair<int, int>, pair_hash> synonymList3 = getIntersection1(synonymList1, synonymList2);
		resultTable.mergeTwoSynonym(clause.getFirstArg(), clause.getSecondArg(), synonymList3);
	}
	//second arg is ident, third arg is subexpr
	else if (clause.getSecondType() == Clause::Type::IDENT && clause.getThirdType() == Clause::Type::SUBEXPRESSION) {
		unordered_set<int> synonymList1 = resultFetcher.getStmtModifying(secondArg);
		removeWrongType(synonymList1, clause.getFirstType());
	
		unordered_set<int> synonymList2 = resultFetcher.getSubExpr(clause.getThirdArg());
		unordered_set<int> synonymList3 = getIntersection2(synonymList1, synonymList2);
		resultTable.mergeOneSynonym(clause.getFirstArg(), synonymList3);
	}
	//second arg is wildcard, third arg is expr("expr")
	else if (clause.getSecondType() == Clause::Type::WILDCARD && clause.getThirdType() == Clause::Type::EXPRESSION) {
		unordered_set<int> synonymList = resultFetcher.getExactExpr(clause.getThirdArg());
		resultTable.mergeOneSynonym(clause.getFirstArg(), synonymList);
	}//second arg is variable synonym, third arg is expr("expr")
	else if (clause.getSecondType() == Clause::Type::VARIABLE && clause.getThirdType() == Clause::Type::EXPRESSION) {
		unordered_set<pair<int, int>, pair_hash> synonymList1 = resultFetcher.getModifiesStmtPairs();
		removeWrongPairs(synonymList1, clause.getFirstType(), clause.getSecondType());

		unordered_set<int> synonymList2 = resultFetcher.getExactExpr(clause.getThirdArg());
		unordered_set<pair<int, int>, pair_hash> synonymList3 = getIntersection1(synonymList1, synonymList2);
		resultTable.mergeTwoSynonym(clause.getFirstArg(), clause.getSecondArg(), synonymList3);
	}//second arg is ident, third arg is expr("expr")
	else{ //if (clause.getSecondType() == Clause::Type::IDENT && clause.getThirdType() == Clause::Type::EXPRESSION)
		unordered_set<int> synonymList1 = resultFetcher.getStmtModifying(secondArg);
		removeWrongType(synonymList1, clause.getFirstType());

		unordered_set<int> synonymList2 = resultFetcher.getExactExpr(clause.getThirdArg());
		unordered_set<int> synonymList3 = getIntersection2(synonymList1, synonymList2);
		resultTable.mergeOneSynonym(clause.getFirstArg(), synonymList3);
	}
	//check if table is empty
	if (resultTable.noResults()) {
		noResults = true;
	}
}

void QueryEvaluator::evaluatePatternIf(Clause clause) {
	//first arg is if
	int firstArg = getIntRepresentation(clause.getFirstArg(), clause.getFirstType());
	int secondArg = getIntRepresentation(clause.getSecondArg(), clause.getSecondType());
	//second arg is wildcard
	if (clause.getSecondType() == Clause::Type::WILDCARD) {
		unordered_set<int> synonymList = resultFetcher.getAllIfs();
		resultTable.mergeOneSynonym(clause.getFirstArg(), synonymList);
	}
	//second arg is variable synonym
	else if (clause.getSecondType() == Clause::Type::VARIABLE) {
		unordered_set<pair<int, int>, pair_hash> synonymList = resultFetcher.getAllIfControlVarPairs();
		removeWrongPairs(synonymList, clause.getFirstType(), clause.getSecondType());
		resultTable.mergeTwoSynonym(clause.getFirstArg(), clause.getSecondArg(), synonymList);
	}
	//second arg is ident
	else if (clause.getSecondType() == Clause::Type::IDENT) {
		unordered_set<int> synonymList = resultFetcher.getAllIfswithControlVar(secondArg);
		resultTable.mergeOneSynonym(clause.getFirstArg(), synonymList);
	}
	//check if table is empty
	if (resultTable.noResults()) {
		noResults = true;
	}
}

void QueryEvaluator::evaluatePatternWhile(Clause clause) {
	//first arg is while
	int firstArg = getIntRepresentation(clause.getFirstArg(), clause.getFirstType());
	int secondArg = getIntRepresentation(clause.getSecondArg(), clause.getSecondType());
	//second arg is wildcard
	if (clause.getSecondType() == Clause::Type::WILDCARD) {
		unordered_set<int> synonymList = resultFetcher.getAllWhiles();
		resultTable.mergeOneSynonym(clause.getFirstArg(), synonymList);
	}
	//second arg is variable synonym
	else if (clause.getSecondType() == Clause::Type::VARIABLE) {
		unordered_set<pair<int, int>, pair_hash> synonymList = resultFetcher.getAllWhileControlVarPairs();
		removeWrongPairs(synonymList, clause.getFirstType(), clause.getSecondType());
		resultTable.mergeTwoSynonym(clause.getFirstArg(), clause.getSecondArg(), synonymList);
	}
	//second arg is ident
	else if (clause.getSecondType() == Clause::Type::IDENT) {
		unordered_set<int> synonymList = resultFetcher.getAllWhileswithControlVar(secondArg);
		resultTable.mergeOneSynonym(clause.getFirstArg(), synonymList);
	}
	//check if table is empty
	if (resultTable.noResults()) {
		noResults = true;
	}
}



/*void QueryEvaluator::evaluateWith(Clause clause) {
	int firstArg = getIntRepresentation(clause.getFirstArg(), clause.getFirstType());
	int secondArg = getIntRepresentation(clause.getSecondArg(), clause.getSecondType());
	if (clause.getFirstType() == Clause::Type::PROCEDURE && clause.getSecondType() == Clause::Type::IDENT) {
		unordered_set<int> temp = resultFetcher.getAllProcedures();
		unordered_set<int> synonymList;
		unordered_set<int>::iterator it = temp.find(secondArg);
		if (it == temp.end()) {
			throw FalseClauseException("False Clause, empty list returned");
		}
		else {
			synonymList.insert(*it);
			resultTable.mergeOneSynonym(clause.getFirstArg(), synonymList);
		}
	}
}
*/





//checks set for correct synonym type pairs, removes wrong pairs
void QueryEvaluator::removeWrongPairs(std::unordered_set<std::pair<int, int>, pair_hash>& tempSet, Clause::Type type1, Clause::Type type2) {
	std::unordered_set<std::pair<int, int>, pair_hash>::iterator it;
	for (it = tempSet.begin(); it != tempSet.end();)
	{
		if (!isSameType(type1, it->first)|| !isSameType(type2, it->second)) {
			it = tempSet.erase(it);
		}
		else ++it;
	}
}

//checks set for correct arg synonym type, removes wrong arg synonym type
void QueryEvaluator::removeWrongType(std::unordered_set<int>& tempSet, Clause::Type type) {
	std::unordered_set<int>::iterator it;
	for (it = tempSet.begin(); it != tempSet.end();)
	{
		if (!isSameType(type, *it)) {
			it = tempSet.erase(it);
		}
		else ++it;
	}
}

std::unordered_set<int> QueryEvaluator::getAllFirstResult(std::unordered_set<std::pair<int, int>, pair_hash> tempSet) {
	std::unordered_set<std::pair<int, int>, pair_hash> ::iterator it;
	std::unordered_set<int> synonymList;
	for (it = tempSet.begin(); it != tempSet.end(); ++it)
	{
		synonymList.insert(it->first);
	}
	return synonymList;
}

std::unordered_set<int> QueryEvaluator::getAllSecondResult(std::unordered_set<std::pair<int, int>, pair_hash> tempSet) {
	std::unordered_set<std::pair<int, int>, pair_hash> ::iterator it;
	std::unordered_set<int> synonymList;
	for (it = tempSet.begin(); it != tempSet.end(); ++it)
	{
		synonymList.insert(it->second);
	}
	return synonymList;
}

std::unordered_set<std::pair<int, int>, pair_hash> QueryEvaluator::getIntersection1(std::unordered_set<std::pair<int, int>, pair_hash> list1, std::unordered_set<int> list2) {
	//list1: assign, variable, list2: assign
	std::unordered_set<std::pair<int, int>, pair_hash> synonymList;
	for (std::unordered_set<std::pair<int, int>, pair_hash>::iterator it = list1.begin(); it != list1.end(); ++it) {
		if (std::find(list2.begin(), list2.end(), it->first) != list2.end()) {
			synonymList.insert(*it);
		}
	}
	return synonymList;
}

std::unordered_set<int> QueryEvaluator::getIntersection2(std::unordered_set<int> list1, std::unordered_set<int> list2) {
	//list1: assign, list2: assign
	std::unordered_set<int> synonymList;
	for (std::unordered_set<int>::iterator it = list1.begin(); it != list1.end(); ++it) {
		if (std::find(list2.begin(), list2.end(), *it) != list2.end()) {
			synonymList.insert(*it);
		}
	}
	return synonymList;
}

//checks if same statement type
bool QueryEvaluator::isSameType(Clause::Type type, int temp) {
	switch (type) {
	case Clause::Type::ASSIGN:
		if (!resultFetcher.isAssign(temp)) {
			return false;
		}
		break;
	case Clause::Type::CALL:
	case Clause::Type::CALLPROC:
		if (!resultFetcher.isCall(temp)) {
			return false;
		}
		break;
	case Clause::Type::IF:
		if (!resultFetcher.isIf(temp)) {
			return false;
		}
		break;
	case Clause::Type::PROG_LINE:
		if (!resultFetcher.isProgLine(temp)) {
			return false;
		}
		break;
	case Clause::Type::STMT:
		if (!resultFetcher.isStmt(temp)) {
			return false;
		}
		break;
	case Clause::Type::WHILE:
		if (!resultFetcher.isWhile(temp)) {
			return false;
		}
		break;
	}
}

std::string QueryEvaluator::getStringRepresentation(int intRepresentation, Clause::Type type) {
	switch (type) {
		case Clause::Type::ASSIGN:
		case Clause::Type::IF:
		case Clause::Type::PROG_LINE:
		case Clause::Type::STMT:
		case Clause::Type::WHILE:
		case Clause::Type::CALL:
			return resultFetcher.getStmtRepresentation(intRepresentation);
			break;
		case Clause::Type::CONSTANT:
			return resultFetcher.getConstantRepresentation(intRepresentation);
			break;
		case Clause::Type::PROCEDURE:
		case Clause::Type::CALLPROC:
			return resultFetcher.getProcedureRepresentation(intRepresentation);
			break;
		case Clause::Type::VARIABLE:
			return resultFetcher.getVariableRepresentation(intRepresentation);
			break;
		default:
			break;
	}
}

int QueryEvaluator::getIntRepresentation(std::string stringRepresentation, Clause::Type type) {
	switch (type) {
		case Clause::Type::ASSIGN:
		case Clause::Type::IF:
		case Clause::Type::PROG_LINE:
		case Clause::Type::STMT:
		case Clause::Type::WHILE:
		case Clause::Type::CALL:
		case Clause::Type::CALLPROC:
		case Clause::Type::VARIABLE:
		case Clause::Type::PROCEDURE:
		case Clause::Type::CONSTANT:
			return -1;
		case Clause::Type::INTEGER:
			return resultFetcher.getStmt(stringRepresentation);
		case Clause::Type::IDENT:
			int v = resultFetcher.getVariable(stringRepresentation);
			if (v == -1) {//IDENT is not a variable name
				int s = resultFetcher.getProcedure(stringRepresentation);
				if (s == -1) {//IDENT is not a procedure name
					return resultFetcher.getConstant(stringRepresentation);
				}
				//IDENT is a procedure name
				return s;
			}
			//IDENT is a variable name
			return v;
	}
}

void QueryEvaluator::clear() {
	noResults = false;
	resultTable = ResultTable();
}

//previous code
//void QueryEvaluator::filterType(const std::string & name, const QueryObject::argType &type) {
//	switch (type) {
//	case QueryObject::ASSIGN:
//		resultTables.denormalizingMergeNew(ResultTable{ name, resultFetcher.getAllAssigns() });
//		break;
//	case QueryObject::CALL:
//		//resultTables.denormalizingMergeNew(ResultTable{ name, resultFetcher.getAllCalls() });
//		break;
//	case QueryObject::CONSTANT:
//		resultTables.denormalizingMergeNew(ResultTable{ name, resultFetcher.getAllConstants() });
//		break;
//	case QueryObject::IF:
//		//resultTables.denormalizingMergeNew(ResultTable{ name, resultFetcher.getAllIfs() });
//		break;
//	case QueryObject::PROCEDURE:
//		//resultTables.denormalizingMergeNew(ResultTable{ name, resultFetcher.getAllProcedures() });
//		break;
//	case QueryObject::PROG_LINE:
//	case QueryObject::STMT:
//		resultTables.denormalizingMergeNew(ResultTable{ name, resultFetcher.getAllStmts() });
//		break;
//	case QueryObject::VARIABLE:
//		resultTables.denormalizingMergeNew(ResultTable{ name, resultFetcher.getAllVariables() });
//		break;
//	case QueryObject::WHILE:
//		resultTables.denormalizingMergeNew(ResultTable{ name, resultFetcher.getAllWhiles() });
//		break;
//	}
//}
//
//void QueryEvaluator::evaluateClause(
//	const QueryObject::clauseType clauseType,
//	const std::vector<std::string> argNames,
//	const std::vector<QueryObject::argType> argTypes) {
//	switch (clauseType) {
//	case QueryObject::FOLLOWS:
//		tryEvaluateBothLiteralClauseBySecond(argNames, argTypes,
//			&ResultFetcher::getStmt, &ResultFetcher::getStmt, &ResultFetcher::getFollowedBy);
//		tryEvaluateFirstArgLiteralClause(argNames, argTypes,
//			&ResultFetcher::getStmt, &ResultFetcher::getFollowing);
//		tryEvaluateSecondArgLiteralClause(argNames, argTypes,
//			&ResultFetcher::getStmt, &ResultFetcher::getFollowedBy);
//		tryEvaluateZeroLiteralClause(argNames, argTypes,
//			&ResultFetcher::getFollowsPairs);
//		break;
//	case QueryObject::FOLLOWS_T:
//		tryEvaluateBothLiteralClauseByFirst(argNames, argTypes,
//			&ResultFetcher::getStmt, &ResultFetcher::getStmt, &ResultFetcher::getTransFollowing);
//		tryEvaluateFirstArgLiteralClause(argNames, argTypes,
//			&ResultFetcher::getStmt, &ResultFetcher::getTransFollowing);
//		tryEvaluateSecondArgLiteralClause(argNames, argTypes,
//			&ResultFetcher::getStmt, &ResultFetcher::getTransFollowedBy);
//		tryEvaluateZeroLiteralClause(argNames, argTypes,
//			&ResultFetcher::getTransFollowsPairs);
//		break;
//	case QueryObject::PARENT:
//		tryEvaluateBothLiteralClauseBySecond(argNames, argTypes,
//			&ResultFetcher::getStmt, &ResultFetcher::getStmt, &ResultFetcher::getParentOf);
//		tryEvaluateFirstArgLiteralClause(argNames, argTypes,
//			&ResultFetcher::getStmt, &ResultFetcher::getChildrenOf);
//		tryEvaluateSecondArgLiteralClause(argNames, argTypes,
//			&ResultFetcher::getStmt, &ResultFetcher::getParentOf);
//		tryEvaluateZeroLiteralClause(argNames, argTypes,
//			&ResultFetcher::getParentPairs);
//		break;
//	case QueryObject::PARENT_T:
//		tryEvaluateBothLiteralClauseByFirst(argNames, argTypes,
//			&ResultFetcher::getStmt, &ResultFetcher::getStmt, &ResultFetcher::getTransChildrenOf);
//		tryEvaluateFirstArgLiteralClause(argNames, argTypes,
//			&ResultFetcher::getStmt, &ResultFetcher::getTransChildrenOf);
//		tryEvaluateSecondArgLiteralClause(argNames, argTypes,
//			&ResultFetcher::getStmt, &ResultFetcher::getTransParentOf);
//		tryEvaluateZeroLiteralClause(argNames, argTypes,
//			&ResultFetcher::getTransParentPairs);
//		break;
//	case QueryObject::MODIFIES:
//		tryEvaluateBothLiteralClauseByFirst(argNames, argTypes,
//			&ResultFetcher::getStmt, &ResultFetcher::getVariable, &ResultFetcher::getModifiedByStmt);
//		tryEvaluateFirstArgLiteralClause(argNames, argTypes,
//			&ResultFetcher::getStmt, &ResultFetcher::getModifiedByStmt);
//		tryEvaluateSecondArgLiteralClause(argNames, argTypes,
//			&ResultFetcher::getVariable, &ResultFetcher::getStmtModifying);
//		tryEvaluateZeroLiteralClause(argNames, argTypes,
//			&ResultFetcher::getModifiesStmtPairs);
//		break;
//	case QueryObject::USES:
//		tryEvaluateBothLiteralClauseByFirst(argNames, argTypes,
//			&ResultFetcher::getStmt, &ResultFetcher::getVariable, &ResultFetcher::getUsedByStmt);
//		tryEvaluateFirstArgLiteralClause(argNames, argTypes,
//			&ResultFetcher::getStmt, &ResultFetcher::getUsedByStmt);
//		tryEvaluateSecondArgLiteralClause(argNames, argTypes,
//			&ResultFetcher::getVariable, &ResultFetcher::getStmtUsing);
//		tryEvaluateZeroLiteralClause(argNames, argTypes,
//			&ResultFetcher::getUsesStmtPairs);
//		break;
//	case QueryObject::PATTERN:
//		evaluatePatternClause(argNames, argTypes);
//		break;
//	default:
//		throw std::invalid_argument("Invalid clause type");
//	}
//}
//
//std::list<std::string> QueryEvaluator::evaluateSingleSelect(const std::string name, const QueryObject::argType type) {
//	filterType(name, type);
//	// resultTableIndex is denormalized
//	auto retVector = resultTables.projectFromDenormalized(name);
//	std::list<std::string> retList{};
//	switch (type) {
//	case QueryObject::CONSTANT:
//		for (const auto &i : retVector) {
//			retList.push_back(resultFetcher.getConstantRepresentation(i));
//		}
//		break;
//	case QueryObject::PROCEDURE:
//		for (const auto &i : retVector) {
//			//retList.push_back(resultFetcher.getProcedureRepresentation(i));
//		}
//		break;
//	case QueryObject::PROG_LINE:
//	case QueryObject::STMT:
//	case QueryObject::ASSIGN:
//	case QueryObject::CALL:
//	case QueryObject::IF:
//	case QueryObject::WHILE:
//		for (const auto &i : retVector) {
//			retList.push_back(resultFetcher.getStmtRepresentation(i));
//		}
//		break;
//	case QueryObject::VARIABLE:
//		for (const auto &i : retVector) {
//			retList.push_back(resultFetcher.getVariableRepresentation(i));
//		}
//		break;
//	}
//	return retList;
//}
//
//void QueryEvaluator::tryEvaluateZeroLiteralClause(
//	const std::vector<std::string> &argNames,
//	const std::vector<QueryObject::argType> &argTypes,
//	std::vector<std::pair<Result, Result>>(ResultFetcher::* getPairs)()) {
//	if (argTypes.size() != 2
//		|| QueryObject::isLiteral(argTypes.at(0))
//		|| QueryObject::isLiteral(argTypes.at(1))) {
//		return;
//	}
//	auto pairs = (resultFetcher.*getPairs)();
//	if (pairs.empty()) {
//		noResults = true;
//		return;
//	}
//	if (QueryObject::isSynonym(argTypes.at(0)) && QueryObject::isSynonym(argTypes.at(1))) {
//		resultTables.denormalizingMergeNew(ResultTable{
//			{ argNames.at(0), argNames.at(1) },
//			pairs
//		});
//		filterType(argNames.at(0), argTypes.at(0));
//		filterType(argNames.at(1), argTypes.at(1));
//		return;
//	}
//	if (QueryObject::isSynonym(argTypes.at(0))) {
//		std::unordered_set<Result> first;
//		for (const auto &p : pairs) {
//			first.emplace(p.first);
//		}
//		resultTables.denormalizingMergeNew(ResultTable{
//			argNames.at(0),
//			std::vector<Result>{ first.begin(), first.end() }
//		});
//		filterType(argNames.at(0), argTypes.at(0));
//		return;
//	}
//	if (QueryObject::isSynonym(argTypes.at(1))) {
//		std::unordered_set<Result> second;
//		for (const auto &p : pairs) {
//			second.emplace(p.second);
//		}
//		resultTables.denormalizingMergeNew(ResultTable{
//			argNames.at(1),
//			std::vector<Result>{ second.begin(), second.end() }
//		});
//		filterType(argNames.at(1), argTypes.at(1));
//		return;
//	}
//}
//
//void QueryEvaluator::tryEvaluateFirstArgLiteralClause(
//	const std::vector<std::string> &argNames,
//	const std::vector<QueryObject::argType> &argTypes,
//	Result(ResultFetcher::*litToResult)(std::string name),
//	std::vector<Result>(ResultFetcher::* getSecondResult)(Result first)) {
//	if (argTypes.size() != 2
//		|| !QueryObject::isLiteral(argTypes.at(0))
//		|| QueryObject::isLiteral(argTypes.at(1))) {
//		return;
//	}
//	auto r1{ (resultFetcher.*getSecondResult)((resultFetcher.*litToResult)(argNames.at(0))) };
//	if (r1.empty()) {
//		noResults = true;
//		return;
//	}
//
//	if (QueryObject::isSynonym(argTypes.at(1))) {
//		resultTables.denormalizingMergeNew(ResultTable{
//			argNames.at(1),
//			r1
//		});
//		filterType(argNames.at(1), argTypes.at(1));
//	}
//}
//
//void QueryEvaluator::tryEvaluateFirstArgLiteralClause(
//	const std::vector<std::string>& argNames,
//	const std::vector<QueryObject::argType>& argTypes,
//	Result(ResultFetcher::* litToResult)(std::string name),
//	Result(ResultFetcher::* getSecondResult)(Result first)) {
//	if (argTypes.size() != 2
//		|| !QueryObject::isLiteral(argTypes.at(0))
//		|| QueryObject::isLiteral(argTypes.at(1))) {
//		return;
//	}
//	auto r1 = (resultFetcher.*getSecondResult)((resultFetcher.*litToResult)(argNames.at(0)));
//	if (r1 == ResultFetcher::NO_RESULT) {
//		noResults = true;
//		return;
//	}
//
//	if (QueryObject::isSynonym(argTypes.at(1))) {
//		resultTables.denormalizingMergeNew(ResultTable{
//			argNames.at(1),
//			{ r1 }
//		});
//		filterType(argNames.at(1), argTypes.at(1));
//	}
//}
//
//void QueryEvaluator::tryEvaluateSecondArgLiteralClause(
//	const std::vector<std::string> &argNames,
//	const std::vector<QueryObject::argType> &argTypes,
//	Result(ResultFetcher::*litToResult)(std::string name),
//	std::vector<Result>(ResultFetcher::* getFirstResult)(Result second)) {
//	if (argTypes.size() != 2
//		|| QueryObject::isLiteral(argTypes.at(0))
//		|| !QueryObject::isLiteral(argTypes.at(1))) {
//		return;
//	}
//	auto r0{ (resultFetcher.*getFirstResult)((resultFetcher.*litToResult)(argNames.at(1))) };
//	if (r0.empty()) {
//		noResults = true;
//		return;
//	}
//
//	if (QueryObject::isSynonym(argTypes.at(0))) {
//		resultTables.denormalizingMergeNew(ResultTable{
//			argNames.at(0),
//			r0
//		});
//		filterType(argNames.at(0), argTypes.at(0));
//	}
//}
//
//void QueryEvaluator::tryEvaluateSecondArgLiteralClause(
//	const std::vector<std::string>& argNames,
//	const std::vector<QueryObject::argType>& argTypes,
//	Result(ResultFetcher::* litToResult)(std::string name),
//	Result(ResultFetcher::* getFirstResult)(Result second)) {
//	if (argTypes.size() != 2
//		|| QueryObject::isLiteral(argTypes.at(0))
//		|| !QueryObject::isLiteral(argTypes.at(1))) {
//		return;
//	}
//	auto r0 = (resultFetcher.*getFirstResult)((resultFetcher.*litToResult)(argNames.at(1)));
//	if (r0 == ResultFetcher::NO_RESULT) {
//		noResults = true;
//		return;
//	}
//
//	if (QueryObject::isSynonym(argTypes.at(0))) {
//		resultTables.denormalizingMergeNew(ResultTable{
//			argNames.at(0),
//			{ r0 }
//		});
//		filterType(argNames.at(0), argTypes.at(0));
//	}
//}
//
//void QueryEvaluator::evaluatePatternClause(
//	const std::vector<std::string>& argNames,
//	const std::vector<QueryObject::argType>& argTypes) {
//	auto &resultFetcher = this->resultFetcher;
//
//	std::vector<std::string> truncatedArgNames{ argNames.at(0), argNames.at(1) };
//	std::vector<QueryObject::argType> truncatedArgTypes{ argTypes.at(0), argTypes.at(1) };
//	tryEvaluateBothLiteralClauseByFirst(truncatedArgNames, truncatedArgTypes,
//		&ResultFetcher::getStmt, &ResultFetcher::getVariable, &ResultFetcher::getModifiedByStmt);
//	tryEvaluateFirstArgLiteralClause(truncatedArgNames, truncatedArgTypes,
//		&ResultFetcher::getStmt, &ResultFetcher::getModifiedByStmt);
//	tryEvaluateSecondArgLiteralClause(truncatedArgNames, truncatedArgTypes,
//		&ResultFetcher::getVariable, &ResultFetcher::getStmtModifying);
//	tryEvaluateZeroLiteralClause(truncatedArgNames, truncatedArgTypes,
//		&ResultFetcher::getModifiesStmtPairs);
//	if (argTypes.at(2) == QueryObject::WILDCARD) {
//		return;
//	}
//	// argTypes.at(2) == QueryObject::SUBEXPRESSION
//	// Assumption: first arg must always be a synonym; if second arg is also synonym.
//	//     suffices to filter by first arg
//	auto subexpr = argNames.at(2);
//	resultTables.filterByColumnPred(argNames.at(0), [&resultFetcher, &subexpr](Result r) {
//		return !resultFetcher.hasSubexpr(r, subexpr);
//	});
//}
//
//void QueryEvaluator::tryEvaluateBothLiteralClauseByFirst	(
//	const std::vector<std::string> &argNames,
//	const std::vector<QueryObject::argType> &argTypes,
//	Result(ResultFetcher::*arg0LitToResult)(std::string name),
//	Result(ResultFetcher::*arg1LitToResult)(std::string name),
//	std::vector<Result>(ResultFetcher::* getSecondResult)(Result first)) {
//	if (argTypes.size() != 2
//		|| !QueryObject::isLiteral(argTypes.at(0))
//		|| !QueryObject::isLiteral(argTypes.at(1))) {
//		return;
//	}
//	auto r1{ (resultFetcher.*getSecondResult)((resultFetcher.*arg0LitToResult)(argNames.at(0))) };
//	if (std::find(r1.begin(), r1.end(), (resultFetcher.*arg1LitToResult)(argNames.at(1))) == r1.end()) {
//		noResults = true;
//		return;
//	}
//	return;
//}
//
//void QueryEvaluator::tryEvaluateBothLiteralClauseBySecond(
//	const std::vector<std::string> &argNames,
//	const std::vector<QueryObject::argType> &argTypes,
//	Result(ResultFetcher::*arg0LitToResult)(std::string name),
//	Result(ResultFetcher::*arg1LitToResult)(std::string name),
//	Result(ResultFetcher::*getFirstResult)(Result second)) {
//	if (argTypes.size() != 2
//		|| !QueryObject::isLiteral(argTypes.at(0))
//		|| !QueryObject::isLiteral(argTypes.at(1))) {
//		return;
//	}
//	auto r1 = (resultFetcher.*getFirstResult)((resultFetcher.*arg1LitToResult)(argNames.at(1)));
//	if (r1 == ResultFetcher::NO_RESULT || r1 != (resultFetcher.*arg0LitToResult)(argNames.at(0))) {
//		noResults = true;
//		return;
//	}
//	return;
//}