#pragma once

#include <string>
#include <vector>
#include "Clause.h"

class QueryObject {
public:
    QueryObject() = default;

    /*
        Method for adding a SELECT clause with 1 entity.
    */
    void addSelectClause(std::string type, std::string name);

    /*
        Method for adding a SELECT clause for selecting tuple.
    */
    void addSelectTupleClause(std::vector<std::string> types, std::vector<std::string> names);

    /*
        Method for adding a clause with 2 arguments, such as Parent(s1,s2).
        This adds an approriate clause object into the clauses vector,
        which contains the relation and the 2 arguments' types & names.
    */
    void addClause(std::string clauseRelation,
        std::string argLeftType, std::string argLeftName,
        std::string argRightType, std::string argRighttName);

    /*
        Method for adding a PATTERN clause, where Pattern arg1(arg2, arg3).
        This adds a PATTERN-relation clause object into the clauses vector,
        which contains the relation and the three arguments' types & names.
        Regarding the string patternType, use "Assign"/"If"/"While"
        for pattern-assign, pattern-if, pattern-while respectively.
    */
    void addPatternClause(std::string patternType,
        std::string arg1Type, std::string arg1Name,
        std::string arg2Type, std::string arg2Name,
        std::string arg3Type, std::string arg3Name);

    // get the select clause
    Clause getSelectClause();

    // get vector containing all other clauses
    std::vector<Clause> getClauses();

    /*
        Method to clear the clauses vector in QueryObject for a new query.
        selectClause remains unchanged as it will be replaced during a new query.
    */
    void clear();

private:
    Clause selectClause;
    std::vector<Clause> clauses;
};
