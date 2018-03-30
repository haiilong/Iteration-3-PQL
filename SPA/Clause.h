#pragma once

#include<string>
#include<vector>
#include<unordered_set>

using namespace std;

class Clause {
public:
    Clause();
    ~Clause();

    enum class Relation {
        AFFECTS,
        AFFECTS_T,
        ASSIGN,
        CALLS,
        CALLS_T,
        FOLLOWS,
        FOLLOWS_T,
        IF,
        INVALID_CLAUSE,
        MODIFIES,
        NEXT,
        NEXT_T,
        PARENT,
        PARENT_T,
        SELECT,
        SELECT_TUPLE,
        USES,
        WHILE,
        WITH
    };

    enum class Type {
        ASSIGN,
        BOOLEAN,
        CALL,
        CALLPROC,
        CONSTANT,
        EMPTY,
        EXPRESSION,
        IDENT,
        IF,
        INTEGER,
        INVALID_ARG,
        PROCEDURE,
        PROG_LINE,
        STMT,
        STMT_LST,
        SUBEXPRESSION,
        VARIABLE,
        WHILE,
        WILDCARD,
    };

    /*
    Method to set the relation, argument type & name within this clause object for a SELECT clause.
    */
    void addSelectArg(string type, string name);

    /*
    Method to set the relation, arguments types & names within this clause object for a SELECT_TUPLE clause.
    */
    void addSelectTupleArgs(vector<string> types, vector<string> names);

    /*
    The following methods are for setting the clause relation, and for setting the 4 arguments' names and types.
    */
    void setRelation(string relationStr, string type1, string type2, string type3, string name1, string name2, string name3);

    /*
    Method for getting the relation that this clause represents
    */
    Relation getRelation();

    /*
    The following methods are for getting the name or type
    of args 1 to 4 respectively.
    */
    string getFirstArg();
    string getSecondArg();
    string getThirdArg();
    Clause::Type getFirstType();
    Clause::Type getSecondType();
    Clause::Type getThirdType();

    /*
    Methods to return the respective vectors containing the argument types and names.
    To be used if the selection target is a tuple.
    */
    vector<Clause::Type> getTupleArgTypes();
    vector<string> getTupleArgNames();

    Type convertStringToArgType(const string str);
    string convertArgTypeToString(const Type type);
    Relation convertStringToRelation(const string str);
    string convertRelationToString(const Relation type);

    // for optimizer
    int getNoOfSyonyms();
    vector<string> getSynonyms();

    string toString();

private:
    Relation relation;

    Type firstArgType, secondArgType, thirdArgType;
    string firstArgName, secondArgName, thirdArgName;

    vector<Clause::Type> tupleArgTypes;
    vector<string> tupleArgNames;

    // for optimizer
    int noOfSynonyms = 0;
    vector<string> synonyms;

    bool isSynonym(Type t);
};