#include "Clause.h"

Clause::Clause() {}
Clause::~Clause() {}

void Clause::addSelectArg(string type, string name) {
    relation = Clause::Relation::SELECT;

    firstArgType = convertStringToArgType(type);
    firstArgName = name;

    secondArgType = Clause::Type::EMPTY;
    secondArgName = "";
    thirdArgType = Clause::Type::EMPTY;
    thirdArgName = "";

    synonyms.push_back(name);
    //noOfSynonyms = 1;
}

void Clause::addSelectTupleArgs(vector<string> types, vector<string> names) {
    relation = Clause::Relation::SELECT_TUPLE;

    for (string typeStr : types) {
        tupleArgTypes.push_back(convertStringToArgType(typeStr));
    }

    for (string nameStr : names) {
        tupleArgNames.push_back(nameStr);
    }

    firstArgType = Clause::Type::EMPTY;
    firstArgName = "";
    secondArgType = Clause::Type::EMPTY;
    secondArgName = "";
    thirdArgType = Clause::Type::EMPTY;
    thirdArgName = "";

    synonyms = names;
}

void Clause::setRelation(string relationStr, string type1, string type2, string type3, string name1, string name2, string name3) {
    relation = convertStringToRelation(relationStr);
    firstArgName = name1;
    secondArgName = name2;
    thirdArgName = name3;
    firstArgType = convertStringToArgType(type1);
    secondArgType = convertStringToArgType(type2);
    thirdArgType = convertStringToArgType(type3);

    noOfSynonyms = 0; //to safeguard
    if (isSynonym(firstArgType)) {
        noOfSynonyms += 1;
        synonyms.push_back(firstArgName);
    }
    if (isSynonym(secondArgType)) {
        noOfSynonyms += 1;
        synonyms.push_back(secondArgName);
    }

    // set score
    if (relation == Relation::FOLLOWS) {
        if (noOfSynonyms == 0) score = 1;
        else if (noOfSynonyms == 1) score = 2;
        else if (noOfSynonyms == 2) score = 1000;
    }
    else if (relation == Relation::PARENT) {
        if (noOfSynonyms == 0) score = 1;
        else if (noOfSynonyms == 1) score = 501;
        else if (noOfSynonyms == 2) score = 1000;
    }
    else if (relation == Relation::FOLLOWS_T) {
        if (noOfSynonyms == 0) score = 1;
        else if (noOfSynonyms == 1) score = 1000;
        else if (noOfSynonyms == 2) score = 50500;
    }
    else if (relation == Relation::PARENT_T) {
        if (noOfSynonyms == 0) score = 1;
        else if (noOfSynonyms == 1) score = 800;
        else if (noOfSynonyms == 2) score = 2000;
    }
    else if (relation == Relation::MODIFIES) {
        if (noOfSynonyms == 0) score = 1;
        else if (noOfSynonyms == 1) score = 525;
        else if (noOfSynonyms == 2) score = 5500;
    }
    else if (relation == Relation::USES) {
        if (noOfSynonyms == 0) score = 1;
        else if (noOfSynonyms == 1) score = 550;
        else if (noOfSynonyms == 2) score = 15500;
    }
    else if (relation == Relation::CALLS) {
        if (noOfSynonyms == 0) score = 1;
        else if (noOfSynonyms == 1) score = 11;
        else if (noOfSynonyms == 2) score = 121;
    }
    else if (relation == Relation::CALLS_T) {
        if (noOfSynonyms == 0) score = 1;
        else if (noOfSynonyms == 1) score = 31;
        else if (noOfSynonyms == 2) score = 151;
    }
    else if (relation == Relation::NEXT) {
        if (noOfSynonyms == 0) score = 1;
        else if (noOfSynonyms == 1) score = 2;
        else if (noOfSynonyms == 2) score = 1000;
    }
    else if (relation == Relation::NEXT_T) {
        if (noOfSynonyms == 0) score = 1500;
        else if (noOfSynonyms == 1) score = 2000;
        else if (noOfSynonyms == 2) score = 875000;
    }
    else if (relation == Relation::AFFECTS) {
        if (noOfSynonyms == 0) score = 90000;
        else if (noOfSynonyms == 1) score = 90040;
        else if (noOfSynonyms == 2) score = 20843250;
    }
    else if (relation == Relation::AFFECTS_T) {
        if (noOfSynonyms == 0) score = 20833250;
        else if (noOfSynonyms == 1) score = 20833350;
        else if (noOfSynonyms == 2) score = 20953250;
    }
    else if (relation == Relation::ASSIGN) {
        if (noOfSynonyms == 1) score = 750;
        else if (noOfSynonyms == 2) score = 1000;
    }
    else if (relation == Relation::WHILE) {
        score = 500;
    }
    else if (relation == Relation::IF) {
        score = 500;
    }
    else { //with
        score = 0;
    }

}

Clause::Relation Clause::getRelation() {
    return relation;
}

string Clause::getFirstArg() {
    return firstArgName;
}

string Clause::getSecondArg() {
    return secondArgName;
}

string Clause::getThirdArg() {
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

vector<Clause::Type> Clause::getTupleArgTypes() {
    return tupleArgTypes;
}

vector<string> Clause::getTupleArgNames() {
    return tupleArgNames;
}

Clause::Relation Clause::convertStringToRelation(const string str) {
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

string Clause::convertRelationToString(const Relation type) {
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

int Clause::getNoOfSyonyms() {
    return noOfSynonyms;
}

vector<string> Clause::getSynonyms() {
    return synonyms;
}

int Clause::getScore() {
    return score;
}

bool Clause::isSynonym(Type t) {
    switch (t) {
    case Type::ASSIGN:
    case Type::CALL:
    case Type::CALLPROC:
    case Type::CONSTANT:
    case Type::IF:
    case Type::PROCEDURE:
    case Type::PROG_LINE:
    case Type::STMT:
    case Type::STMT_LST:
    case Type::VARIABLE:
    case Type::WHILE:
        return true;
    default:
        return false;
    }
}

string Clause::toString() {
    switch (relation) {
    case Relation::IF:
    case Relation::ASSIGN:
    case Relation::WHILE:
        return firstArgName + "(" + secondArgName + "," + secondArgName + ")";
    case Relation::WITH:
        return firstArgName + "=" + secondArgName;
    default:
        return convertRelationToString(relation) + "(" + firstArgName + "," + secondArgName + ")";
    }
}

Clause::Type Clause::convertStringToArgType(const string str) {
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

string Clause::convertArgTypeToString(const Type type) {
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