#include "Clause.h"

void Clause::addSelectArg(std::string type, std::string name) {
    relation = Clause::Relation::SELECT;

    firstArgType = convertStringToArgType(type);
    firstArgName = name;

    secondArgType = Clause::Type::EMPTY;
    secondArgName = "";
    thirdArgType = Clause::Type::EMPTY;
    thirdArgName = "";
}

void Clause::addSelectTupleArgs(std::vector<std::string> types, std::vector<std::string> names) {
    relation = Clause::Relation::SELECT_TUPLE;

    for (std::string typeStr : types) {
        tupleArgTypes.push_back(convertStringToArgType(typeStr));
    }

    for (std::string nameStr : names) {
        tupleArgNames.push_back(nameStr);
    }

    firstArgType = Clause::Type::EMPTY;
    firstArgName = "";
    secondArgType = Clause::Type::EMPTY;
    secondArgName = "";
    thirdArgType = Clause::Type::EMPTY;
    thirdArgName = "";
}

void Clause::setRelation(std::string relationStr) {
    relation = convertStringToRelation(relationStr);
}

void Clause::setArgumentNames(std::string name1, std::string name2, std::string name3) {
    firstArgName = name1;
    secondArgName = name2;
    thirdArgName = name3;
}

void Clause::setArgumentTypes(std::string type1, std::string type2, std::string type3) {
    firstArgType = convertStringToArgType(type1);
    secondArgType = convertStringToArgType(type2);
    thirdArgType = convertStringToArgType(type3);
}

Clause::Relation Clause::getRelation() {
    return relation;
}

std::string Clause::getFirstArg() {
    return firstArgName;
}

std::string Clause::getSecondArg() {
    return secondArgName;
}

std::string Clause::getThirdArg() {
    return thirdArgName;
}

Clause::Type Clause::getFirstType() {
    return firstArgType;
}

Clause::Type Clause::getSecondType() {
    return secondArgType;
}

Clause::Type Clause::getThirdType() {
    return thirdArgType;
}

std::vector<Clause::Type> Clause::getTupleArgTypes() {
    return tupleArgTypes;
}

std::vector<std::string> Clause::getTupleArgNames() {
    return tupleArgNames;
}

Clause::Relation Clause::convertStringToRelation(const std::string str) {
    if (str == "Affects") return Clause::Relation::AFFECTS;
    else if (str == "Affects*") return Clause::Relation::AFFECTS_T;
    else if (str == "Assign") return Clause::Relation::ASSIGN;
    else if (str == "Calls") return Clause::Relation::CALLS;
    else if (str == "Calls*") return Clause::Relation::CALLS_T;
    else if (str == "Follows") return Clause::Relation::FOLLOWS;
    else if (str == "Follows*") return Clause::Relation::FOLLOWS_T;
    else if (str == "If") return Clause::Relation::IF;
    else if (str == "Modifies") return Clause::Relation::MODIFIES;
    else if (str == "Next") return Clause::Relation::NEXT;
    else if (str == "Next*") return Clause::Relation::NEXT_T;
    else if (str == "Parent") return Clause::Relation::PARENT;
    else if (str == "Parent*") return Clause::Relation::PARENT_T;
    else if (str == "Select") return Clause::Relation::SELECT;
    else if (str == "SelectTuple") return Clause::Relation::SELECT_TUPLE;
    else if (str == "Uses") return Clause::Relation::USES;
    else if (str == "While") return Clause::Relation::WHILE;
    else if (str == "With") return Clause::Relation::WITH;
    else return Clause::Relation::INVALID_CLAUSE;
}

std::string Clause::convertRelationToString(const Relation type) {
    if (type == Clause::Relation::AFFECTS) return "Affects";
    else if (type == Clause::Relation::AFFECTS_T) return "Affects*";
    else if (type == Clause::Relation::ASSIGN) return "Assign";
    else if (type == Clause::Relation::CALLS) return "Calls";
    else if (type == Clause::Relation::CALLS_T) return "Calls*";
    else if (type == Clause::Relation::FOLLOWS) return "Follows";
    else if (type == Clause::Relation::FOLLOWS_T) return "Follows*";
    else if (type == Clause::Relation::IF) return "If";
    else if (type == Clause::Relation::MODIFIES) return "Modifies";
    else if (type == Clause::Relation::NEXT) return "Next";
    else if (type == Clause::Relation::NEXT_T) return "Next*";
    else if (type == Clause::Relation::PARENT) return "Parent";
    else if (type == Clause::Relation::PARENT_T) return "Parent*";
    else if (type == Clause::Relation::SELECT) return "Select";
    else if (type == Clause::Relation::SELECT_TUPLE) return "SelectTuple";
    else if (type == Clause::Relation::USES) return "Uses";
    else if (type == Clause::Relation::WHILE) return "While";
    else if (type == Clause::Relation::WITH) return "With";
    else return "Invalid_clause";
}

Clause::Type Clause::convertStringToArgType(const std::string str) {
    if (str == "assign") return Clause::Type::ASSIGN;
    else if (str == "boolean") return Clause::Type::BOOLEAN;
    else if (str == "call") return Clause::Type::CALL;
    else if (str == "callproc") return Clause::Type::CALLPROC;
    else if (str == "constant") return Clause::Type::CONSTANT;
    else if (str == "empty") return Clause::Type::EMPTY;
    else if (str == "expression") return Clause::Type::EXPRESSION;
    else if (str == "ident") return Clause::Type::IDENT;
    else if (str == "if") return Clause::Type::IF;
    else if (str == "integer") return Clause::Type::INTEGER;
    else if (str == "procedure") return Clause::Type::PROCEDURE;
    else if (str == "prog_line") return Clause::Type::PROG_LINE;
    else if (str == "stmt") return Clause::Type::STMT;
    else if (str == "stmtlst") return Clause::Type::STMT_LST;
    else if (str == "subexpression") return Clause::Type::SUBEXPRESSION;
    else if (str == "variable") return Clause::Type::VARIABLE;
    else if (str == "while") return Clause::Type::WHILE;
    else if (str == "wildcard") return Clause::Type::WILDCARD;
    else return Clause::Type::INVALID_ARG;
}

std::string Clause::convertArgTypeToString(const Type type) {
    if (type == Clause::Type::ASSIGN) return "assign";
    else if (type == Clause::Type::BOOLEAN) return "boolean";
    else if (type == Clause::Type::CALL) return "call";
    else if (type == Clause::Type::CALLPROC) return "callproc";
    else if (type == Clause::Type::CONSTANT) return "constant";
    else if (type == Clause::Type::EMPTY) return "empty";
    else if (type == Clause::Type::EXPRESSION) return "expression";
    else if (type == Clause::Type::IDENT) return "ident";
    else if (type == Clause::Type::IF) return "if";
    else if (type == Clause::Type::INTEGER) return "integer";
    else if (type == Clause::Type::PROCEDURE) return "procedure";
    else if (type == Clause::Type::PROG_LINE) return "prog_line";
    else if (type == Clause::Type::STMT) return "stmt";
    else if (type == Clause::Type::STMT_LST) return "stmtlst";
    else if (type == Clause::Type::SUBEXPRESSION) return "subexpression";
    else if (type == Clause::Type::VARIABLE) return "variable";
    else if (type == Clause::Type::WHILE) return "while";
    else if (type == Clause::Type::WILDCARD) return "wildcard";
    else return "invalid_arg";
}