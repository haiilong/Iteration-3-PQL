#include "SourceParser.h"

#include "PKB.h"


/* INIT */

SourceParser* SourceParser::instance = NULL;

// private initialiser
SourceParser::SourceParser(IPKB &pkb) {
    this->pkb = &pkb;
}

// clear instance
void SourceParser::clear() {
    SourceParser::instance = NULL;
}

// returns a singleton instance
SourceParser* SourceParser::getInstance(IPKB &pkb) {
    if(instance == 0) {
        instance = new SourceParser(pkb);
    }
    return instance;
}

// default initialiser
SourceParser* SourceParser::getInstance() {
    return SourceParser::getInstance(*PKB::getPKBInstance());
}

/* END OF INIT */

/* PUBLIC METHODS */

bool SourceParser::parse(SourceCode source) {
    std::vector<RawToken> rawTokens = StringUtility::tokenize(source, "{};()+=-*");
    TokenList tokens;
    for(RawToken rawToken : rawTokens) {
        tokens.push_back(new SourceToken(rawToken));
    }
    tokenIterator = tokens.begin();
    tokenEnd = tokens.end();
    return buildProgram();
}

/* END OF PUBLIC METHODS */

/* PRIVATE METHODS */

// utility methods

/*
/ set the follows relationship on each statement in stmtList
*/
void SourceParser::setFollows(StatementList& stmtList) {
    if(stmtList.size() == 0) return;
    StatementListIterator it = stmtList.begin();
    Statement followee = *it;
    it++;
    while(it != stmtList.end()) {
        Statement follower = *it;
        pkb->setFollows(followee.n, follower.n);

        followee = follower;
        it++;
    }
}

/* 
/ set the Statement parent as the parent of 
/ all statements in the StatementList children
*/
void SourceParser::setParent(Statement parent, StatementList& children) {
    for(Statement child : children) {
        pkb->setParent(parent.n, child.n);
    }
}

/*
/ set the userindex as the user of variable used
*/
void SourceParser::setUses(UserIndex user, Variable used) {
    pkb->setProcUses(currentProcedure, used);
    pkb->setUses(user.n, used);
}

/*
/ set the userindex as the user of all variables in used
*/
void SourceParser::setUses(UserIndex modifier, std::unordered_set<Variable> used) {
    for(Variable var : used) {
        setUses(modifier, var);
    }
}

/*
/ set the modifier as the modifier of the variable modified
*/
void SourceParser::setModifies(ModifierIndex modifier, Variable modified) {
    pkb->setProcModifies(currentProcedure, modified);
    pkb->setModifies(modifier.n, modified);
}

/*
/ set the modifier as the modifier of all variables in modified
*/
void SourceParser::setModifies(ModifierIndex modifier, std::unordered_set<Variable> modified) {
    for(Variable var : modified) {
        setModifies(modifier, var);
    }
}

void SourceParser::setNext(StatementList& stmtList) {
    if(stmtList.size() == 0) return;
    StatementListIterator it = stmtList.begin();
    Statement n1 = *it;
    it++;
    while(it != stmtList.end()) {
        Statement n2 = *it;
        setNext(n1, n2.n);

        n1 = n2;
        it++;
    }
}


void SourceParser::setNext(Statement s, LineNumber n) {
    if(s.last.size() == 0) {
        // if this is not an if statement
        setNext(s.n, n);
    }
    else {
        // otherwise recursively set next for each of the 
        // last statements in the then and else statement lists
        for(Statement child : s.last) {
            setNext(child, n);
        }
    }
}

void SourceParser::setNext(LineNumber n1, LineNumber n2) {
    pkb->setNext(n1, n2);
}

void SourceParser::setRelationForParentStatement(Statement parent, StatementList& children) {
    setNext(children);
    setFollows(children);
    setParent(parent, children);
    for(Statement child : children) {
        // set parent to modify and use every variable modified by child
        setModifies(parent, pkb->getModifiedByStmt(child.n));
        setUses(parent, pkb->getUsedByStmt(child.n));
    }
}

// read the token and increment the iterator (consume the token)
SourceToken* SourceParser::getToken() {
    if(tokenIterator == tokenEnd) return NULL;
    SourceToken *token = *tokenIterator;
    tokenIterator++;
    return token;
}

