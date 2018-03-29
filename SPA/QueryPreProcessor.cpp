#include "QueryPreProcessor.h"

QueryPreProcessor::QueryPreProcessor() {
    initRelationTable();
}

QueryPreProcessor::~QueryPreProcessor() {
}

/**
* Validate a query string, return true or false.
*/
bool QueryPreProcessor::validate(const string &query) {
    clear();

    // split by ";", the last token is the query clause, the rest are declaration clauses
    vector<string> queryTokens = StringUtility::split(query, ";");

    // validate each declaration one by one (more efficient), each declaration looks like: design-entity synonym (‘,’ synonym)*
    bool isValidDeclaration = true;
    for (size_t i = 0; i < queryTokens.size() - 1; i++) {
        isValidDeclaration = validateDeclaration(queryTokens[i]);
        if (!isValidDeclaration) {
            return false;
        }
    }

    // try to split clauses into different types of clauses
    if (!parseQueryClause(queryTokens.back())) {
        return false;
    }

    if (!validateSelectCl()) {
        return false;
    }

    for (const auto &clause : withCl) {
        if (!validateWithCl(clause)) {
            return false;
        }
    }

    for (const auto &clause : patternCl) {
        if (!validatePatternCl(clause)) {
            return false;
        }
    }

    for (const auto &clause : suchThatCl) {
        if (!validateSuchThatCl(clause)) {
            return false;
        }
    }

    return true;
}

QueryObject QueryPreProcessor::getQueryObject() {
    return queryObject;
}

/**
* Validate declaration of synonyms, return true or false.
*/
bool QueryPreProcessor::validateDeclaration(const string &declaration) {
    // split declaration into design-entity and synonym
    vector<string> declarationTokens = StringUtility::breakOn(declaration, " ");
    string designEntity = declarationTokens[0];
    string synonyms = declarationTokens[1];

    // validate design-entity
    if (!isDesignEntity(designEntity)) {
        return false;
    }

    // validate each declared synonym in the list
    vector<string> synonymList = StringUtility::split(synonyms, ",");
    string synonym;
    for (size_t i = 0; i < synonymList.size(); i++) {
        synonym = synonymList[i];

        // validate synonym's naming grammar
        if (!isSynonym(synonym)) {
            return false;
        }

        // check if synonym is already declared
        if (synonymTable.find(synonym) != synonymTable.end()) {
            return false;
        }

        // add synonym into synonymTable
        synonymTable.insert(make_pair(synonym, designEntity));
    }

    return true;
}

/**
* Parse the query clause into 3 parts
*/
bool QueryPreProcessor::parseQueryClause(const string &queryClause) {
    //high level structural validation
    const regex structure("^Select\\s+(\\S+|(<\\s*\\S+(\\s*,\\s*\\S+)*\\s*>))(\\s+((such\\s+that\\s+(\\S+\\s*\\(\\s*\\S+\\s*,\\s*\\S+\\s*\\)"
        "(\\s+and\\s+\\S+\\s*\\(\\s*\\S+\\s*,\\s*\\S+\\s*\\))*))|(with\\s+(\\S+\\s*=\\s*\\S+(\\s+and\\s+\\S+\\s*=\\s*\\S+)*))"
        "|(pattern\\s+(\\S+\\s*\\(\\s*\\S+\\s*,\\s*(_|(\".*\")|(_\".*\"_))\\s*(,\\s*_\\s*)?\\)(\\s+and\\s+\\S+\\s*\\(\\s*\\S+\\s*,\\s*(_|(\".*\")|(_\".*\"_))\\s*(,\\s*_\\s*)?\\))*))))*$");

    if (!regex_match(queryClause, structure)) {
        return false;
    }

    // after this step, the clause is confirmed to be structurally correct
    // we now extract the clauses and put them onto the corresponding vector

    // regex to search for different types of clauses
    const regex select("Select\\s+((<\\s*\\S+(\\s*,\\s*\\S+)*\\s*>)|\\S+)");
    const regex suchThat("such\\s+that\\s+(\\S+\\s*\\(\\s*\\S+\\s*,\\s*\\S+\\s*\\)(\\s+and\\s+\\S+\\s*\\(\\s*\\S+\\s*,\\s*\\S+\\s*\\))*)");
    const regex pattern("pattern\\s+(\\S+\\s*\\(\\s*\\S+\\s*,\\s*(_|(\".*\")|(_\".*\"_))\\s*(,\\s*_\\s*)?\\)(\\s+and\\s+\\S+\\s*\\(\\s*\\S+\\s*,\\s*(_|(\".*\")|(_\".*\"_))\\s*(,\\s*_\\s*)?\\))*)");
    const regex with("\\S+\\s*=\\s*\\S+(\\s+and\\s+\\S+\\s*=\\s*\\S+)*");

    // regex iterator
    smatch sm;
    sregex_iterator it(queryClause.begin(), queryClause.end(), suchThat);
    sregex_iterator it2(queryClause.begin(), queryClause.end(), pattern);
    sregex_iterator it3(queryClause.begin(), queryClause.end(), with);
    sregex_iterator reg_end;

    // extract Select Clause
    regex_search(queryClause, sm, select);
    selectCl = sm.str();
    selectCl = selectCl.substr(selectCl.find_first_of(" \t") + 1);

    // extract such that Clause
    for (; it != reg_end; ++it) {
        string curr = it->str();
        curr = curr.substr(curr.find_first_of(" \t") + 1);
        curr = curr.substr(curr.find_first_of(" \t") + 1);
        vector<string> tokens = StringUtility::split(curr, "and");
        for (auto v : tokens) {
            suchThatCl.push_back(v);
        }
    }

    // extract pattern Clause
    for (; it2 != reg_end; ++it2) {
        string curr = it2->str();
        curr = curr.substr(curr.find_first_of(" \t") + 1);
        vector<string> tokens = StringUtility::split(curr, "and");
        for (auto v : tokens) {
            patternCl.push_back(v);
        }
    }

    // extract with clause
    for (; it3 != reg_end; ++it3) {
        vector<string> tokens = StringUtility::split(it3->str(), "and");
        for (auto v : tokens) {
            withCl.push_back(v);
        }
    }

    return true;
}

