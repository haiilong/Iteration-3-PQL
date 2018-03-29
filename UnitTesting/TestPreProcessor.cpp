#include "stdafx.h"
#include "CppUnitTest.h"
#include "QueryPreProcessor.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
    TEST_CLASS(TestValidate) {
public:
    QueryPreProcessor qpp;
    QueryObject qo;

    // this test is for system testing debug
    // this is also kinda the integration test for PQL because we test the setter/getter for the storage QueryObject

    TEST_METHOD(testValidateQuery) {
        // test for any query validation here
        Assert::IsTrue(qpp.validate(
            "assign a; stmt s; if i; variable v1, v2; while w; "
            "Select v1.varName "
            "such that Uses(w, v2) and Uses(a, v2) and Next(s, a) and Parent*(i, s) "
            "pattern i(v1,_,_) and a(v2, \"x*y+z\") "
            "such that Modifies(w, v1)"
        ));

        // check the content of the QO below here
        qo = qpp.getQueryObject();
        // check select clause
        Clause select = qo.getSelectClause();
        Assert::IsTrue(select.getRelation() == Clause::Relation::SELECT);
        Assert::IsTrue(select.getFirstArg() == "v1");
        Assert::IsTrue(select.getFirstType() == Clause::Type::VARIABLE);

        // check clauses, first of all, there are 7 clauses
        Assert::IsTrue(qo.getClauses().size() == 7);

        // Check the content of each clause one by one, with -> pattern -> such that

        //first clause is pattern i(v1,_,_)
        Clause first = qo.getClauses().at(0);
        Assert::IsTrue(first.getRelation() == Clause::Relation::IF);
        Assert::IsTrue(first.getFirstType() == Clause::Type::IF);
        Assert::IsTrue(first.getSecondType() == Clause::Type::VARIABLE);
        Assert::IsTrue(first.getFirstArg() == "i");
        Assert::IsTrue(first.getSecondArg() == "v1");

        //second clause is pattern a(v2, \"x*y+z\")
        Clause second = qo.getClauses().at(1);
        Assert::IsTrue(second.getRelation() == Clause::Relation::ASSIGN);
        Assert::IsTrue(second.getFirstType() == Clause::Type::ASSIGN);
        Assert::IsTrue(second.getSecondType() == Clause::Type::VARIABLE);
        Assert::IsTrue(second.getThirdType() == Clause::Type::EXPRESSION);
        Assert::IsTrue(second.getFirstArg() == "a");
        Assert::IsTrue(second.getSecondArg() == "v2");
        Assert::IsTrue(second.getThirdArg() == " x y * z + ");

        //third clause is Uses(w, v2)
        Clause third = qo.getClauses().at(2);
        Assert::IsTrue(third.getRelation() == Clause::Relation::USES);
        Assert::IsTrue(third.getFirstType() == Clause::Type::WHILE);
        Assert::IsTrue(third.getSecondType() == Clause::Type::VARIABLE);
        Assert::IsTrue(third.getFirstArg() == "w");
        Assert::IsTrue(third.getSecondArg() == "v2");

        //forth clause is Uses(a, v2)
        Clause forth = qo.getClauses().at(3);
        Assert::IsTrue(forth.getRelation() == Clause::Relation::USES);
        Assert::IsTrue(forth.getFirstType() == Clause::Type::ASSIGN);
        Assert::IsTrue(forth.getSecondType() == Clause::Type::VARIABLE);
        Assert::IsTrue(forth.getFirstArg() == "a");
        Assert::IsTrue(forth.getSecondArg() == "v2");

        //fifth clause is Next(s, a)
        Clause fifth = qo.getClauses().at(4);
        Assert::IsTrue(fifth.getRelation() == Clause::Relation::NEXT);
        Assert::IsTrue(fifth.getFirstType() == Clause::Type::STMT);
        Assert::IsTrue(fifth.getSecondType() == Clause::Type::ASSIGN);
        Assert::IsTrue(fifth.getFirstArg() == "s");
        Assert::IsTrue(fifth.getSecondArg() == "a");

        //sixth clause is Parent*(i, s)
        Clause sixth = qo.getClauses().at(5);
        Assert::IsTrue(sixth.getRelation() == Clause::Relation::PARENT_T);
        Assert::IsTrue(sixth.getFirstType() == Clause::Type::IF);
        Assert::IsTrue(sixth.getSecondType() == Clause::Type::STMT);
        Assert::IsTrue(sixth.getFirstArg() == "i");
        Assert::IsTrue(sixth.getSecondArg() == "s");

        //seventh clause is Modifies(w, v1)
        Clause seventh = qo.getClauses().at(6);
        Assert::IsTrue(seventh.getRelation() == Clause::Relation::MODIFIES);
        Assert::IsTrue(seventh.getFirstType() == Clause::Type::WHILE);
        Assert::IsTrue(seventh.getSecondType() == Clause::Type::VARIABLE);
        Assert::IsTrue(seventh.getFirstArg() == "w");
        Assert::IsTrue(seventh.getSecondArg() == "v1");
    }

    TEST_METHOD(testBooleanSelectQuery) {
        Assert::IsTrue(qpp.validate("Select BOOLEAN"));
        Assert::IsTrue(qpp.validate("assign a; variable v; Select BOOLEAN such that Uses(a, v) and Modifies(a, v)"));

        qo = qpp.getQueryObject();
        // check select clause
        Clause select = qo.getSelectClause();
        Assert::IsTrue(select.getRelation() == Clause::Relation::SELECT);
        Assert::IsTrue(select.getFirstType() == Clause::Type::BOOLEAN);
    }

    TEST_METHOD(testSimpleSelectQuery) {
        // one declaration
        Assert::IsTrue(qpp.validate("assign a, c#13, d, e, f; Select a"));
        qo = qpp.getQueryObject();
        Clause select = qo.getSelectClause();
        Assert::IsTrue(select.getRelation() == Clause::Relation::SELECT);
        Assert::IsTrue(select.getFirstArg() == "a");
        Assert::IsTrue(select.getFirstType() == Clause::Type::ASSIGN);

        // multiple declarations
        Assert::IsTrue(qpp.validate("assign a; stmt b, x; prog_line c; while d; Select b"));
        qo = qpp.getQueryObject();
        select = qo.getSelectClause();
        Assert::IsTrue(select.getRelation() == Clause::Relation::SELECT);
        Assert::IsTrue(select.getFirstArg() == "b");
        Assert::IsTrue(select.getFirstType() == Clause::Type::STMT);

        // duplicates in declaration
        Assert::IsFalse(qpp.validate("assign a; stmt a; Select b"));

        // wrong naming grammar in declaration
        Assert::IsFalse(qpp.validate("assign 1a; stmt 2d; Select b"));

        // wrong design entity
        Assert::IsFalse(qpp.validate("xd123 a; Select a"));

        // empty select clause
        Assert::IsFalse(qpp.validate("assign a; stmt d;"));

        // wrong format in select clause
        Assert::IsFalse(qpp.validate("stmt t; Selec t"));

        // undeclared synonym in select clause
        Assert::IsFalse(qpp.validate("stmt t; Select a"));
    }

    TEST_METHOD(testAttrRefSelectQuery) {
        // one declaration
        Assert::IsTrue(qpp.validate("assign a, c#13, d, e, f; Select a.stmt#"));
        qo = qpp.getQueryObject();
        Clause select = qo.getSelectClause();
        Assert::IsTrue(select.getRelation() == Clause::Relation::SELECT);
        Assert::IsTrue(select.getFirstArg() == "a");
        Assert::IsTrue(select.getFirstType() == Clause::Type::ASSIGN);

        // one declaration
        Assert::IsTrue(qpp.validate("constant a, c#13, d, e, f; Select d.value"));
        qo = qpp.getQueryObject();
        select = qo.getSelectClause();
        Assert::IsTrue(select.getRelation() == Clause::Relation::SELECT);
        Assert::IsTrue(select.getFirstArg() == "d");
        Assert::IsTrue(select.getFirstType() == Clause::Type::CONSTANT);

        // one declaration
        Assert::IsTrue(qpp.validate("variable a, c#13, d, e, f; Select e.varName"));
        qo = qpp.getQueryObject();
        select = qo.getSelectClause();
        Assert::IsTrue(select.getRelation() == Clause::Relation::SELECT);
        Assert::IsTrue(select.getFirstArg() == "e");
        Assert::IsTrue(select.getFirstType() == Clause::Type::VARIABLE);

        // multiple declarations
        Assert::IsTrue(qpp.validate("assign a; stmt b, x; call c; while d; Select c.procName"));
        qo = qpp.getQueryObject();
        select = qo.getSelectClause();
        Assert::IsTrue(select.getRelation() == Clause::Relation::SELECT);
        Assert::IsTrue(select.getFirstArg() == "c");
        Assert::IsTrue(select.getFirstType() == Clause::Type::CALLPROC);

        // duplicates in declaration
        Assert::IsFalse(qpp.validate("assign a; stmt a; Select b.stmt#"));

        // wrong naming grammar in declaration
        Assert::IsFalse(qpp.validate("assign 1a; stmt 2d; Select b.stmt#"));

        // wrong design entity
        Assert::IsFalse(qpp.validate("xd123 a; Select a.varName"));

        // empty select clause
        Assert::IsFalse(qpp.validate("assign a; stmt d;"));

        // wrong format in select clause
        Assert::IsFalse(qpp.validate("stmt t; Selec t"));

        // undeclared synonym in select clause
        Assert::IsFalse(qpp.validate("stmt t; Select a.value"));

        //wrong attribute matching with synonyms
        Assert::IsFalse(qpp.validate("variable a, c#13, d, e, f; Select e.value"));
        Assert::IsFalse(qpp.validate("procedure a, c#13, d, e, f; Select e.stmt#"));
        Assert::IsFalse(qpp.validate("call a, c#13, d, e, f; Select e.varName"));
        Assert::IsFalse(qpp.validate("assign a, c#13, d, e, f; Select e.procName"));
    }

    TEST_METHOD(testTupleSelectQuery) {
        Assert::IsTrue(qpp.validate("assign a, c#13, d, e, f; Select <a.stmt#, d, e, f.stmt#>"));
        Assert::IsTrue(qpp.validate("assign a, c#13, d, e, f; Select <a,d>"));

        // undeclared synonym
        Assert::IsFalse(qpp.validate("assign a, c#13, d, e, f; Select <a.stmt#, g, e, f.stmt#>"));

        // wrong matching attribute
        Assert::IsFalse(qpp.validate("assign a, c#13, d, e, f; Select <a.stmt#, g, e.value, f.stmt#>"));

        // check the content of the query object
        Assert::IsTrue(qpp.validate("assign a, c#13, d, e, f; call c; Select <a.stmt#, d, e, f.stmt#, c>"));
        qo = qpp.getQueryObject();
        Clause select = qo.getSelectClause();
        vector<Clause::Type> argType = { Clause::Type::ASSIGN, Clause::Type::ASSIGN, Clause::Type::ASSIGN, Clause::Type::ASSIGN, Clause::Type::CALL };
        vector<string> argName = {"a", "d", "e", "f", "c"};
        Assert::IsTrue(select.getTupleArgTypes() == argType);
        Assert::IsTrue(select.getTupleArgNames() == argName);
    }

    TEST_METHOD(testOneFollowsQuery) {
        // all 9 standards true (syn-stmt, _, integer) x(syn-stmt, _, integer)
        Assert::IsTrue(qpp.validate("stmt s; assign a; Select s such that Follows(s,a)"));
        Assert::IsTrue(qpp.validate("while s; prog_line  a; Select s such that Follows(s,_)"));
        Assert::IsTrue(qpp.validate("stmt s; assign a; Select s such that Follows(s,1)"));
        Assert::IsTrue(qpp.validate("while s; prog_line  a; Select s such that Follows(1,a)"));
        Assert::IsTrue(qpp.validate("stmt s; assign a; Select s such that Follows(1,a)"));
        Assert::IsTrue(qpp.validate("while s; prog_line  a; Select s such that Follows(1,a)"));
        Assert::IsTrue(qpp.validate("stmt s; assign a; Select s such that Follows(_,a)"));
        Assert::IsTrue(qpp.validate("while s; prog_line  a; Select s such that Follows(_,a)"));
        Assert::IsTrue(qpp.validate("stmt s; assign a; Select s such that Follows(_,a)"));

        // test content of query object
        Assert::IsTrue(qpp.validate("stmt s; assign a; Select s such that Follows(s,a)"));
        qo = qpp.getQueryObject();
        Clause select = qo.getSelectClause();
        Assert::IsTrue(select.getRelation() == Clause::Relation::SELECT);
        Assert::IsTrue(select.getFirstArg() == "s");
        Assert::IsTrue(select.getFirstType() == Clause::Type::STMT);
        Clause follows = qo.getClauses().at(0);
        Assert::IsTrue(follows.getRelation() == Clause::Relation::FOLLOWS);
        Assert::IsTrue(follows.getFirstType() == Clause::Type::STMT);
        Assert::IsTrue(follows.getSecondType() == Clause::Type::ASSIGN);
        Assert::IsTrue(follows.getFirstArg() == "s");
        Assert::IsTrue(follows.getSecondArg() == "a");

        // test content of query object
        Assert::IsTrue(qpp.validate("stmt s; assign a; Select s such that Follows(_,10)"));
        qo = qpp.getQueryObject();
        select = qo.getSelectClause();
        Assert::IsTrue(select.getRelation() == Clause::Relation::SELECT);
        Assert::IsTrue(select.getFirstArg() == "s");
        Assert::IsTrue(select.getFirstType() == Clause::Type::STMT);
        follows = qo.getClauses().at(0);
        Assert::IsTrue(follows.getRelation() == Clause::Relation::FOLLOWS);
        Assert::IsTrue(follows.getFirstType() == Clause::Type::WILDCARD);
        Assert::IsTrue(follows.getSecondType() == Clause::Type::INTEGER);
        Assert::IsTrue(follows.getFirstArg() == "_");
        Assert::IsTrue(follows.getSecondArg() == "10");

        // wrong first argument due to undeclared synonym
        Assert::IsFalse(qpp.validate("stmt s; assign a; Select s such that Follows(t,a)"));

        // wrong first argument due to wrong type of synonym
        Assert::IsFalse(qpp.validate("variable v; assign a; Select s such that Follows(v,a)"));

        // wrong first argument due to wrong expression format
        Assert::IsFalse(qpp.validate("variable v; assign a; Select s such that Follows(\"asdas,a)"));

        // wrong second argument due to undeclared synonym
        Assert::IsFalse(qpp.validate("stmt s; assign a; Select s such that Follows(a,t)"));

        // wrong second argument due to wrong type of synonym
        Assert::IsFalse(qpp.validate("variable v; assign a; Select s such that Follows(a,v)"));

        // wrong second argument due to wrong expression format
        Assert::IsFalse(qpp.validate("variable v; assign a; Select s such that Follows(a,\"asdas)"));
    }

    TEST_METHOD(testOneParentQuery) {
        // all 9 standards true (syn-stmt/not-assign, _, integer) x(syn-stmt, _, integer)
        Assert::IsTrue(qpp.validate("stmt s; assign a; Select s such that Parent(s,a)"));
        Assert::IsTrue(qpp.validate("while s; prog_line  a; Select s such that Parent(s,_)"));
        Assert::IsTrue(qpp.validate("stmt s; assign a; Select s such that Parent(s,5)"));
        Assert::IsTrue(qpp.validate("while s; prog_line  a; Select s such that Parent(1,a)"));
        Assert::IsTrue(qpp.validate("stmt s; assign a; Select s such that Parent(1,a)"));
        Assert::IsTrue(qpp.validate("while s; prog_line  a; Select s such that Parent(1,a)"));
        Assert::IsTrue(qpp.validate("stmt s; assign a; Select s such that Parent(_,a)"));
        Assert::IsTrue(qpp.validate("while s; prog_line  a; Select s such that Parent(_,a)"));
        Assert::IsTrue(qpp.validate("stmt s; assign a; Select s such that Parent(_,a)"));

        // test content of query object
        Assert::IsTrue(qpp.validate("stmt s; assign a; Select s such that Parent(s,a)"));
        qo = qpp.getQueryObject();
        Clause select = qo.getSelectClause();
        Assert::IsTrue(select.getRelation() == Clause::Relation::SELECT);
        Assert::IsTrue(select.getFirstArg() == "s");
        Assert::IsTrue(select.getFirstType() == Clause::Type::STMT);
        Clause parent = qo.getClauses().at(0);
        Assert::IsTrue(parent.getRelation() == Clause::Relation::PARENT);
        Assert::IsTrue(parent.getFirstType() == Clause::Type::STMT);
        Assert::IsTrue(parent.getSecondType() == Clause::Type::ASSIGN);
        Assert::IsTrue(parent.getFirstArg() == "s");
        Assert::IsTrue(parent.getSecondArg() == "a");

        // test content of query object
        Assert::IsTrue(qpp.validate("stmt s; assign a; Select s such that Parent(_,10)"));
        qo = qpp.getQueryObject();
        select = qo.getSelectClause();
        Assert::IsTrue(select.getRelation() == Clause::Relation::SELECT);
        Assert::IsTrue(select.getFirstArg() == "s");
        Assert::IsTrue(select.getFirstType() == Clause::Type::STMT);
        parent = qo.getClauses().at(0);
        Assert::IsTrue(parent.getRelation() == Clause::Relation::PARENT);
        Assert::IsTrue(parent.getFirstType() == Clause::Type::WILDCARD);
        Assert::IsTrue(parent.getSecondType() == Clause::Type::INTEGER);
        Assert::IsTrue(parent.getFirstArg() == "_");
        Assert::IsTrue(parent.getSecondArg() == "10");

        // wrong first argument due to undeclared synonym
        Assert::IsFalse(qpp.validate("stmt s; assign a; Select s such that Parent(t,a)"));

        // wrong first argument due to wrong type of synonym
        Assert::IsFalse(qpp.validate("variable v; assign a; Select s such that Parent(v,a)"));

        // wrong first argument cannot be assign
        Assert::IsFalse(qpp.validate("variable v; assign a; Select s such that Parent(a,_)"));

        // wrong first argument due to wrong expression format
        Assert::IsFalse(qpp.validate("variable v; assign a; Select s such that Parent(\"asdas,a)"));

        // wrong second argument due to undeclared synonym
        Assert::IsFalse(qpp.validate("stmt s; assign a; Select s such that Parent(a,t)"));

        // wrong second argument due to wrong type of synonym
        Assert::IsFalse(qpp.validate("variable v; assign a; Select s such that Parent(a,v)"));

        // wrong second argument due to wrong expression format
        Assert::IsFalse(qpp.validate("variable v; assign a; Select s such that Parent(a,\"asdas)"));

    }

    TEST_METHOD(testOneModifiesQuery) {
        // all 9 standards true (procedure, syn-stmt,integer) x (syn-variable,_,EXPRESSION)
        Assert::IsTrue(qpp.validate("stmt s; variable a; Select s such that Modifies(s,a)"));
        Assert::IsTrue(qpp.validate("while s; variable a; Select s such that Modifies(s,_)"));
        Assert::IsTrue(qpp.validate("stmt s; variable a; Select s such that Modifies(s,\"abc\")"));
        Assert::IsTrue(qpp.validate("while s; variable a; Select s such that Modifies(1,a)"));
        Assert::IsTrue(qpp.validate("stmt s; variable a; Select s such that Modifies(1,_)"));
        Assert::IsTrue(qpp.validate("while s; variable a; Select s such that Modifies(1,\"abc\")"));
        Assert::IsTrue(qpp.validate("procedure p; variable a; Select p such that Modifies(p,a)"));
        Assert::IsTrue(qpp.validate("procedure p; variable a; Select p such that Modifies(p,_)"));
        Assert::IsTrue(qpp.validate("procedure p; variable a; Select p such that Modifies(p,\"abc\")"));

        // test content of query object
        Assert::IsTrue(qpp.validate("stmt s; variable a; Select s such that Modifies(s,a)"));
        qo = qpp.getQueryObject();
        Clause select = qo.getSelectClause();
        Assert::IsTrue(select.getRelation() == Clause::Relation::SELECT);
        Assert::IsTrue(select.getFirstArg() == "s");
        Assert::IsTrue(select.getFirstType() == Clause::Type::STMT);
        Clause modifies = qo.getClauses().at(0);
        Assert::IsTrue(modifies.getRelation() == Clause::Relation::MODIFIES);
        Assert::IsTrue(modifies.getFirstType() == Clause::Type::STMT);
        Assert::IsTrue(modifies.getSecondType() == Clause::Type::VARIABLE);
        Assert::IsTrue(modifies.getFirstArg() == "s");
        Assert::IsTrue(modifies.getSecondArg() == "a");

        // test content of query object
        Assert::IsTrue(qpp.validate("while s; prog_line  a; Select s such that Modifies(1,\"abc\")"));
        qo = qpp.getQueryObject();
        select = qo.getSelectClause();
        Assert::IsTrue(select.getRelation() == Clause::Relation::SELECT);
        Assert::IsTrue(select.getFirstArg() == "s");
        Assert::IsTrue(select.getFirstType() == Clause::Type::WHILE);
        modifies = qo.getClauses().at(0);
        Assert::IsTrue(modifies.getRelation() == Clause::Relation::MODIFIES);
        Assert::IsTrue(modifies.getFirstType() == Clause::Type::INTEGER);
        Assert::IsTrue(modifies.getSecondType() == Clause::Type::IDENT);
        Assert::IsTrue(modifies.getFirstArg() == "1");
        Assert::IsTrue(modifies.getSecondArg() == "abc");

        // wrong first argument due to undeclared synonym
        Assert::IsFalse(qpp.validate("stmt s; variable a; Select s such that Modifies(x,a)"));

        // wrong first argument due to wrong type of synonym
        Assert::IsFalse(qpp.validate("stmt s; variable a; Select s such that Modifies(_,a)"));

        // wrong first argument due to wrong expression format
        Assert::IsFalse(qpp.validate("stmt s; variable a; Select s such that Modifies(\"sdf,a)"));

        // wrong second argument due to undeclared synonym
        Assert::IsFalse(qpp.validate("stmt s; variable a; Select s such that Modifies(s,n)"));

        // wrong second argument due to wrong type of synonym
        Assert::IsFalse(qpp.validate("stmt s; while a; Select s such that Modifies(s,a)"));

        // wrong second argument due to wrong expression format
        Assert::IsFalse(qpp.validate("prog_line s; variable a; Select s such that Modifies(s, \"sdf)"));
    }

    TEST_METHOD(testOneUsesQuery) {
        // all 9 standards true (procedure, syn-stmt,integer) x (syn-variable,_,EXPRESSION)
        Assert::IsTrue(qpp.validate("stmt s; variable a; Select s such that Modifies(s,a)"));
        Assert::IsTrue(qpp.validate("while s; variable a; Select s such that Uses(s,_)"));
        Assert::IsTrue(qpp.validate("stmt s; variable a; Select s such that Uses(s,\"abc\")"));
        Assert::IsTrue(qpp.validate("while s; variable a; Select s such that Uses(1,a)"));
        Assert::IsTrue(qpp.validate("stmt s; variable a; Select s such that Uses(1,_)"));
        Assert::IsTrue(qpp.validate("while s; variable a; Select s such that Uses(1,\"abc\")"));
        Assert::IsTrue(qpp.validate("procedure p; variable a; Select p such that Uses(p,a)"));
        Assert::IsTrue(qpp.validate("procedure p; variable a; Select p such that Uses(p,_)"));
        Assert::IsTrue(qpp.validate("procedure p; variable a; Select p such that Uses(p,\"abc\")"));

        // test content of query object
        Assert::IsTrue(qpp.validate("stmt s; variable a; Select s such that Uses(s,a)"));
        qo = qpp.getQueryObject();
        Clause select = qo.getSelectClause();
        Assert::IsTrue(select.getRelation() == Clause::Relation::SELECT);
        Assert::IsTrue(select.getFirstArg() == "s");
        Assert::IsTrue(select.getFirstType() == Clause::Type::STMT);
        Clause uses = qo.getClauses().at(0);
        Assert::IsTrue(uses.getRelation() == Clause::Relation::USES);
        Assert::IsTrue(uses.getFirstType() == Clause::Type::STMT);
        Assert::IsTrue(uses.getSecondType() == Clause::Type::VARIABLE);
        Assert::IsTrue(uses.getFirstArg() == "s");
        Assert::IsTrue(uses.getSecondArg() == "a");

        // test content of query object
        Assert::IsTrue(qpp.validate("while s; prog_line  a; Select s such that Uses(1,\"abc\")"));
        qo = qpp.getQueryObject();
        select = qo.getSelectClause();
        Assert::IsTrue(select.getRelation() == Clause::Relation::SELECT);
        Assert::IsTrue(select.getFirstArg() == "s");
        Assert::IsTrue(select.getFirstType() == Clause::Type::WHILE);
        uses = qo.getClauses().at(0);
        Assert::IsTrue(uses.getRelation() == Clause::Relation::USES);
        Assert::IsTrue(uses.getFirstType() == Clause::Type::INTEGER);
        Assert::IsTrue(uses.getSecondType() == Clause::Type::IDENT);
        Assert::IsTrue(uses.getFirstArg() == "1");
        Assert::IsTrue(uses.getSecondArg() == "abc");

        // wrong first argument due to undeclared synonym
        Assert::IsFalse(qpp.validate("stmt s; variable a; Select s such that Uses(x,a)"));

        // wrong first argument due to wrong type of synonym
        Assert::IsFalse(qpp.validate("stmt s; variable a; Select s such that Uses(_,a)"));

        // wrong first argument due to wrong expression format
        Assert::IsFalse(qpp.validate("stmt s; variable a; Select s such that Uses(\"sdf,a)"));

        // wrong second argument due to undeclared synonym
        Assert::IsFalse(qpp.validate("stmt s; variable a; Select s such that Uses(s,n)"));

        // wrong second argument due to wrong type of synonym
        Assert::IsFalse(qpp.validate("stmt s; while a; Select s such that Uses(s,a)"));

        // wrong second argument due to wrong expression format
        Assert::IsFalse(qpp.validate("prog_line s; variable a; Select s such that Uses(s, \"sdf)"));
    }

    TEST_METHOD(testOneCallQuery) {
        // all 9 standards true (procedure, wildcard,ident) x (procedure, wildcard,ident)
        Assert::IsTrue(qpp.validate("procedure p; procedure q; Select p such that Calls(p,q)"));
        Assert::IsTrue(qpp.validate("procedure p; procedure q; Select p such that Calls(p,_)"));
        Assert::IsTrue(qpp.validate("procedure p; procedure q; Select p such that Calls(p,\"proc\")"));
        Assert::IsTrue(qpp.validate("procedure p; procedure q; Select p such that Calls(_,q)"));
        Assert::IsTrue(qpp.validate("procedure p; procedure q; Select p such that Calls(_,_)"));
        Assert::IsTrue(qpp.validate("procedure p; procedure q; Select p such that Calls(_,\"proc\")"));
        Assert::IsTrue(qpp.validate("procedure p; procedure q; Select p such that Calls(\"proc\",q)"));
        Assert::IsTrue(qpp.validate("procedure p; procedure q; Select p such that Calls(\"proc\",q)"));
        Assert::IsTrue(qpp.validate("procedure p; procedure q; Select p such that Calls(\"proc\",\"proc\")"));

        // test content of query object
        Assert::IsTrue(qpp.validate("procedure p; procedure q; Select p such that Calls(p,\"proc\")"));
        qo = qpp.getQueryObject();
        Clause select = qo.getSelectClause();
        Assert::IsTrue(select.getRelation() == Clause::Relation::SELECT);
        Assert::IsTrue(select.getFirstArg() == "p");
        Assert::IsTrue(select.getFirstType() == Clause::Type::PROCEDURE);
        Clause calls = qo.getClauses().at(0);
        Assert::IsTrue(calls.getRelation() == Clause::Relation::CALLS);
        Assert::IsTrue(calls.getFirstType() == Clause::Type::PROCEDURE);
        Assert::IsTrue(calls.getSecondType() == Clause::Type::IDENT);
        Assert::IsTrue(calls.getFirstArg() == "p");
        Assert::IsTrue(calls.getSecondArg() == "proc");

        // test content of query object
        Assert::IsTrue(qpp.validate("procedure p; procedure q; Select p such that Calls(_,q)"));
        qo = qpp.getQueryObject();
        select = qo.getSelectClause();
        Assert::IsTrue(select.getRelation() == Clause::Relation::SELECT);
        Assert::IsTrue(select.getFirstArg() == "p");
        Assert::IsTrue(select.getFirstType() == Clause::Type::PROCEDURE);
        calls = qo.getClauses().at(0);
        Assert::IsTrue(calls.getRelation() == Clause::Relation::CALLS);
        Assert::IsTrue(calls.getFirstType() == Clause::Type::WILDCARD);
        Assert::IsTrue(calls.getSecondType() == Clause::Type::PROCEDURE);
        Assert::IsTrue(calls.getFirstArg() == "_");
        Assert::IsTrue(calls.getSecondArg() == "q");

        // wrong first argument due to undeclared synonym
        Assert::IsFalse(qpp.validate("procedure p; procedure q; Select p such that Calls(p1,q)"));

        // wrong first argument due to wrong type of synonym
        Assert::IsFalse(qpp.validate("procedure p; variable q; Select p such that Calls(q,p)"));

        // wrong second argument due to undeclared synonym
        Assert::IsFalse(qpp.validate("procedure p; procedure q; Select p such that Calls(p,q1)"));

        // wrong second argument due to wrong type of synonym
        Assert::IsFalse(qpp.validate("procedure p; variable q; Select p such that Calls(p,q)"));
    }

    TEST_METHOD(testOneNextQuery) {
        // all 9 standards true (syn-stmt, _, integer) x(syn-stmt, _, integer)
        Assert::IsTrue(qpp.validate("stmt s; assign a; Select s such that Next(s,a)"));
        Assert::IsTrue(qpp.validate("while s; prog_line  a; Select s such that Next(s,_)"));
        Assert::IsTrue(qpp.validate("stmt s; assign a; Select s such that Next(s,1)"));
        Assert::IsTrue(qpp.validate("while s; prog_line  a; Select s such that Next(1,a)"));
        Assert::IsTrue(qpp.validate("stmt s; assign a; Select s such that Next(1,a)"));
        Assert::IsTrue(qpp.validate("while s; prog_line  a; Select s such that Next(1,a)"));
        Assert::IsTrue(qpp.validate("stmt s; assign a; Select s such that Next(_,a)"));
        Assert::IsTrue(qpp.validate("while s; prog_line  a; Select s such that Next(_,a)"));
        Assert::IsTrue(qpp.validate("stmt s; assign a; Select s such that Next(_,a)"));

        // test content of query object
        Assert::IsTrue(qpp.validate("stmt s; assign a; Select s such that Next(s,a)"));
        qo = qpp.getQueryObject();
        Clause select = qo.getSelectClause();
        Assert::IsTrue(select.getRelation() == Clause::Relation::SELECT);
        Assert::IsTrue(select.getFirstArg() == "s");
        Assert::IsTrue(select.getFirstType() == Clause::Type::STMT);
        Clause next = qo.getClauses().at(0);
        Assert::IsTrue(next.getRelation() == Clause::Relation::NEXT);
        Assert::IsTrue(next.getFirstType() == Clause::Type::STMT);
        Assert::IsTrue(next.getSecondType() == Clause::Type::ASSIGN);
        Assert::IsTrue(next.getFirstArg() == "s");
        Assert::IsTrue(next.getSecondArg() == "a");

        // test content of query object
        Assert::IsTrue(qpp.validate("stmt s; assign a; Select s such that Next(_,10)"));
        qo = qpp.getQueryObject();
        select = qo.getSelectClause();
        Assert::IsTrue(select.getRelation() == Clause::Relation::SELECT);
        Assert::IsTrue(select.getFirstArg() == "s");
        Assert::IsTrue(select.getFirstType() == Clause::Type::STMT);
        next = qo.getClauses().at(0);
        Assert::IsTrue(next.getRelation() == Clause::Relation::NEXT);
        Assert::IsTrue(next.getFirstType() == Clause::Type::WILDCARD);
        Assert::IsTrue(next.getSecondType() == Clause::Type::INTEGER);
        Assert::IsTrue(next.getFirstArg() == "_");
        Assert::IsTrue(next.getSecondArg() == "10");

        // wrong first argument due to undeclared synonym
        Assert::IsFalse(qpp.validate("stmt s; assign a; Select s such that Next(t,a)"));

        // wrong first argument due to wrong type of synonym
        Assert::IsFalse(qpp.validate("variable v; assign a; Select s such that Next(v,a)"));

        // wrong first argument due to wrong expression format
        Assert::IsFalse(qpp.validate("variable v; assign a; Select s such that Next(\"asdas,a)"));

        // wrong second argument due to undeclared synonym
        Assert::IsFalse(qpp.validate("stmt s; assign a; Select s such that Next(a,t)"));

        // wrong second argument due to wrong type of synonym
        Assert::IsFalse(qpp.validate("variable v; assign a; Select s such that Next(a,v)"));

        // wrong second argument due to wrong expression format
        Assert::IsFalse(qpp.validate("variable v; assign a; Select s such that Next(a,\"asdas)"));
    }

    TEST_METHOD(testOnePatternAssignClause) {
        // all 9 standards true (syn-variable,_,ident) x (_,EXPRESSION, SUBEXPRESSION)
        Assert::IsTrue(qpp.validate("assign a; variable v; Select a pattern a(v,_)"));
        Assert::IsTrue(qpp.validate("assign a; variable v; Select a pattern a(v,_\"a+b+c\"_)"));
        Assert::IsTrue(qpp.validate("assign a; variable v; Select a pattern a(_,_)"));
        Assert::IsTrue(qpp.validate("assign a; variable v; Select a pattern a(_,_\"a+b*c\"_)"));
        Assert::IsTrue(qpp.validate("assign a; variable v; Select a pattern a(\"xyz\",_)"));
        Assert::IsTrue(qpp.validate("assign a; variable v; Select a pattern a(\"xyz\",_\"x+y*z\"_)"));
        Assert::IsTrue(qpp.validate("assign a; variable v; Select a pattern a(v,\"a+b+c\")"));
        Assert::IsTrue(qpp.validate("assign a; variable v; Select a pattern a(_,\"a+b*c\")"));
        Assert::IsTrue(qpp.validate("assign a; variable v; Select a pattern a(\"xyz\",\"x+y*z\")"));

        // test content of query object
        Assert::IsTrue(qpp.validate("assign a; variable v; Select a pattern a(\"xyz\",\"x+y*z\")"));
        qo = qpp.getQueryObject();
        Clause select = qo.getSelectClause();
        Assert::IsTrue(select.getRelation() == Clause::Relation::SELECT);
        Assert::IsTrue(select.getFirstArg() == "a");
        Assert::IsTrue(select.getFirstType() == Clause::Type::ASSIGN);
        Clause assign = qo.getClauses().at(0);
        Assert::IsTrue(assign.getRelation() == Clause::Relation::ASSIGN);
        Assert::IsTrue(assign.getFirstType() == Clause::Type::ASSIGN);
        Assert::IsTrue(assign.getSecondType() == Clause::Type::IDENT);
        Assert::IsTrue(assign.getThirdType() == Clause::Type::EXPRESSION);
        Assert::IsTrue(assign.getFirstArg() == "a");
        Assert::IsTrue(assign.getSecondArg() == "xyz");
        Assert::IsTrue(assign.getThirdArg() == " x y z * + ");

        // test content of query object
        Assert::IsTrue(qpp.validate("assign a; variable v; Select a pattern a(_,_\"(a+b)*(c+d)\"_)"));
        qo = qpp.getQueryObject();
        select = qo.getSelectClause();
        Assert::IsTrue(select.getRelation() == Clause::Relation::SELECT);
        Assert::IsTrue(select.getFirstArg() == "a");
        Assert::IsTrue(select.getFirstType() == Clause::Type::ASSIGN);
        assign = qo.getClauses().at(0);
        Assert::IsTrue(assign.getRelation() == Clause::Relation::ASSIGN);
        Assert::IsTrue(assign.getFirstType() == Clause::Type::ASSIGN);
        Assert::IsTrue(assign.getSecondType() == Clause::Type::WILDCARD);
        Assert::IsTrue(assign.getThirdType() == Clause::Type::SUBEXPRESSION);
        Assert::IsTrue(assign.getFirstArg() == "a");
        Assert::IsTrue(assign.getSecondArg() == "_");
        Assert::IsTrue(assign.getThirdArg() == " a b + c d + * ");

        // wrong relation
        Assert::IsFalse(qpp.validate("assign a; variable v; Select a pattern b(v,_)"));
        Assert::IsFalse(qpp.validate("stmt a; variable v; Select a pattern a(v,_)"));

        // wrong first argument
        Assert::IsFalse(qpp.validate("assign a; prog_line v; Select a pattern a(v,_)"));
        Assert::IsFalse(qpp.validate("assign a; variable v; Select a pattern a(t,_)"));
        Assert::IsFalse(qpp.validate("assign a; prog_line v; Select a pattern a(\"x,_)"));

        // wrong second argument
        Assert::IsFalse(qpp.validate("assign a; prog_line v; Select a pattern a(_,asdfg)"));
        Assert::IsFalse(qpp.validate("assign a; prog_line v; Select a pattern a(_,v)"));
        Assert::IsFalse(qpp.validate("assign a; prog_line v; Select a pattern a(_,\"1+1a\")"));
        Assert::IsFalse(qpp.validate("assign a; prog_line v; Select a pattern a(_,_\"((a+b*c)\"_)"));
    }

    TEST_METHOD(testOnePatternWhileClause) {
        // all 3 standards true (syn-variable,_,ident) x (_)
        Assert::IsTrue(qpp.validate("while w; variable v; Select w pattern w(v,_)"));
        Assert::IsTrue(qpp.validate("while w; variable v; Select w pattern w(_,_)"));
        Assert::IsTrue(qpp.validate("while w; variable v; Select w pattern w(\"xyz\",_)"));

        // test content of query object
        Assert::IsTrue(qpp.validate("while w; variable v; Select w pattern w(\"xyz\",_)"));
        qo = qpp.getQueryObject();
        Clause select = qo.getSelectClause();
        Assert::IsTrue(select.getRelation() == Clause::Relation::SELECT);
        Assert::IsTrue(select.getFirstArg() == "w");
        Assert::IsTrue(select.getFirstType() == Clause::Type::WHILE);
        Clause assign = qo.getClauses().at(0);
        Assert::IsTrue(assign.getRelation() == Clause::Relation::WHILE);
        Assert::IsTrue(assign.getFirstType() == Clause::Type::WHILE);
        Assert::IsTrue(assign.getSecondType() == Clause::Type::IDENT);
        Assert::IsTrue(assign.getThirdType() == Clause::Type::WILDCARD);
        Assert::IsTrue(assign.getFirstArg() == "w");
        Assert::IsTrue(assign.getSecondArg() == "xyz");
        Assert::IsTrue(assign.getThirdArg() == "_");

        // test content of query object
        Assert::IsTrue(qpp.validate("while w; variable v; Select w pattern w(v,_)"));
        qo = qpp.getQueryObject();
        select = qo.getSelectClause();
        Assert::IsTrue(select.getRelation() == Clause::Relation::SELECT);
        Assert::IsTrue(select.getFirstArg() == "w");
        Assert::IsTrue(select.getFirstType() == Clause::Type::WHILE);
        assign = qo.getClauses().at(0);
        Assert::IsTrue(assign.getRelation() == Clause::Relation::WHILE);
        Assert::IsTrue(assign.getFirstType() == Clause::Type::WHILE);
        Assert::IsTrue(assign.getSecondType() == Clause::Type::VARIABLE);
        Assert::IsTrue(assign.getThirdType() == Clause::Type::WILDCARD);
        Assert::IsTrue(assign.getFirstArg() == "w");
        Assert::IsTrue(assign.getSecondArg() == "v");
        Assert::IsTrue(assign.getThirdArg() == "_");

        // wrong relation
        Assert::IsFalse(qpp.validate("while w; variable v; Select w pattern b(v,_)"));
        Assert::IsFalse(qpp.validate("stmt w; variable v; Select w pattern w(v,_)"));

        // wrong first argument
        Assert::IsFalse(qpp.validate("while w; variable v; Select w pattern w(sd,_)"));
        Assert::IsFalse(qpp.validate("while w; stmt v; Select w pattern w(v,_)"));
        Assert::IsFalse(qpp.validate("while w; stmt v; Select w pattern w(\"1\",_)"));

        // wrong second argument
        Assert::IsFalse(qpp.validate("while w; variable v; Select w pattern w(v,sdfsd2)"));
        Assert::IsFalse(qpp.validate("while w; variable v; Select w pattern w(v,\"123\")"));
    }

    TEST_METHOD(testOnePatternIfClause) {
        // all 3 standards true (syn-variable,_,ident) x (_)
        Assert::IsTrue(qpp.validate("if i; variable v; Select i pattern i(v,_,_)"));
        Assert::IsTrue(qpp.validate("if i; variable v; Select i pattern i(_,_,_)"));
        Assert::IsTrue(qpp.validate("if i; variable v; Select i pattern i(\"xyz\",_,_)"));

        // test content of query object
        Assert::IsTrue(qpp.validate("if i; variable v; Select i pattern i(\"xyz\",_,_)"));
        qo = qpp.getQueryObject();
        Clause select = qo.getSelectClause();
        Assert::IsTrue(select.getRelation() == Clause::Relation::SELECT);
        Assert::IsTrue(select.getFirstArg() == "i");
        Assert::IsTrue(select.getFirstType() == Clause::Type::IF);
        Clause assign = qo.getClauses().at(0);
        Assert::IsTrue(assign.getRelation() == Clause::Relation::IF);
        Assert::IsTrue(assign.getFirstType() == Clause::Type::IF);
        Assert::IsTrue(assign.getSecondType() == Clause::Type::IDENT);
        Assert::IsTrue(assign.getThirdType() == Clause::Type::WILDCARD);
        Assert::IsTrue(assign.getFirstArg() == "i");
        Assert::IsTrue(assign.getSecondArg() == "xyz");
        Assert::IsTrue(assign.getThirdArg() == "_");

        // test content of query object
        Assert::IsTrue(qpp.validate("if i; variable v; Select i pattern i(v,_,_)"));
        qo = qpp.getQueryObject();
        select = qo.getSelectClause();
        Assert::IsTrue(select.getRelation() == Clause::Relation::SELECT);
        Assert::IsTrue(select.getFirstArg() == "i");
        Assert::IsTrue(select.getFirstType() == Clause::Type::IF);
        assign = qo.getClauses().at(0);
        Assert::IsTrue(assign.getRelation() == Clause::Relation::IF);
        Assert::IsTrue(assign.getFirstType() == Clause::Type::IF);
        Assert::IsTrue(assign.getSecondType() == Clause::Type::VARIABLE);
        Assert::IsTrue(assign.getThirdType() == Clause::Type::WILDCARD);
        Assert::IsTrue(assign.getFirstArg() == "i");
        Assert::IsTrue(assign.getSecondArg() == "v");
        Assert::IsTrue(assign.getThirdArg() == "_");

        // wrong relation
        Assert::IsFalse(qpp.validate("if i; variable v; Select i pattern b(v,_,_)"));
        Assert::IsFalse(qpp.validate("stmt i; variable v; Select i pattern i(v,_,_)"));

        // wrong first argument
        Assert::IsFalse(qpp.validate("if i; variable v; Select i pattern i(sd,_,_)"));
        Assert::IsFalse(qpp.validate("if i; stmt v; Select i pattern i(v,_,_)"));
        Assert::IsFalse(qpp.validate("if i; stmt v; Select i pattern i(\"1\",_,_)"));

        // wrong second argument
        Assert::IsFalse(qpp.validate("if i; variable v; Select i pattern i(v,sdfsd2)"));
        Assert::IsFalse(qpp.validate("if i; variable v; Select i pattern i(v,\"123\")"));
        Assert::IsFalse(qpp.validate("if i; variable v; Select i pattern i(v,_)"));
        Assert::IsFalse(qpp.validate("if i; variable v; Select i pattern i(v,_, _ ,_)"));
    }

    TEST_METHOD(testOneWithClause) {
        // both sides are integers
        Assert::IsTrue(qpp.validate("if i; Select i with i.stmt# = 3"));
        Assert::IsTrue(qpp.validate("if i; Select i with 3 = i.stmt#"));
        Assert::IsTrue(qpp.validate("if i; stmt s; Select i with i.stmt# = s.stmt#"));
        Assert::IsTrue(qpp.validate("if i; stmt s; Select i with s.stmt# = i.stmt#"));
        Assert::IsTrue(qpp.validate("stmt s; assign a; Select s with s.stmt# = a.stmt#"));
        Assert::IsTrue(qpp.validate("stmt s; assign a; Select s with a.stmt# = s.stmt#"));
        Assert::IsTrue(qpp.validate("stmt s; while w; Select s with s.stmt# = w.stmt#"));
        Assert::IsTrue(qpp.validate("stmt s; while w; Select s with w.stmt# = s.stmt#"));
        Assert::IsTrue(qpp.validate("stmt s; call c; Select s with s.stmt# = c.stmt#"));
        Assert::IsTrue(qpp.validate("stmt s; call c; Select s with c.stmt# = s.stmt#"));
        Assert::IsTrue(qpp.validate("stmt s; prog_line n; Select s with s.stmt# = n"));
        Assert::IsTrue(qpp.validate("stmt s; prog_line n; Select s with n = s.stmt#"));
        Assert::IsTrue(qpp.validate("stmt s; constant c; Select s with s.stmt# = c.value"));
        Assert::IsTrue(qpp.validate("stmt s; constant c; Select s with c.value = s.stmt#"));

        // both sides are strings
        Assert::IsTrue(qpp.validate("procedure p; call c; Select p with p.procName = c.procName"));
        Assert::IsTrue(qpp.validate("procedure p; call c; Select p with c.procName = p.procName"));
        Assert::IsTrue(qpp.validate("variable v; call c; Select c with c.procName = v.varName"));
        Assert::IsTrue(qpp.validate("variable v; call c; Select c with v.varName = c.procName"));
        Assert::IsTrue(qpp.validate("variable v; Select v with v.varName = \"asdz\""));
        Assert::IsTrue(qpp.validate("variable v; Select v with \"asdz\" = v.varName"));

        // int vs string
        Assert::IsFalse(qpp.validate("procedure p; Select p with p.procName = 5"));

        // test content of the object
        Assert::IsTrue(qpp.validate("procedure p; call c; Select p with p.procName = c.procName"));
        qo = qpp.getQueryObject();
        Clause with = qo.getClauses().at(0);
        Assert::IsTrue(with.getRelation() == Clause::Relation::WITH);
        Assert::IsTrue(with.getFirstType() == Clause::Type::PROCEDURE);
        Assert::IsTrue(with.getSecondType() == Clause::Type::CALLPROC);
        Assert::IsTrue(with.getFirstArg() == "p");
        Assert::IsTrue(with.getSecondArg() == "c");

        // test content of the object
        Assert::IsTrue(qpp.validate("stmt s; prog_line n; Select s with s.stmt# = n"));
        qo = qpp.getQueryObject();
        with = qo.getClauses().at(0);
        Assert::IsTrue(with.getRelation() == Clause::Relation::WITH);
        Assert::IsTrue(with.getFirstType() == Clause::Type::STMT);
        Assert::IsTrue(with.getSecondType() == Clause::Type::PROG_LINE);
        Assert::IsTrue(with.getFirstArg() == "s");
        Assert::IsTrue(with.getSecondArg() == "n");

        // test content of the object
        Assert::IsTrue(qpp.validate("stmt s; constant c; Select s with c.value = 10"));
        qo = qpp.getQueryObject();
        with = qo.getClauses().at(0);
        Assert::IsTrue(with.getRelation() == Clause::Relation::WITH);
        Assert::IsTrue(with.getFirstType() == Clause::Type::CONSTANT);
        Assert::IsTrue(with.getSecondType() == Clause::Type::INTEGER);
        Assert::IsTrue(with.getFirstArg() == "c");
        Assert::IsTrue(with.getSecondArg() == "10");

        // test content of the object
        Assert::IsTrue(qpp.validate("stmt s; constant c; Select s with 10 = c.value"));
        qo = qpp.getQueryObject();
        with = qo.getClauses().at(0);
        Assert::IsTrue(with.getRelation() == Clause::Relation::WITH);
        Assert::IsTrue(with.getFirstType() == Clause::Type::CONSTANT);
        Assert::IsTrue(with.getSecondType() == Clause::Type::INTEGER);
        Assert::IsTrue(with.getFirstArg() == "c");
        Assert::IsTrue(with.getSecondArg() == "10");

        // test content of the object
        Assert::IsTrue(qpp.validate("call c; Select c with c.procName = \"xD\""));
        qo = qpp.getQueryObject();
        with = qo.getClauses().at(0);
        Assert::IsTrue(with.getRelation() == Clause::Relation::WITH);
        Assert::IsTrue(with.getFirstType() == Clause::Type::CALLPROC);
        Assert::IsTrue(with.getSecondType() == Clause::Type::IDENT);
        Assert::IsTrue(with.getFirstArg() == "c");
        Assert::IsTrue(with.getSecondArg() == "xD");

        // test content of the object
        Assert::IsTrue(qpp.validate("call c; Select c with \"xD\" = c.procName"));
        qo = qpp.getQueryObject();
        with = qo.getClauses().at(0);
        Assert::IsTrue(with.getRelation() == Clause::Relation::WITH);
        Assert::IsTrue(with.getFirstType() == Clause::Type::CALLPROC);
        Assert::IsTrue(with.getSecondType() == Clause::Type::IDENT);
        Assert::IsTrue(with.getFirstArg() == "c");
        Assert::IsTrue(with.getSecondArg() == "xD");
    }

    TEST_METHOD(testRandom) {
        Assert::IsFalse(qpp.validate("procedure p; call c; Select  "));
    }
    };
}