#pragma once

#include<string>
#include<vector>

class Clause {
public:
    Clause() = default;

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
    void addSelectArg(std::string type, std::string name);

    /*
        Method to set the relation, arguments types & names within this clause object for a SELECT_TUPLE clause.
    */
    void addSelectTupleArgs(std::vector<std::string> types, std::vector<std::string> names);

    /*
        The following methods are for setting the clause relation, and for setting the 4 arguments' names and types.
    */
    void setRelation(std::string relationStr);
    void setArgumentNames(std::string name1, std::string name2, std::string name3);
    void setArgumentTypes(std::string type1, std::string type2, std::string type3);

    /*
        Method for getting the relation that this clause represents
    */
    Relation getRelation();

    /*
        The following methods are for getting the name or type
        of args 1 to 4 respectively.
    */
    std::string getFirstArg();
    std::string getSecondArg();
    std::string getThirdArg();
    Clause::Type getFirstType();
    Clause::Type getSecondType();
    Clause::Type getThirdType();

    /*
        Methods to return the respective vectors containing the argument types and names.
        To be used if the selection target is a tuple.
    */
    std::vector<Clause::Type> getTupleArgTypes();
    std::vector<std::string> getTupleArgNames();

    Type convertStringToArgType(const std::string str);
    std::string convertArgTypeToString(const Type type);
    Relation convertStringToRelation(const std::string str);
    std::string convertRelationToString(const Relation type);

private:
    Relation relation;

    Type firstArgType, secondArgType, thirdArgType;
    std::string firstArgName, secondArgName, thirdArgName;

    std::vector<Clause::Type> tupleArgTypes;
    std::vector<std::string> tupleArgNames;
};