/**
* Validate select clause, return true or false. Also update Query Object.
*/
bool QueryPreProcessor::validateSelectCl() {
    if (selectCl[0] == '<') {
        if (selectCl.size() < 5) {
            return false;
        }
        if (selectCl[selectCl.size() - 1] != '>') {
            return false;
        }
        selectCl = selectCl.substr(1, selectCl.size()-2);
        vector<string> args = StringUtility::split(selectCl, ",");
        vector<string> argName;
        vector<string> argType;
        for (auto item : args) {
            string type = getSelectType(item);

            if (type == "invalid") {
                return false;
            }

           argName.push_back(getSynFromAttr(item));
           argType.push_back(type);
        }
        queryObject.addSelectTupleClause(argType, argName);
    }
    else {
        if (selectCl == "BOOLEAN") {
            queryObject.addSelectClause("boolean", "BOOLEAN");
            return true;
        }

        string select = getSelectType(selectCl);

        if (select == "invalid") {
            return false;
        }

        // add result clause to query object
        queryObject.addSelectClause(select, getSynFromAttr(selectCl));
    }
    return true;
}

/**
* Validate the such that clause, return true or false. Also update Query Object.
*/
bool QueryPreProcessor::validateSuchThatCl(const string &clause) {
    int openingBrace = clause.find_first_of("(");
    int comma = clause.find_first_of(",");
    int closingBrace = clause.length() - 1;

    // parse such that
    string relation = StringUtility::trim(clause.substr(0, openingBrace));
    string firstArgument = StringUtility::trim(clause.substr(openingBrace + 1, comma - openingBrace - 1));
    string secondArgument = StringUtility::trim(clause.substr(comma + 1, closingBrace - comma - 1));
    string firstArgType = getType(firstArgument);
    string secondArgType = getType(secondArgument);

    // validate relationship based on RelationTable
    if (!isValidRelation(relation, firstArgType, secondArgType)) {
        return false;
    }

    // trim ident for QO
    if (firstArgType == "ident") {
        firstArgument = convertIdent(firstArgument);
    }
    if (secondArgType == "ident") {
        secondArgument = convertIdent(secondArgument);
    }

    // pass clause to object
    queryObject.addClause(relation, firstArgType, firstArgument, secondArgType, secondArgument);

    return true;
}

