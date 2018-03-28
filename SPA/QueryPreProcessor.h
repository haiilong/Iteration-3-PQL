#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <regex>

#include "QueryObject.h"
#include "StringUtility.h"
#include "ExpressionUtility.h"

using namespace std;

class QueryPreProcessor {

public:
    QueryPreProcessor();
    ~QueryPreProcessor();

    bool validate(const string &query);

    QueryObject getQueryObject();


    void clear();


private:
    bool validateDeclaration(const string &declaration);
    bool parseQueryClause(const string &queryClause);
    bool validateSelectCl();
    bool validateSuchThatCl(const string &s);
    bool validatePatternCl(const string &s);
    bool validateWithCl(const string &s);

    string getType(const string &name);
    bool isValidRelation(const string &relation, const string &firstArg, const string &secondArg);

    QueryObject queryObject;

    bool isDesignEntity(const string &s);
    bool isSynonym(const string &s);
    bool isVarName(const string &s);
    bool isConstValue(const string &s);
    bool isFactor(const string &s);
    bool isIdent(const string &s);

    string getExpressionType(const string &s);

    bool isExpression(const string &s);
    bool isSubExpression(const string &s);

    string convertIdent(const string & s);
    string convertExpression(const string & s);
    string convertSubExpression(const string & s);
    string parseAttrRef(const string & s);
    string getSelectType(const string & s);
    string getWithType(const string & s);
    string getSynFromAttr(const string & s);

    void initRelationTable();

    unordered_map<string, string> synonymTable;
    unordered_map<string, vector<string>> firstArgMap;
    unordered_map<string, vector<string>> secondArgMap;

    string selectCl;
    vector<string> suchThatCl;
    vector<string> patternCl;
    vector<string> withCl;
};