SourceToken* SourceParser::readCurrentToken() {
    if(next_token == NULL) throw BadTokenException("Expected a token but there are no tokens left");
    return next_token;
}

/* match token will 
// 1) check if the next unconsumed token matches TokenType type
// 2) consume the token
// 3a) return the consumed token if matched
// 3b) throw exception if not matched
*/
SourceToken* SourceParser::matchToken(TokenType type) {
    if(next_token == NULL) {
        std::string err = "Expected " + SourceToken::getTextForEnum(type) + " but there are no tokens left";
        throw BadTokenException(err);
    }
    SourceToken *current = next_token;
    TokenType currentType = readCurrentToken()->getType();
    if(currentType == type) {
        next_token = getToken();
        return current;
    }
    else {
        std::string err = "Expected " + SourceToken::getTextForEnum(type) + " got " + current->getRawToken();
        throw BadTokenException(err);
    }
}

// build program with tokens
bool SourceParser::buildProgram() {
    next_token = getToken();
    while(next_token != NULL) {
        Procedure p = buildProcedure();
    }
    DesignExtractor::setPostParsingRelations();
    return true;
}

Procedure SourceParser::buildProcedure() {
    matchToken(PROCEDURE_TOKEN);
    RawToken procedureName = matchToken(VARIABLE_TOKEN)->getRawToken();
    Procedure proc = createProcedure(procedureName);
    currentProcedure = proc;

    matchToken(OPEN_BRACE_TOKEN);
    StatementList *statementList = getStmtList();
    
    if(statementList->size() == 0) throw EmptyStatementListException("Procedure statement list is empty");

    setFollows(*statementList);
    setNext(*statementList);
    matchToken(CLOSE_BRACE_TOKEN);
    return proc;
}

StatementList* SourceParser::getStmtList() {
    TokenType nextTokenType = readCurrentToken()->getType();
    switch(nextTokenType) {
        case CLOSE_BRACE_TOKEN: {
            return new StatementList();
        }
        case VARIABLE_TOKEN: {
            // build an assignment statement
            Statement stmt = buildAssignStatement();
            StatementList *rest = getStmtList();
            rest->push_front(stmt);
            return rest;
        }
        case IF_TOKEN: {
            Statement s = buildIfStatement();
            StatementList *rest = getStmtList();
            rest->push_front(s);
            return rest;
        }
        case WHILE_TOKEN: {
            Statement s = buildWhileStatement();
            StatementList *rest = getStmtList();
            rest->push_front(s);
            return rest;
        }
        case CALL_TOKEN: {
            Statement s = buildCallStatement();
            StatementList *rest = getStmtList();
            rest->push_front(s);
            return rest;
        }
        default: {
            std::string err = "Unexpected statement type spotted: " + next_token->getRawToken();
            throw BadTokenException(err);
        }
    }
}

Statement SourceParser::buildCallStatement() {
    matchToken(CALL_TOKEN);
    SourceToken *c = matchToken(VARIABLE_TOKEN);
    Procedure p = createProcedure(c->getRawToken());
    matchToken(SEMI_COLON_TOKEN);

    Statement res = createCallStatement(p);
    currentLineNumber++;
    return res;
}

Statement SourceParser::buildWhileStatement() {
    matchToken(WHILE_TOKEN);
    SourceToken *c = matchToken(VARIABLE_TOKEN);

    Variable control = createVariable(c->getRawToken(), currentLineNumber);
    Statement whileStmt = createWhileStatement(control);
    currentLineNumber++;

    matchToken(OPEN_BRACE_TOKEN);

    StatementList *whileStmtList = getStmtList();
    if(whileStmtList->size() == 0) throw EmptyStatementListException("while statement list is empty");
    setRelationForParentStatement(whileStmt, *whileStmtList);
    setNext(whileStmt.n, whileStmtList->front().n);
    setNext(whileStmtList->back(), whileStmt.n);

    matchToken(CLOSE_BRACE_TOKEN);

    return whileStmt;
}