/**
* Validate the pattern clause, return true or false. Also update Query Object.
*/
bool QueryPreProcessor::validatePatternCl(const string &clause) {
    int openingBrace = clause.find_first_of("(");
    int comma = clause.find_first_of(",");
    int closingBrace = clause.length() - 1;

    string relation = StringUtility::trim(clause.substr(0, openingBrace));
    string relationType = getType(relation);

    string firstArgument;
    string secondArgument;
    string firstArgType;
    string secondArgType;

    if (relationType == "assign") {
        firstArgument = StringUtility::trim(clause.substr(openingBrace + 1, comma - openingBrace - 1));
        secondArgument = StringUtility::trim(clause.substr(comma + 1, closingBrace - comma - 1));
        firstArgType = getType(firstArgument);
        secondArgType = getExpressionType(secondArgument);

        // validate relationship based on RelationTable
        if (!isValidRelation(relationType, firstArgType, secondArgType)) {
            return false;
        }

        if (firstArgType == "ident") {
            firstArgument = convertIdent(firstArgument);
        }
        if (secondArgType == "expression") {
            secondArgument = convertExpression(secondArgument);
        }
        else if (secondArgType == "subexpression") {
            secondArgument = convertSubExpression(secondArgument);
        }
        queryObject.addPatternClause("Assign", "assign", relation, firstArgType, firstArgument, secondArgType, secondArgument);
    }
    else if (relationType == "while") {
        firstArgument = StringUtility::trim(clause.substr(openingBrace + 1, comma - openingBrace - 1));
        secondArgument = StringUtility::trim(clause.substr(comma + 1, closingBrace - comma - 1));
        firstArgType = getType(firstArgument);

        // validate relationship based on RelationTable
        vector<string> whileList = firstArgMap["while"];

        if (find(whileList.begin(), whileList.end(), firstArgType) == whileList.end()) {
            return false;
        }
        if (secondArgument != "_") {
            return false;
        }

        if (firstArgType == "ident") {
            firstArgument = convertIdent(firstArgument);
        }
        queryObject.addPatternClause("While", "while", relation, firstArgType, firstArgument, "wildcard", "_");
    }
    else if (relationType == "if") {
        firstArgument = StringUtility::trim(clause.substr(openingBrace + 1, comma - openingBrace - 1));
        firstArgType = getType(firstArgument);

        // validate relationship based on RelationTable
        vector<string> ifList = firstArgMap["if"];

        if (find(ifList.begin(), ifList.end(), firstArgType) == ifList.end()) {
            return false;
        }

        secondArgument = clause.substr(comma + 1, closingBrace - comma - 1);
        secondArgument.erase(remove_if(secondArgument.begin(), secondArgument.end(), isspace), secondArgument.end());

        if (secondArgument != "_,_") {
            return false;
        }

        if (firstArgType == "ident") {
            firstArgument = convertIdent(firstArgument);
        }

        queryObject.addPatternClause("If", "if", relation, firstArgType, firstArgument, "wildcard", "_");
    }
    else {
        return false;
    }
    return true;
}

/**
* Validate the with clause, return true or false. Also update Query Object.
*/
bool QueryPreProcessor::validateWithCl(const string & s) {
    vector<string> args = StringUtility::split(s, "=");

    string arg1 = getWithType(args[0]);
    string arg2 = getWithType(args[1]);

    if (arg1 == "invalid" || arg2 == "invalid") {
        return false;
    }

    unordered_set<string> intSet = { "prog_line", "stmt", "assign", "while", "if", "constant", "integer", "call" };
    unordered_set<string> stringSet = { "procedure", "callproc", "variable", "ident" };

    if (intSet.find(arg1) != intSet.end()) {
        if (intSet.find(arg2) == intSet.end()) {
            return false;
        }
    }
    else if (stringSet.find(arg1) != stringSet.end()) {
        if (stringSet.find(arg2) == stringSet.end()) {
            return false;
        }
    }
    else {
        return false;
    }

    if (arg1 == "integer" || arg1 == "ident") {
        queryObject.addClause("With", arg2, getSynFromAttr(args[1]), arg1, getSynFromAttr(args[0]));
    }
    else {
        queryObject.addClause("With", arg1, getSynFromAttr(args[0]), arg2, getSynFromAttr(args[1]));
    }
    return true;
}

/**
*
*/
/**
* Get type for a name
*/
string QueryPreProcessor::getType(const string &name) {
    if (name == "_") {
        return "wildcard";
    }
    if (isSynonym(name)) {
        if (synonymTable.find(name) != synonymTable.end()) {
            return synonymTable[name];
        }
        else {
            return "invalid";
        }
    }
    if (isConstValue(name)) {
        return "integer";
    }
    if (isIdent(name)) {
        return "ident";
    }
    return "invalid";
}

/**
* Check if a relation with the specified 2 argument types are valid by using relation table
*/
bool QueryPreProcessor::isValidRelation(const string &relation, const string &firstArg, const string &secondArg) {
    if (firstArgMap.find(relation) == firstArgMap.end()) {
        return false;
    }
    vector<string> firstList = firstArgMap[relation];
    vector<string> secondList = secondArgMap[relation];

    if (find(firstList.begin(), firstList.end(), firstArg) == firstList.end()) {
        return false;
    }
    else if (find(secondList.begin(), secondList.end(), secondArg) == secondList.end()) {
        return false;
    }

    return true;
}

