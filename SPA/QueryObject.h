#pragma once

#include <string>
#include <vector>
#include <unordered_set>
#include "ClauseGroup.h"

using namespace std;

class QueryObject {
public:
    QueryObject();
    ~QueryObject();

    /*
    Method for adding a SELECT clause with 1 entity.
    */
    void addSelectClause(string type, string name);

    /*
    Method for adding a SELECT clause for selecting tuple.
    */
    void addSelectTupleClause(vector<string> types, vector<string> names);

    /*
    Method for adding a clause with 2 arguments, such as Parent(s1,s2).
    This adds an approriate clause object into the clauses vector,
    which contains the relation and the 2 arguments' types & names.
    */
    void addClause(string clauseRelation,
        string argLeftType, string argLeftName,
        string argRightType, string argRighttName);

    /*
    Method for adding a PATTERN clause, where Pattern arg1(arg2, arg3).
    This adds a PATTERN-relation clause object into the clauses vector,
    which contains the relation and the three arguments' types & names.
    Regarding the string patternType, use "Assign"/"If"/"While"
    for pattern-assign, pattern-if, pattern-while respectively.
    */
    void addPatternClause(string patternType,
        string arg1Type, string arg1Name,
        string arg2Type, string arg2Name,
        string arg3Type, string arg3Name);

    // get the select clause
    Clause getSelectClause();

    // get vector containing all other clauses
    vector<Clause> getClauses();

    /*
    Method to clear the clauses vector in QueryObject for a new query.
    selectClause remains unchanged as it will be replaced during a new query.
    */
    void clear();

    // for optimizer
    unordered_set<string> getAllSynonyms();
    vector<Clause> getGenericClauses();
    vector<Clause> getBooleanClauses();

    void QueryObject::setBooleanGroup(ClauseGroup booleanGroup);
    void QueryObject::setGenericGroup(vector<ClauseGroup> genericGroup);
    void QueryObject::setSelectGroup(ClauseGroup selectGroup);

    // for evaluator
    ClauseGroup QueryObject::getBooleanGroup();
    vector<ClauseGroup> QueryObject::getGenericGroup();
    ClauseGroup QueryObject::getSelectGroup();

private:
    Clause selectClause;
    vector<Clause> clauses;

    // for optimizer
    unordered_set<string> allSynonyms;
    vector<Clause> genericGroup;
    vector<Clause> booleanGroup;

    // for evaluator
    ClauseGroup booleanClauses;
    vector<ClauseGroup> genericClauses;
    ClauseGroup selectClauses;
};
