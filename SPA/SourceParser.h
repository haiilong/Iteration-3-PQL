#pragma once

#include <string>
#include <vector>
#include <stack>

#include "SourceToken.h"
#include "IPKB.h"
#include "StringUtility.h"
#include "ExpressionUtility.h"
#include "DesignExtractor.h"
#include "BadTokenException.h"
#include "EmptyStatementListException.h"

class Statement {
public:
    Statement(int n, std::vector<Statement> last) {
        this->n = n;
        this->last = last;
    }
    int n;
    std::vector<Statement> last;
};

typedef std::string SourceCode;

typedef std::string Constant;
typedef std::string VariableName;
typedef Statement UserIndex;
typedef Statement ModifierIndex;

typedef int Variable;
typedef std::list<Statement> StatementList;
typedef int Procedure;
typedef int LineNumber;
typedef std::string ExpressionPattern;
typedef std::list<SourceToken *> TokenList;
typedef std::list<Statement>::iterator StatementListIterator;

class SourceParser {
private: 
    // init
    SourceParser(IPKB& pkb);

    // set relations
    void setRelationForParentStatement(Statement parent, StatementList& children);
    void setFollows(StatementList &stmtList);
    void setParent(Statement parent, StatementList &children);
    void setUses(UserIndex user, Variable used);
    void setUses(UserIndex modifier, std::unordered_set<Variable> used);
    void setModifies(ModifierIndex modifier, Variable modified);
    void setModifies(ModifierIndex modifier, std::unordered_set<Variable> modified);

    void setNext(StatementList & stmtList);

    void setNext(Statement s, LineNumber n);

    void setNext(LineNumber n1, LineNumber n2);

    // methods
    bool buildProgram();

    Procedure buildProcedure();
    Procedure createProcedure(RawToken procedureName);

    Statement createCallStatement(Procedure p);

    StatementList * getStmtList();
    Statement buildCallStatement();
    Statement buildWhileStatement();
    Statement buildIfStatement();
    Statement buildAssignStatement();

    Statement createAssignStatement(SourceToken * LHS, TokenList RHS);
    Statement createIfStatement(Variable control);
    Statement createWhileStatement(Variable control);
    Variable createVariable(VariableName& name, LineNumber n);
    void createConstant(Constant & constant, LineNumber n);
    ExpressionPattern createExpressionPattern(Statement s, TokenList & tokens);

    SourceToken * getToken();
    SourceToken * readCurrentToken();
    SourceToken * matchToken(TokenType type);

    // fields
    static SourceParser* instance;
    IPKB* pkb;
    LineNumber currentLineNumber = 1;
    Procedure currentProcedure = 0;
    SourceTokenIterator tokenIterator;
    SourceTokenIterator tokenEnd;
    SourceToken *next_token;

public:
    // returns a singleton instance of sourceparser
    static SourceParser* getInstance(IPKB& pkb);
    static SourceParser* getInstance();
    static void clear();
    // returns true if parse succeeded, false otherwise. parses the source code and writes information into the PKB
    bool parse(SourceCode source);
};