/**
* Check if a string is a design-entity, return true or false.
*/
bool QueryPreProcessor::isDesignEntity(const string &s) {
    vector<string> designEntityList = { "stmt", "assign", "if", "while", "variable", "constant", "prog_line", "call", "procedure", "stmtLst" };
    return find(designEntityList.begin(), designEntityList.end(), s) != designEntityList.end();
}

/**
* Check if a string is a valid synonym name, return true or false.
*/
bool QueryPreProcessor::isSynonym(const string &s) {
    regex synonymName("^[a-zA-Z][a-zA-Z0-9#]*$");
    return regex_match(s, synonymName);
}

/**
* Check if a string is a valid variable name, return true or false.
*/
bool QueryPreProcessor::isVarName(const string &s) {
    regex varName("^[a-zA-Z][a-zA-Z0-9]*$");
    return regex_match(s, varName);
}

/**
* Check if a string is a valid constant value, return true or false.
*/
bool QueryPreProcessor::isConstValue(const string &s) {
    regex constantValue("[0-9]+");
    return regex_match(s, constantValue);
}

/**
* Check if a string is a valid factor, return true or false.
*/
bool QueryPreProcessor::isFactor(const string &s) {
    return (isVarName(s) || isConstValue(s));
}

/**
* Check if a string is of type ident (defined by QO)
*/
bool QueryPreProcessor::isIdent(const string &s) {
    if (s.size() <= 2) {
        return false;
    }
    else if (!(s[0] == '"' && s[s.size() - 1] == '"')) {
        return false;
    }
    else if (!(isVarName(s.substr(1, s.size() - 2)))) {
        return false;
    }
    return true;
}

/**
* find what type of expression a string is
*/
string QueryPreProcessor::getExpressionType(const string &s) {
    if (s == "_") {
        return "wildcard";
    }
    else if (isExpression(s)) {
        return "expression";
    }
    else if (isSubExpression(s)) {
        return "subexpression";
    }
    return "invalid";
}

/**
* Check if a string is of type expression (defined by QO)
*/
bool QueryPreProcessor::isExpression(const string &s) {
    if (s.size() <= 2) {
        return false;
    }
    else if (!(s[0] == '"' && s[s.size() - 1] == '"')) {
        return false;
    }
    else if (!(ExpressionUtility::verify(s.substr(1, s.size() - 2)))) {
        return false;
    }
    return true;
}

/**
* Check if a string is of type subexpression (defined by QO)
*/
bool QueryPreProcessor::isSubExpression(const string &s) {
    if (s.size() <= 4) {
        return false;
    }
    else if (!(s.substr(0, 2) == "_\"" && s.substr(s.size() - 2, 2) == "\"_")) {
        return false;
    }
    else if (!(ExpressionUtility::verify(s.substr(2, s.size() - 4)))) {
        return false;
    }
    return true;
}

/**
* remove quotations around ident
*/
string QueryPreProcessor::convertIdent(const string &s) {
    return s.substr(1, s.size() - 2);
}

/**
* convert expression to postfix
*/
string QueryPreProcessor::convertExpression(const string &s) {
    return ExpressionUtility::toPostFix(s.substr(1, s.size() - 2));
}

/**
* convert subexpression to postfix
*/
string QueryPreProcessor::convertSubExpression(const string &s) {
    return ExpressionUtility::toPostFix(s.substr(2, s.size() - 4));
}

/**
*
*/
string QueryPreProcessor::parseAttrRef(const string &s) {
    int dot = s.find_first_of(".");
    if (dot == -1) {
        return "invalid";
    }
    string first = s.substr(0, dot);
    string second = s.substr(dot + 1, s.length() - 1);

    string firstType;

    if (isSynonym(first)) {
        if (synonymTable.find(first) != synonymTable.end()) {
            firstType = synonymTable[first];
        }
        else {
            return "invalid";
        }
    }
    if (second == "procName") {
        if (firstType == "procedure") {
            return "procedure";
        }
        else if (firstType == "call") {
            return "callproc";
        }
        else {
            return "invalid";
        }
    }
    else if (second == "varName" && firstType == "variable") {
        return firstType;
    }
    else if (second == "value" && firstType == "constant") {
        return firstType;
    }
    else if (second == "stmt#" && (firstType == "assign" || firstType == "stmt" || firstType == "prog_line" || firstType == "call" || firstType == "if" || firstType == "while")) {
        return firstType;
    }

    return "invalid";
}