Statement SourceParser::buildIfStatement() {
    matchToken(IF_TOKEN);
    SourceToken *c = matchToken(VARIABLE_TOKEN);
    
    Variable control = createVariable(c->getRawToken(), currentLineNumber);
    Statement ifStmt = createIfStatement(control);
    currentLineNumber++;

    matchToken(THEN_TOKEN);
    matchToken(OPEN_BRACE_TOKEN);

    StatementList *thenStmtList = getStmtList();
    if(thenStmtList->size() == 0) throw EmptyStatementListException("then statement list is empty");
    setRelationForParentStatement(ifStmt, *thenStmtList);
    setNext(ifStmt.n, thenStmtList->front().n);
    ifStmt.last.push_back(thenStmtList->back());

    matchToken(CLOSE_BRACE_TOKEN);

    matchToken(ELSE_TOKEN);
    matchToken(OPEN_BRACE_TOKEN);

    StatementList *elseStmtList = getStmtList();
    if(elseStmtList->size() == 0) throw EmptyStatementListException("else statement list is empty");
    setRelationForParentStatement(ifStmt, *elseStmtList);
    setNext(ifStmt.n, elseStmtList->front().n);
    ifStmt.last.push_back(elseStmtList->back());

    matchToken(CLOSE_BRACE_TOKEN);

    return ifStmt;
}

Statement SourceParser::buildAssignStatement() {
    SourceToken *LHS = matchToken(VARIABLE_TOKEN);
    matchToken(EQUAL_TOKEN);
    
    TokenList expr;
    while(readCurrentToken()->getType() != SEMI_COLON_TOKEN) {
        expr.push_back(next_token);
        next_token = getToken();
    }
    TokenVector v(std::begin(expr), std::end(expr));
    if (!ExpressionUtility::verify(v)) throw BadTokenException("Bad expression");

    matchToken(SEMI_COLON_TOKEN);

    Statement s = createAssignStatement(LHS, expr);

    currentLineNumber++;

    return s;
}

Procedure SourceParser::createProcedure(RawToken procedureName) {
    return pkb->insertProc(procedureName);
}

Statement SourceParser::createCallStatement(Procedure p) {
    Statement s = Statement(currentLineNumber, std::vector<Statement>());
    pkb->insertStmt(currentLineNumber, "CALL", currentProcedure);
    pkb->setCalls(currentLineNumber, currentProcedure, p);

    return s;
}

/* returns the line number of the assign statement
// calls createVariable for the LHS and buildExpressionTree for the RHS
// calls insertStatement
*/
Statement SourceParser::createAssignStatement(SourceToken *LHS, TokenList RHS) {
    // LHS
    Variable l = createVariable(LHS->getRawToken(), currentLineNumber);
    Statement s = Statement(currentLineNumber, std::vector<Statement>());
    setModifies(s, l);

    // RHS
    ExpressionPattern pattern = createExpressionPattern(s, RHS);
    pkb->insertPatternExpression(currentLineNumber, pattern);

    // create statement in pkb
    pkb->insertStmt(currentLineNumber, "ASSIGN", currentProcedure);
    
    return s;
}

Statement SourceParser::createIfStatement(Variable control) {
    Statement s = Statement(currentLineNumber, std::vector<Statement>());
    setUses(s, control);
    pkb->insertStmt(currentLineNumber, "IF", currentProcedure);
    pkb->setControlVariable(currentLineNumber, control);
    return s;
}

Statement SourceParser::createWhileStatement(Variable control) {
    Statement s = Statement(currentLineNumber, std::vector<Statement>());
    setUses(s, control);
    pkb->insertStmt(currentLineNumber, "WHILE", currentProcedure);
    pkb->setControlVariable(currentLineNumber, control);
    return s;
}

// returns the index of the variable in the pkb
// calls insertVar() method in the pkb
Variable SourceParser::createVariable(VariableName& name, LineNumber n) {
    return pkb->insertVar(name);
}

// returns the index of the variable in the pkb
// calls insertVar() method in the pkb
void SourceParser::createConstant(Constant& constant, LineNumber n) {
    pkb->insertConst(std::stoi(constant));
}

// returns a string representation of the expression pattern in prefix format
ExpressionPattern SourceParser::createExpressionPattern(
    Statement s,
    TokenList& expressionTokenList) {
    for(SourceToken *token : expressionTokenList) {
        TokenType type = token->getType();
        if(type == CONSTANT_TOKEN) {
            createConstant(token->getRawToken(), currentLineNumber);
        }
        else if(type == VARIABLE_TOKEN) {
            Variable var = createVariable(token->getRawToken(), currentLineNumber);
            setUses(s, var);
        }
    }
    return ExpressionUtility::toPostFix(expressionTokenList);
}
