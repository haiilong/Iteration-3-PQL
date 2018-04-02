#include "QueryObject.h"

QueryObject::QueryObject() {}

QueryObject::~QueryObject() {}

void QueryObject::addSelectClause(string type, string name) {
    Clause newSelectClause = Clause();
    newSelectClause.addSelectArg(type, name);
    selectClause = newSelectClause;

    selectSynonyms.push_back(name);
    selectSynonymMap[name] = type;
}

void QueryObject::addSelectTupleClause(vector<string> types, vector<string> names) {
    Clause newSelectClause = Clause();
    newSelectClause.addSelectTupleArgs(types, names);
    selectClause = newSelectClause;
    for (size_t i = 0; i < types.size(); i++) {
        selectSynonyms.push_back(names[i]);
        selectSynonymMap[names[i]] = types[i];
    }
}

void QueryObject::addClause(string clauseRelation, string argLeftType, string argLeftName, string argRightType, string argRightName) {
    Clause clause = Clause();
    clause.setRelation(clauseRelation, argLeftType, argRightType, "empty", argLeftName, argRightName, "");
    //clauses.push_back(clause);

    if (clause.getNoOfSyonyms() == 0) {
        booleanGroup.push_back(clause);
    }
    else {
        genericGroup.push_back(clause);
        for (auto item : clause.getSynonyms()) {
            if (allSynonyms.find(item) == allSynonyms.end()) {
                allSynonyms.insert(item);
            }
        }
    }

}

void QueryObject::addPatternClause(string patternType, string arg1Type, string arg1Name, string arg2Type, string arg2Name, string arg3Type, string arg3Name) {
    Clause clause = Clause();
    clause.setRelation(patternType, arg1Type, arg2Type, arg3Type, arg1Name, arg2Name, arg3Name);
    //clauses.push_back(clause);
    if (clause.getNoOfSyonyms() == 0) {
        booleanGroup.push_back(clause);
    }
    else {
        genericGroup.push_back(clause);
        for (auto item : clause.getSynonyms()) {
            if (allSynonyms.find(item) == allSynonyms.end()) {
                allSynonyms.insert(item);
            }
        }
    }
}

Clause QueryObject::getSelectClause() {
    return selectClause;
}

vector<Clause> QueryObject::getClauses() {
    return clauses;
}

void QueryObject::clear() {
    //clauses.clear();
    allSynonyms.clear();
    genericGroup.clear();
    booleanGroup.clear();

    selectSynonyms.clear();
    selectSynonymMap.clear();

    selectClauses.clear();
}

unordered_set<string> QueryObject::getAllSynonyms() {
    return allSynonyms;
}

vector<Clause> QueryObject::getGenericClauses() {
    return genericGroup;
}

vector<Clause> QueryObject::getBooleanClauses() {
    return booleanGroup;
}

void QueryObject::setBooleanGroup(ClauseGroup booleanGroup) {
    booleanClauses = booleanGroup;
}

void QueryObject::setGenericGroup(vector<ClauseGroup> genericGroup) {
    genericClauses = genericGroup;
}

void QueryObject::addSelectGroup(string name, Clause::Type type) {
    selectClauses.push_back(make_pair(name, type));
}

vector<string> QueryObject::getSelectSynonyms() {
    return selectSynonyms;
}

Clause::Type QueryObject::getType(string name) {
    return Clause::convertStringToArgType(selectSynonymMap[name]);
}

ClauseGroup QueryObject::getBooleanGroup() {
    return booleanClauses;
}

vector<ClauseGroup> QueryObject::getGenericGroup() {
    return genericClauses;
}

vector<pair<string, Clause::Type>> QueryObject::getSelectGroup() {
    return selectClauses;
}