/**
*
*/
string QueryPreProcessor::getSelectType(const string &s) {
    if (isSynonym(s)) {
        if (synonymTable.find(s) != synonymTable.end()) {
            return synonymTable[s];
        }
        else {
            return "invalid";
        }
    }
    else {
        return parseAttrRef(s);
    }
}

/**
*
*/
string QueryPreProcessor::getWithType(const string &s) {
    if (isConstValue(s)) {
        return "integer";
    }
    else if (isIdent(s)) {
        return "ident";
    }
    else if (isSynonym(s)) {
        if (synonymTable.find(s) != synonymTable.end()) {
            if (synonymTable[s] == "prog_line") {
                return "prog_line";
            }
            else {
                return "invalid";
            }
        }
        else {
            return "invalid";
        }
    }
    else {
        return parseAttrRef(s);
    }
}

/**
*
*/
string QueryPreProcessor::getSynFromAttr(const string &s) {
    int dot = s.find_first_of(".");
    if (dot == -1) {
        if (isIdent(s)) {
            return convertIdent(s);
        }
        else {
            return s;
        }
    }
    return s.substr(0, dot);
}


/**
* Initialize the relation table, this is done once whenever QueryPreProcessor is initialized
*/
void QueryPreProcessor::initRelationTable() {
    firstArgMap["Follows"] = { "call", "stmt", "assign", "while", "if", "prog_line", "wildcard", "integer" };
    secondArgMap["Follows"] = { "call", "stmt", "assign", "while", "if", "prog_line", "wildcard", "integer" };

    firstArgMap["Follows*"] = { "call", "stmt", "assign", "while", "if", "prog_line", "wildcard", "integer" };
    secondArgMap["Follows*"] = { "call", "stmt", "assign", "while", "if", "prog_line", "wildcard", "integer" };

    firstArgMap["Parent"] = { "stmt", "while", "if", "prog_line", "wildcard", "integer" };
    secondArgMap["Parent"] = { "call", "stmt", "assign", "while", "if", "prog_line", "wildcard", "integer" };

    firstArgMap["Parent*"] = { "stmt", "while", "if", "prog_line", "wildcard", "integer" };
    secondArgMap["Parent*"] = { "call", "stmt", "assign", "while", "if", "prog_line", "wildcard", "integer" };

    firstArgMap["Modifies"] = { "procedure", "call", "if", "stmt", "while", "assign", "prog_line", "integer" , "ident" };
    secondArgMap["Modifies"] = { "variable", "wildcard", "ident" };

    firstArgMap["Uses"] = { "procedure", "call", "if", "stmt", "while", "assign", "prog_line", "integer" , "ident" };
    secondArgMap["Uses"] = { "variable", "wildcard", "ident" };

    firstArgMap["Calls"] = { "procedure", "wildcard", "ident" };
    secondArgMap["Calls"] = { "procedure", "wildcard", "ident" };

    firstArgMap["Calls*"] = { "procedure", "wildcard", "ident" };
    secondArgMap["Calls*"] = { "procedure", "wildcard", "ident" };

    firstArgMap["Next"] = { "call", "stmt", "assign", "while", "if", "prog_line", "wildcard", "integer" };
    secondArgMap["Next"] = { "call", "stmt", "assign", "while", "if", "prog_line", "wildcard", "integer" };

    firstArgMap["Next*"] = { "call", "stmt", "assign", "while", "if", "prog_line", "wildcard", "integer" };
    secondArgMap["Next*"] = { "call", "stmt", "assign", "while", "if", "prog_line", "wildcard", "integer" };

    firstArgMap["Affects"] = { "stmt", "assign", "integer", "prog_line", "wildcard" };
    secondArgMap["Affects"] = { "stmt", "assign", "integer", "prog_line", "wildcard" };

    firstArgMap["Affects*"] = { "stmt", "assign", "integer", "prog_line", "wildcard" };
    secondArgMap["Affects*"] = { "stmt", "assign", "integer", "prog_line", "wildcard" };

    firstArgMap["assign"] = { "variable", "wildcard", "ident" };
    secondArgMap["assign"] = { "wildcard", "expression", "subexpression" };

    firstArgMap["while"] = { "variable", "wildcard", "ident" };

    firstArgMap["if"] = { "variable", "wildcard", "ident" };

}

/**
* Clear synonym table and query object after every query
*/
void QueryPreProcessor::clear() {
    synonymTable.clear();
    suchThatCl.clear();
    patternCl.clear();
    withCl.clear();
    queryObject.clear();
}