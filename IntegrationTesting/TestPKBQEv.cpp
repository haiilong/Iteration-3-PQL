#include "stdafx.h"
#include "PKB.h"
#include "QueryEvaluator.h"
#include "QueryObject.h"
#include "DesignExtractor.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestQueryEvaluator) {
public:
	TEST_METHOD(TestGetAssignmentStatements) {
		PKB::clearPKB();
		PKB* pkb = PKB::getPKBInstance();
		pkb->insertStmt(1, "ASSIGN", 1);
		pkb->insertStmt(2, "ASSIGN", 1);
		pkb->insertStmt(3, "ASSIGN", 1);

		auto message = L"unqualified assignment synonym query should return '1, 3, 11'";
		QueryObject qo;
		QueryEvaluator queryEvaluator(*pkb);
		qo.addSelectClause("assign", "a");
		auto l = queryEvaluator.evaluate(qo);
		std::string s1 = "1", s2 = "2", s3 = "3";
		Assert::IsTrue(l == std::list<std::string>{s1, s2, s3}, message);
	}
	/*
	TEST_METHOD(TestGetAllProcs) {
		PKB::clearPKB();
		PKB* pkb = PKB::getPKBInstance();
		pkb->insertProc("Proc1");
		auto message = L"unqualified procedure synonym query should return 'Proc1, Proc5, Proc9'";
		QueryObject qo;
		QueryEvaluator queryEvaluator(*pkb);
		qo.addSelectClause("procedure", "p");
		auto l = queryEvaluator.evaluate(qo);
		std::string s1 = "Proc1";
		Assert::IsTrue(l == std::list<std::string>{s1}, message);
	}
	*/
	TEST_METHOD(TestGetAllStmts) {
		// check whether pkb can insert variable correctly
		PKB::clearPKB();
		PKB* pkb = PKB::getPKBInstance();
		pkb->insertStmt(1, "ASSIGN", 1);
		pkb->insertStmt(2, "ASSIGN", 1);
		pkb->insertStmt(3, "ASSIGN", 1);

		auto message = L"unqualified statement synonym query should return '1, 3, 7, 11'";
		QueryObject qo;
		QueryEvaluator queryEvaluator(*pkb);
		qo.addSelectClause("stmt", "s");
		auto l = queryEvaluator.evaluate(qo);
		std::string s1 = "1", s2 = "2", s3 = "3";
		Assert::IsTrue(l == std::list<std::string>{s1, s2, s3}, message);
	}
	TEST_METHOD(TestGetAllVars) {
		// check whether pkb can insert variable correctly
		PKB::clearPKB();
		PKB* pkb = PKB::getPKBInstance();
		pkb->insertVar("v");
		pkb->insertVar("x");
		pkb->insertVar("z");

		auto message = L"unqualified variable synonym query should return 'v, x, y'";
		QueryObject qo;
		QueryEvaluator queryEvaluator(*pkb);
		qo.addSelectClause("variable", "v");
		auto l = queryEvaluator.evaluate(qo);
		std::string s1 = "v", s2 = "x", s3 = "z";
		Assert::IsTrue(l == std::list<std::string>{s1, s2, s3}, message);
	}
	TEST_METHOD(testFollowsIntegerSynonym) {
		PKB::clearPKB();
		PKB* pkb = PKB::getPKBInstance();
		QueryEvaluator queryEvaluator(*pkb);
		pkb->insertStmt(1, "ASSIGN", 1);
		pkb->insertStmt(2, "ASSIGN", 1);
		pkb->insertStmt(3, "ASSIGN", 1);
		pkb->insertStmt(4, "ASSIGN", 1);
		pkb->setFollows(1, 2);
		pkb->setFollows(2, 3);
		pkb->setFollows(3, 4);

		QueryObject qo;
		qo.addSelectClause("assign", "a");
		qo.addClause("Follows", "integer", "4", "assign", "a");

		auto l = queryEvaluator.evaluate(qo);
		Assert::IsTrue(l == std::list<std::string>{ });
		
		QueryObject qo1;
		qo1.addSelectClause("assign", "a");
		qo1.addClause("Follows", "integer", "2", "assign", "a");

		auto l1 = queryEvaluator.evaluate(qo1);
		Assert::IsTrue(l1 == std::list<std::string>{ "3" });

		QueryObject qo2;
		qo2.addSelectClause("stmt", "s");
		qo2.addClause("Follows", "integer", "2", "stmt", "s");

		auto l2 = queryEvaluator.evaluate(qo2);
		Assert::IsTrue(l2 == std::list<std::string>{ "3" });
	}

	TEST_METHOD(testFollowedBySynonymInteger) {
		PKB::clearPKB();
		PKB* pkb = PKB::getPKBInstance();
		QueryEvaluator queryEvaluator(*pkb);
		pkb->insertStmt(1, "ASSIGN", 1);
		pkb->insertStmt(2, "ASSIGN", 1);
		pkb->insertStmt(3, "ASSIGN", 1);
		pkb->insertStmt(4, "ASSIGN", 1);
		pkb->setFollows(1, 2);
		pkb->setFollows(2, 3);
		pkb->setFollows(3, 4);

		QueryObject qo;
		qo.addSelectClause("stmt", "s");
		qo.addClause("Follows", "stmt", "s", "integer", "2");

		auto l = queryEvaluator.evaluate(qo);
		Assert::IsTrue(l == std::list<std::string>{ "1" });

		QueryObject qo1;
		qo1.addSelectClause("stmt", "s");
		qo1.addClause("Follows", "stmt", "s", "integer", "3");

		auto l1 = queryEvaluator.evaluate(qo1);
		Assert::IsTrue(l1 == std::list<std::string>{ "2" });

		
	}

	TEST_METHOD(testTransFollowsIntegerInteger) {
		/** Source code for reference
		procedure First {
		    if ddd then {
		        while y {
		            ddd = 0*9; }      }
		    else {
		        while y {
		            ddd = z*x; }
		    }
			aaa = 1;
		}
		*/
		PKB::clearPKB();
		PKB* pkb = PKB::getPKBInstance();
		QueryEvaluator queryEvaluator(*pkb);
		pkb->insertVar("ddd");
		pkb->insertVar("y");
		pkb->insertVar("aaa");

		pkb->insertStmt(1, "IF", 1);
		pkb->insertStmt(2, "WHILE", 1);
		pkb->insertStmt(3, "ASSIGN", 1);
		pkb->insertStmt(4, "WHILE", 1);
		pkb->insertStmt(5, "ASSIGN", 1);
		pkb->insertStmt(6, "ASSIGN", 1);
		
		pkb->setFollows(1, 6);
		DesignExtractor::populateFollowsStarTable();

		QueryObject qo;
		qo.addSelectClause("variable", "v");
		qo.addClause("Follows*", "integer", "3", "integer", "5");

		auto l = queryEvaluator.evaluate(qo);
		for (auto &s : l) {
			Logger::WriteMessage(s.c_str());
		}
		Assert::IsTrue(l == std::list<std::string>{ });

		QueryObject qo1;
		qo1.addSelectClause("variable", "v");
		qo1.addClause("Follows*", "integer", "1", "integer", "6");

		auto l1 = queryEvaluator.evaluate(qo1);
		for (auto &s : l1) {
			Logger::WriteMessage(s.c_str());
		}
		Assert::IsTrue(l1 == std::list<std::string>{ "ddd", "y", "aaa"});
	}

	// Test follows relationship with an integer and a wildcard as params
	TEST_METHOD(testFollowsIntegerWildcard) {
		/** Source code for reference
		procedure First {
		if ddd then {
		while y {
		ddd = 0*9; }      }
		else {
		while y {
		ddd = z*x; }
		}
		aaa = 1;
		}
		*/
		PKB::clearPKB();
		PKB* pkb = PKB::getPKBInstance();
		QueryEvaluator queryEvaluator(*pkb);
		pkb->insertVar("ddd");
		pkb->insertVar("y");
		pkb->insertVar("aaa");

		pkb->insertStmt(1, "IF", 1);
		pkb->insertStmt(2, "WHILE", 1);
		pkb->insertStmt(3, "ASSIGN", 1);
		pkb->insertStmt(4, "WHILE", 1);
		pkb->insertStmt(5, "ASSIGN", 1);
		pkb->insertStmt(6, "ASSIGN", 1);

		pkb->setFollows(1, 6);
		DesignExtractor::populateFollowsStarTable();

		QueryObject qo;
		qo.addSelectClause("variable", "v");
		qo.addClause("Follows", "integer", "2", "wildcard", "_");

		auto l = queryEvaluator.evaluate(qo);
		for (auto &s : l) {
			Logger::WriteMessage(s.c_str());
		}
		Assert::IsTrue(l == std::list<std::string>{ });


		QueryObject qo1;
		qo1.addSelectClause("variable", "v");
		qo1.addClause("Follows*", "integer", "1", "wildcard", "_");

		auto l1 = queryEvaluator.evaluate(qo1);
		for (auto &s : l1) {
			Logger::WriteMessage(s.c_str());
		}
		Assert::IsTrue(l1 == std::list<std::string>{ "ddd", "y", "aaa"});

		QueryObject qo2;
		qo2.addSelectClause("variable", "v");
		qo2.addClause("Follows", "wildcard", "_", "integer", "3");

		auto l2 = queryEvaluator.evaluate(qo2);
		for (auto &s : l2) {
			Logger::WriteMessage(s.c_str());
		}
		Assert::IsTrue(l2 == std::list<std::string>{ });
	}

	TEST_METHOD(testParentSynonymInteger) {
		/** Source code for reference
		procedure First {
		if ddd then { //1
		while y { //2
		ddd = 0*9; }      } //3
		else {
		while y { //4
		ddd = z*x; } //5
		}
		aaa = 1; //6
		}
		*/
		PKB::clearPKB();
		PKB* pkb = PKB::getPKBInstance();
		QueryEvaluator queryEvaluator(*pkb);
		pkb->insertVar("ddd");
		pkb->insertVar("y");
		pkb->insertVar("aaa");

		pkb->insertStmt(1, "IF", 1);
		pkb->insertStmt(2, "WHILE", 1);
		pkb->insertStmt(3, "ASSIGN", 1);
		pkb->insertStmt(4, "WHILE", 1);
		pkb->insertStmt(5, "ASSIGN", 1);
		pkb->insertStmt(6, "ASSIGN", 1);

		pkb->setFollows(1, 6);

		pkb->setParent(1, 2);
		pkb->setParent(2, 3);
		pkb->setParent(1, 4);
		pkb->setParent(4, 5);

		QueryObject qo;
		qo.addSelectClause("stmt", "s");
		qo.addClause("Parent", "stmt", "s", "integer", "3");

		auto l = queryEvaluator.evaluate(qo);
		for (auto &s : l) {
			Logger::WriteMessage(s.c_str());
		}
		Assert::IsTrue(l == std::list<std::string>{"2" });
		

		QueryObject qo1;
		qo1.addSelectClause("stmt", "s");
		qo1.addClause("Parent", "integer", "4", "stmt", "s");

		auto l1 = queryEvaluator.evaluate(qo1);
		for (auto &s : l1) {
			Logger::WriteMessage(s.c_str());
		}
		Assert::IsTrue(l1 == std::list<std::string>{ "5"});
	}

	TEST_METHOD(testParentIntegerInteger) {
		/** Source code for reference
		procedure First {
		if ddd then { //1
		while y { //2
		ddd = 0*9; }      } //3
		else {
		while y { //4
		ddd = z*x; } //5
		}
		aaa = 1; //6
		}
		*/
		PKB::clearPKB();
		PKB* pkb = PKB::getPKBInstance();
		QueryEvaluator queryEvaluator(*pkb);
		pkb->insertVar("ddd");
		pkb->insertVar("y");
		pkb->insertVar("aaa");

		pkb->insertStmt(1, "IF", 1);
		pkb->insertStmt(2, "WHILE", 1);
		pkb->insertStmt(3, "ASSIGN", 1);
		pkb->insertStmt(4, "WHILE", 1);
		pkb->insertStmt(5, "ASSIGN", 1);
		pkb->insertStmt(6, "ASSIGN", 1);

		pkb->setFollows(1, 6);

		pkb->setParent(1, 2);
		pkb->setParent(2, 3);
		pkb->setParent(1, 4);
		pkb->setParent(4, 5);

		
		QueryObject qo;
		qo.addSelectClause("variable", "v");
		qo.addClause("Parent", "integer","3", "integer", "5");

		auto l = queryEvaluator.evaluate(qo);
		for (auto &s : l) {
			Logger::WriteMessage(s.c_str());
		}
		Assert::IsTrue(l == std::list<std::string>{ });
		
		// this one currently fails: returns empty list
		QueryObject qo1;
		qo1.addSelectClause("variable", "v");
		qo1.addClause("Parent", "integer", "4", "integer", "5");

		auto l1 = queryEvaluator.evaluate(qo1);
		for (auto &s : l1) {
			Logger::WriteMessage(s.c_str());
		}
		Assert::IsTrue(l1 == std::list<std::string>{"ddd", "y", "aaa"});
		
	}

	TEST_METHOD(testParentIntegerWildcard) {
		/** Source code for reference
		procedure First {
		if ddd then { //1
		while y { //2
		ddd = 0*9; }      } //3
		else {
		while y { //4
		ddd = z*x; } //5
		}
		aaa = 1; //6
		}
		*/
		PKB::clearPKB();
		PKB* pkb = PKB::getPKBInstance();
		QueryEvaluator queryEvaluator(*pkb);
		pkb->insertVar("ddd");
		pkb->insertVar("y");
		pkb->insertVar("aaa");

		pkb->insertStmt(1, "IF", 1);
		pkb->insertStmt(2, "WHILE", 1);
		pkb->insertStmt(3, "ASSIGN", 1);
		pkb->insertStmt(4, "WHILE", 1);
		pkb->insertStmt(5, "ASSIGN", 1);
		pkb->insertStmt(6, "ASSIGN", 1);

		pkb->setFollows(1, 6);

		pkb->setParent(1, 2);
		pkb->setParent(2, 3);
		pkb->setParent(1, 4);
		pkb->setParent(4, 5);


		QueryObject qo;
		qo.addSelectClause("variable", "v");
		qo.addClause("Parent", "integer", "2", "wildcard", "_");

		auto l = queryEvaluator.evaluate(qo);
		for (auto &s : l) {
			Logger::WriteMessage(s.c_str());
		}
		Assert::IsTrue(l == std::list<std::string>{"ddd", "y", "aaa" });

		// this one currently fails: it returns all vars
		QueryObject qo1;
		qo1.addSelectClause("variable", "v");
		qo1.addClause("Parent", "wildcard", "_", "integer", "1");

		auto l1 = queryEvaluator.evaluate(qo1);
		for (auto &s : l1) {
			Logger::WriteMessage(s.c_str());
		}
		Assert::IsTrue(l1 == std::list<std::string>{});

	}
	// Test Parent* methods:
	TEST_METHOD(TestgetTransParentOf) {
		PKB::clearPKB();
		PKB* pkb = PKB::getPKBInstance();
		QueryEvaluator queryEvaluator(*pkb);
		pkb->setParent(16, 17);
		pkb->setParent(17, 18);
		pkb->setParent(16, 19);
		pkb->setParent(19, 20);

		std::unordered_set<int> r1 = pkb->getTransParentOf(2);
		std::unordered_set<int> expected1 = { 1 };
		//Assert::IsTrue(r1 == expected1);

		std::unordered_set<int> r2 = pkb->getTransParentOf(4);
		std::unordered_set<int> expected2 = { 3,1 };
		//Assert::IsTrue(r2 == expected2);

		// test no result
		std::unordered_set<int> r3 = pkb->getTransParentOf(9);
		std::unordered_set<int> expected3 = {};
		//Assert::IsTrue(r3 == expected3);
	};

		TEST_METHOD(TestgetTransChildrenOf) {
		PKB::clearPKB();
		PKB* pkb = PKB::getPKBInstance();
		QueryEvaluator queryEvaluator(*pkb);
		pkb->insertStmt(16, "WHILE", 1);
		pkb->insertStmt(17, "ASSIGN", 1);
		pkb->insertStmt(18, "ASSIGN", 1);
		pkb->insertStmt(19, "ASSIGN", 1);
		pkb->insertStmt(20, "ASSIGN", 1);
		pkb->setParent(16, 17);
		pkb->setParent(16, 19);
		pkb->setParent(17, 18);
		pkb->setParent(19, 20);

		DesignExtractor::populateParentStarTable();

		QueryObject qo;
		qo.addSelectClause("stmt", "s");
		qo.addClause("Parent*", "integer", "16", "stmt", "s");

		std::list<std::string> l = queryEvaluator.evaluate(qo);
		for (auto &s : l) {
			Logger::WriteMessage(s.c_str());
		}
		l.sort();
		std::list<std::string> expected = { "17", "18", "19", "20" };
		expected.sort();
		Assert::IsTrue(l == expected);
	};

	TEST_METHOD(testParentStarIntegerInteger) {
		/** Source code for reference
		procedure First {
		if ddd then { //1
		while y { //2
		ddd = 0*9; }      } //3
		else {
		while y { //4
		ddd = z*x; } //5
		}
		aaa = 1; //6
		}
		*/
		PKB::clearPKB();
		PKB* pkb = PKB::getPKBInstance();
		QueryEvaluator queryEvaluator(*pkb);
		pkb->insertVar("ddd");
		pkb->insertVar("y");
		pkb->insertVar("aaa");

		pkb->insertStmt(1, "IF", 1);
		pkb->insertStmt(2, "WHILE", 1);
		pkb->insertStmt(3, "ASSIGN", 1);
		pkb->insertStmt(4, "WHILE", 1);
		pkb->insertStmt(5, "ASSIGN", 1);
		pkb->insertStmt(6, "ASSIGN", 1);

		pkb->setFollows(1, 6);

		pkb->setParent(1, 2);
		pkb->setParent(2, 3);
		pkb->setParent(1, 4);
		pkb->setParent(4, 5);

		DesignExtractor::populateFollowsStarTable();
		DesignExtractor::populateParentStarTable();
		
		QueryObject qo;
		qo.addSelectClause("variable", "v");
		qo.addClause("Parent*", "integer", "2", "integer", "3");

		auto l = queryEvaluator.evaluate(qo);
		for (auto &s : l) {
			Logger::WriteMessage(s.c_str());
		}
		Assert::IsTrue(l == std::list<std::string>{"ddd", "y", "aaa" });
	
		QueryObject qo1;
		qo1.addSelectClause("variable", "v");
		qo1.addClause("Parent*", "integer", "4", "integer", "5");

		auto l1 = queryEvaluator.evaluate(qo1);
		for (auto &s : l1) {
			Logger::WriteMessage(s.c_str());
		}
		Assert::IsTrue(l1 == std::list<std::string>{"ddd", "y", "aaa"});
		
		QueryObject qo2;
		qo2.addSelectClause("variable", "v");
		qo2.addClause("Parent*", "integer", "1", "integer", "6");

		auto l2 = queryEvaluator.evaluate(qo2);
		for (auto &s : l2) {
			Logger::WriteMessage(s.c_str());
		}
		Assert::IsTrue(l2 == std::list<std::string>{ });
	}

private:
	};
};
