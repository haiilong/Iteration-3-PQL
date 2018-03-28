#include "QueryObject.h"

void QueryObject::addSelectClause(std::string type, std::string name) {
    Clause newSelectClause = Clause();
    newSelectClause.addSelectArg(type, name);
    selectClause = newSelectClause;
}

void QueryObject::addSelectTupleClause(std::vector<std::string> types, std::vector<std::string> names) {
    Clause newSelectClause = Clause();
    newSelectClause.addSelectTupleArgs(types, names);
    selectClause = newSelectClause;
}

void QueryObject::addClause(std::string clauseRelation, std::string argLeftType, std::string argLeftName, std::string argRightType, std::string argRightName) {
    Clause newClause = Clause();
    newClause.setRelation(clauseRelation);
    newClause.setArgumentTypes(argLeftType, argRightType, "empty");
    newClause.setArgumentNames(argLeftName, argRightName, "");
    clauses.push_back(newClause);
}

void QueryObject::addPatternClause(std::string patternType, std::string arg1Type, std::string arg1Name, std::string arg2Type, std::string arg2Name, std::string arg3Type, std::string arg3Name) {
    Clause newClause = Clause();
    newClause.setRelation(patternType);
    newClause.setArgumentTypes(arg1Type, arg2Type, arg3Type);
    newClause.setArgumentNames(arg1Name, arg2Name, arg3Name);
    clauses.push_back(newClause);
}

Clause QueryObject::getSelectClause() {
    return selectClause;
}

std::vector<Clause> QueryObject::getClauses() {
    return clauses;
}

void QueryObject::clear() {
    clauses.clear();
}