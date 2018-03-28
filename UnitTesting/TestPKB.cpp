#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/pkb.h"
#include "../SPA/DesignExtractor.h"
#include <iostream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestPKB) {
	public:
		/**
		1. Test public APIs for SourceParser:
		*/
		
		TEST_METHOD(TestinsertVar) {

			// check whether pkb can insert variable correctly
			PKB* pkb = PKB::getPKBInstance();
			std::string var1 = "x";
			int varIndex1 = pkb->insertVar(var1);
			int expected1 = 1;
			Assert::AreEqual(varIndex1, expected1);

			// for existing var name, the method should return var index
			int varIndex2 = pkb->insertVar("x");
			int expected2 = 1;
			Assert::AreEqual(varIndex2, expected2);

		};

 		TEST_METHOD(TestinsertStmt) {

			// check whether pkb can insert stmt correctly
			PKB* pkb = PKB::getPKBInstance();
			int stmtNum1 = pkb->insertStmt(1, "ASSIGN", 1);
			int expected1 = 1;
			Assert::AreEqual(stmtNum1, expected1);

			// for existing stmt number, the method should return -1
			int stmtNum2 = pkb->insertStmt(1, "WHILE", 1);
			int expected2 = -1;
			Assert::AreEqual(stmtNum2, expected2);
		};

		TEST_METHOD(TestinsertProc) {
			PKB* pkb = PKB::getPKBInstance();
			// check whether pkb can insert a procedure correctly
			int procIndex1 = pkb->insertProc("MAIN");
			int expected1 = 1;
			Assert::AreEqual(procIndex1, expected1);

			// for existing procedure name, the method should return its index
			int procIndex2 = pkb->insertProc("MAIN");
			int expected2 = 1;
			Assert::AreEqual(procIndex2, expected2);

		};

		TEST_METHOD(TestinsertPatternExpression) {
			PKB::clearPKB();
			PKB* pkb = PKB::getPKBInstance();
			// check whether pkb can insert a pattern expression correctly
			int patternIndex1 = pkb->insertPatternExpression(1, "x");
			int expected1 = 1;
			Assert::AreEqual(patternIndex1, expected1);

			// for existing stmt num, the method should return -1
			int patternIndex2 = pkb->insertPatternExpression(1, "y");
			int expected2 = -1;
			Assert::AreEqual(patternIndex2, expected2);

		};

		TEST_METHOD(TestsetFollows) {
			PKB::clearPKB();
			PKB* pkb = PKB::getPKBInstance();
			int id1 = pkb->setFollows(1, 2);
			int expected1 = 1;
			Assert::AreEqual(id1, expected1);

			int id2 = pkb->setFollows(2, 3);
			int expected2 = 2;
			Assert::AreEqual(id2, expected2);

			// for existing stmt numbers, the method should return -1
			int id3 = pkb->setFollows(2, 4);
			int expected3 = -1;
			Assert::AreEqual(id3, expected3);

			int id4 = pkb->setFollows(6, 3);
			int expected4 = -1;
			Assert::AreEqual(id4, expected4);

		};

		TEST_METHOD(TestsetParents) {
			PKB::clearPKB();
			PKB* pkb = PKB::getPKBInstance();
			int id1 = pkb->setParent(1, 2);
			int expected1 = 1;
			Assert::AreEqual(id1, expected1);

			int id2 = pkb->setParent(1, 3);
			int expected2 = 1;
			Assert::AreEqual(id2, expected2);

			// for existing stmt numbers, the method should return -1
			int id3 = pkb->setParent(2, 3);
			int expected3 = -1;
			Assert::AreEqual(id3, expected3);

		};

		TEST_METHOD(TestinsertConst) {
			PKB::clearPKB();
			PKB* pkb = PKB::getPKBInstance();
			int id1 = pkb->insertConst(1);
			int expected1 = 1;
			Assert::AreEqual(id1, expected1);

			int id2 = pkb->insertConst(4);
			int expected2 = 4;
			Assert::AreEqual(id2, expected2);

			// for existing const, the method should return -1
			int id3 = pkb->insertConst(4);
			int expected3 = -1;
			Assert::AreEqual(id3, expected3);

		};

		
		/**
		2. Test public APIs for QE:
		*/
		TEST_METHOD(TestgetAllStmts) {
			PKB* pkb = PKB::getPKBInstance();
			pkb->insertStmt(1, "ASSIGN", 1);
			pkb->insertStmt(2, "ASSIGN", 1);
			pkb->insertStmt(3, "ASSIGN", 1);
			pkb->insertStmt(4, "ASSIGN", 1);

			std::unordered_set<int> stmtList = pkb->getAllStmts();
			std::unordered_set<int> expected = { 1, 2, 3, 4 };
			Assert::IsTrue(stmtList == expected);
		};

		TEST_METHOD(TestgetAllAssigns) {
			PKB::clearPKB();
			PKB* pkb = PKB::getPKBInstance();
			pkb->insertStmt(1, "ASSIGN", 1);
			pkb->insertStmt(2, "WHILE", 1);
			pkb->insertStmt(3, "ASSIGN", 1);
			pkb->insertStmt(4, "ASSIGN", 1);

			std::unordered_set<int> stmtList = pkb->getAllAssigns();
			std::unordered_set<int> expected = { 1, 3, 4 };
			Assert::IsTrue(stmtList == expected);
		};

		TEST_METHOD(TestgetAllVariables) {
			PKB* pkb = PKB::getPKBInstance();
			pkb->insertVar("x");
			pkb->insertVar("y");
			pkb->insertVar("z");
			pkb->insertVar("t");

			std::unordered_set<int> varList = pkb->getAllVariables();
			std::unordered_set<int> expected = {1,2,3,4};
			Assert::IsTrue(varList == expected);
		};

		TEST_METHOD(TestgetAllProcedures) {
			PKB* pkb = PKB::getPKBInstance();
			pkb->insertProc("MAIN");
			pkb->insertProc("SECOND");

			std::unordered_set<int> procList = pkb->getAllProcedures();
			std::unordered_set<int> expected = {1, 2};
			Assert::IsTrue(procList == expected);
		};

		TEST_METHOD(TestgetAllConsts) {
			PKB* pkb = PKB::getPKBInstance();
			pkb->insertConst(1);
			pkb->insertConst(5);
			pkb->insertConst(77);
			pkb->insertConst(30);

			std::unordered_set<int> constList = pkb->getAllConstants();
			std::unordered_set<int> expected = { 1, 5, 77, 30 };
			Assert::IsTrue(constList == expected);
		};

		TEST_METHOD(TesthasSubexpr) {
			PKB::clearPKB();
			PKB* pkb = PKB::getPKBInstance();
            pkb->insertPatternExpression(1, "+xy");


			// for existing procedure name, the method should return -1
			bool result = pkb->hasSubexpr(1, "x");
			Assert::IsTrue(result);

		};

		// Test follows methods
		TEST_METHOD(TestgetFollowsPairs) {
			PKB::clearPKB();
			PKB* pkb = PKB::getPKBInstance();
			pkb->setFollows(1, 2);
			pkb->setFollows(2, 3);
			pkb->setFollows(3, 4);

			std::unordered_set<std::pair<int, int>, pair_hash> p = pkb->getFollowsPairs();
			std::unordered_set<std::pair<int, int>, pair_hash> expected = {};
			expected.insert(std::make_pair(1, 2));
			expected.insert(std::make_pair(2, 3));
			expected.insert(std::make_pair(3, 4));
			Assert::IsTrue(p == expected);
		};

		TEST_METHOD(TestgetFollowing) {
			PKB::clearPKB();
			PKB* pkb = PKB::getPKBInstance();
			pkb->setFollows(1, 2);
			pkb->setFollows(2, 3);
			pkb->setFollows(3, 4);

			int NO_RESULT = -1;
			int r1 = pkb->getFollowing(1);
			int expected1 = 2;
			Assert::AreEqual(r1, expected1);

			int r2 = pkb->getFollowing(0);
			int expected2 = NO_RESULT;
			Assert::AreEqual(r2, expected2);
		};

		// Test follows* methods:
		TEST_METHOD(TestgetTransFollowing) {
			PKB::clearPKB();
			PKB* pkb = PKB::getPKBInstance();
			pkb->setFollows(1, 2);
			pkb->setFollows(2, 3);
			pkb->setFollows(3, 4);
			DesignExtractor::populateFollowsStarTable();

			std::unordered_set<int> r1 = pkb->getTransFollowing(1);
			std::unordered_set<int> expected1 = {2,3,4};
			Assert::IsTrue(r1 == expected1);

			std::unordered_set<int> r2 = pkb->getTransFollowing(2);
			std::unordered_set<int> expected2 = { 3,4 };
			Assert::IsTrue(r2 == expected2);

			// test no result
			std::unordered_set<int> r3 = pkb->getTransFollowing(9);
			std::unordered_set<int> expected3 = {};
			Assert::IsTrue(r3 == expected3);
		};

		TEST_METHOD(TestgetTransFollowingDebug) {
			// (6,7) (7,8) (8,10) (10, 11) (11,12) (12, 16)
			PKB::clearPKB();
			PKB* pkb = PKB::getPKBInstance();
			pkb->setFollows(6, 7);
			pkb->setFollows(7, 8);
			pkb->setFollows(8, 10);
			pkb->setFollows(10, 11);
			pkb->setFollows(11, 12);
			pkb->setFollows(12, 16);

			DesignExtractor::populateFollowsStarTable();

			std::unordered_set<std::pair<int, int>, pair_hash> result = pkb->getTransFollowsPairs();
			Logger::WriteMessage("All follow* pairs:");
			for (auto &s : result) {
				std::string pairStr = std::to_string(s.first) + "," + std::to_string(s.second);
				Logger::WriteMessage(pairStr.c_str());
			}
		};


		TEST_METHOD(TestgetTransFollowedBy) {
			PKB::clearPKB();
			PKB* pkb = PKB::getPKBInstance();
			pkb->setFollows(1, 2);
			pkb->setFollows(2, 3);
			pkb->setFollows(3, 4);
			DesignExtractor::populateFollowsStarTable();

			std::unordered_set<int> r1 = pkb->getTransFollowedBy(2);
			std::unordered_set<int> expected1 = { 1 };
			Assert::IsTrue(r1 == expected1);

			std::unordered_set<int> r2 = pkb->getTransFollowedBy(4);
			std::unordered_set<int> expected2 = { 3,2,1 };
			Assert::IsTrue(r2 == expected2);

			// test no result
			std::unordered_set<int> r3 = pkb->getTransFollowedBy(9);
			std::unordered_set<int> expected3 = {};
			Assert::IsTrue(r3 == expected3);
		};

		// Test Parent methods:
		TEST_METHOD(TestgetParentPairs) {
			PKB::clearPKB();
			PKB* pkb = PKB::getPKBInstance();
			pkb->setParent(1, 2);
			pkb->setParent(1, 3);
			pkb->setParent(3, 4);

			std::unordered_set<std::pair<int, int>, pair_hash> r1 = pkb->getParentPairs();
			std::unordered_set<std::pair<int, int>, pair_hash> expected1 = {};
			expected1.insert(std::make_pair(1, 2));
			expected1.insert(std::make_pair(1, 3));
			expected1.insert(std::make_pair(3, 4));
			Assert::IsTrue(r1 == expected1);
		};

		TEST_METHOD(TestgetChildrenOf) {
			PKB::clearPKB();
			PKB* pkb = PKB::getPKBInstance();
			pkb->setParent(1, 2);
			pkb->setParent(1, 3);
			pkb->setParent(3, 4);

			std::unordered_set<int> r1 = pkb->getChildrenOf(1);
			std::unordered_set<int> expected1 = {2, 3};
			Assert::IsTrue(r1 == expected1);

			std::unordered_set<int> r2 = pkb->getChildrenOf(3);
			std::unordered_set<int> expected2 = { 4 };
			Assert::IsTrue(r2 == expected2);
		};

		// Test Parent* methods:
		TEST_METHOD(TestgetTransParentOf) {
			PKB::clearPKB();
			PKB* pkb = PKB::getPKBInstance();
			pkb->setParent(2, 3);
			pkb->setParent(1, 2);
			DesignExtractor::populateParentStarTable();

			std::unordered_set<int> r1 = pkb->getTransParentOf(2);
			std::unordered_set<int> expected1 = { 1 };
			Assert::IsTrue(r1 == expected1);

			std::unordered_set<int> r2 = pkb->getTransParentOf(3);
			std::unordered_set<int> expected2 = { 2,1 };
			Assert::IsTrue(r2 == expected2);

			// test no result
			std::unordered_set<int> r3 = pkb->getTransParentOf(9);
			std::unordered_set<int> expected3 = {};
			Assert::IsTrue(r3 == expected3);

			
			int i1 = pkb->setParent(16, 17);
			int i2 = pkb->setParent(16, 19);
			int i3 = pkb->setParent(17, 18);
			int i4 = pkb->setParent(19, 20);
			DesignExtractor::populateParentStarTable();

			std::unordered_set<int> r4 = pkb->getTransParentOf(18);
			std::unordered_set<int> expected4 = { 17, 16};
			Assert::IsTrue(r4 == expected4);
			
		};

		TEST_METHOD(TestgetTransChildrenOf) {
			PKB::clearPKB();
			PKB* pkb = PKB::getPKBInstance();
			pkb->setParent(1, 2);
			pkb->setParent(1, 3);
			pkb->setParent(3, 4);
			DesignExtractor::populateParentStarTable();

			std::unordered_set<int> r1 = pkb->getTransChildrenOf(1);
			std::unordered_set<int> expected1 = { 2,3,4 };
			Assert::IsTrue(r1 == expected1);

			std::unordered_set<int> r2 = pkb->getTransChildrenOf(3);
			std::unordered_set<int> expected2 = { 4 };
			Assert::IsTrue(r2 == expected2);

			// test no result
			std::unordered_set<int> r3 = pkb->getTransChildrenOf(9);
			std::unordered_set<int> expected3 = {};
			Assert::IsTrue(r3 == expected3);
		};

		TEST_METHOD(TestgetUses) {
			PKB::clearPKB();
			PKB* pkb = PKB::getPKBInstance();
			pkb->setUses(1, 1);
			pkb->setUses(1, 2);
			pkb->setUses(2, 1);
			pkb->setUses(3, 1);

			std::unordered_set<int> r1 = pkb->getUsedByStmt(1);
			std::unordered_set<int> expected1 = { 1,2 };
			Assert::IsTrue(r1 == expected1);

			std::unordered_set<int> r2 = pkb->getStmtUsing(1);
			std::unordered_set<int> expected2 = { 1,2,3 };
			Assert::IsTrue(r2 == expected2);

			// test no result
			std::unordered_set<int> r3 = pkb->getUsedByStmt(9);
			std::unordered_set<int> expected3 = {};
			Assert::IsTrue(r3 == expected3);
		};

		TEST_METHOD(TestgetModifies) {
			PKB::clearPKB();
			PKB* pkb = PKB::getPKBInstance();
			pkb->setModifies(1, 1);
			pkb->setModifies(2, 1);
			pkb->setModifies(3, 1);
			pkb->setModifies(1, 2);

			std::unordered_set<int> r1 = pkb->getModifiedByStmt(1);
			std::unordered_set<int> expected1 = { 1,2 };
			Assert::IsTrue(r1 == expected1);

			std::unordered_set<int> r2 = pkb->getStmtModifying(1);
			std::unordered_set<int> expected2 = { 1,2,3 };
			Assert::IsTrue(r2 == expected2);

			// test no result
			std::unordered_set<int> r3 = pkb->getStmtModifying(9);
			std::unordered_set<int> expected3 = {};
			Assert::IsTrue(r3 == expected3);
		};

		TEST_METHOD(TestgetProcedureTransCalling) {
			PKB::clearPKB();
			PKB* pkb = PKB::getPKBInstance();
			/**
			source:
			procedure First {
			    call Second;  // 1
			}
			procedure Second {
			    call Third; // 2
			} 
			procedure Third {
			    x = 1; // 3
			}
			*/
			int p1 = pkb->insertProc("First");
			int p2 = pkb->insertProc("Second");
			int p3 = pkb->insertProc("Third");
			pkb->setCalls(1, p1, p2);
			pkb->setCalls(2, p2, p3);
			DesignExtractor::populateCallsStarTable();

			std::unordered_set<std::pair<int, int>, pair_hash> pairs =
				pkb->getTransCallsProcedurePairs();

			Logger::WriteMessage("all *calls pairs:");
			for (auto &s : pairs) {
				std::string pairStr = std::to_string(s.first) + "," + std::to_string(s.second);
				Logger::WriteMessage(pairStr.c_str());
			}

			std::unordered_set<int> result = pkb->getProcedureTransCalling(3);
			std::unordered_set<int> expected = { 2, 1 };

			Logger::WriteMessage("p trans calls 3:");
			for (auto &s : result) {
				Logger::WriteMessage(std::to_string(s).c_str());
			}
			Assert::IsTrue(result == expected);
		};
		
		TEST_METHOD(TestgetProcedureUsing) {
			/**
			source:
			procedure First {
			    x = y;        // 1 
			    call Second;  // 2 
			}
			procedure Second {
			    z = 1 + ddd;  // 3
			    call Third;   // 4
			}
			procedure Third {
			    y = c;        // 5
			}
			*/
			PKB::clearPKB();
			PKB* pkb = PKB::getPKBInstance();

			int p1 = pkb->insertProc("First");
			int p2 = pkb->insertProc("Second");
			int p3 = pkb->insertProc("Third");
			pkb->setCalls(2, p1, p2);
			pkb->setCalls(4, p2, p3);
			DesignExtractor::populateCallsStarTable();

			/** var table:
			x        1
			y        2
			z        3
			ddd      4
			c        5
			*/
			pkb->setProcUses(1, 2);
			pkb->setProcUses(2, 4);
			pkb->setProcUses(3, 5);
			pkb->insertStmt(2, "CALL", 1);
			pkb->insertStmt(4, "CALL", 2);
			DesignExtractor::setProcCallUses(p2, p3);
			DesignExtractor::setProcCallUses(p1, p2);
			DesignExtractor::setCallStmtUses();

			std::unordered_set<int> result = pkb->getUsedByProcedure(1);
			std::unordered_set<int> expected = { 2, 4, 5 };
			Assert::IsTrue(result == expected);

			std::unordered_set<int> result2 = pkb->getUsedByStmt(2);
			std::unordered_set<int> expected2 = { 4, 5 };
			Assert::IsTrue(result2 == expected2);
		};
	};
}