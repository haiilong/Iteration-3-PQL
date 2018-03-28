#include "stdafx.h"

#include "ResultFetcherStub.h"
#include "QueryEvaluator.h"
#include "QueryObject.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestQueryEvaluatorGetAll) {
public:
	TEST_METHOD_INITIALIZE(constructResultsFetcher) {
		resultFetcher.allStmts = { 1, 3, 7, 11 };
		resultFetcher.allAssigns = { 1, 3, 11 };
		resultFetcher.allProcedures = { 4 };
		resultFetcher.allVariables = { 2, 6, 9 };
	}
	
	TEST_METHOD(testGetAllAssigns) {
		auto message = L"unqualified assignment synonym query should return '1, 3, 11'";

		QueryObject qo;
		QueryEvaluator queryEvaluator(resultFetcher);
		qo.addSelectClause("assign", "a");
		std:list<std::string> l = queryEvaluator.evaluate(qo);
	
		Assert::IsTrue(l == std::list<std::string>{"1", "3", "11"}, message);
	}

	TEST_METHOD(testGetAllProcedures) {
		auto message = L"unqualified procedure synonym query should return '4'";

		QueryObject qo;
		QueryEvaluator queryEvaluator(resultFetcher);
		qo.addSelectClause("procedure", "p");
		std:list<std::string> l = queryEvaluator.evaluate(qo);
		Assert::IsTrue(l == std::list<std::string>{"4"}, message);
	}

	TEST_METHOD(testGetAllStmts) {
		auto message = L"unqualified statement synonym query should return '1, 3, 7, 11'";

		QueryObject qo;
		QueryEvaluator queryEvaluator(resultFetcher);

		qo.addSelectClause("stmt", "s");
		std:list<std::string> l = queryEvaluator.evaluate(qo);
		Assert::IsTrue(l == std::list<std::string>{"1", "3", "11", "7"}, message);
	}
	TEST_METHOD(testGetAllProgLines) {
		auto message = L"unqualified prog_line synonym query should return '1, 3, 7, 11'";

		QueryObject qo;
		QueryEvaluator queryEvaluator(resultFetcher);
		qo.addSelectClause("prog_line", "n");
		auto l = queryEvaluator.evaluate(qo);
		Assert::IsTrue(l == std::list<std::string>{"1", "3", "11", "7"}, message);
	}
	TEST_METHOD(testGetAllVariables) {
		auto message = L"unqualified variable synonym query should return '2, 6, 9'";

		QueryObject qo;
		QueryEvaluator queryEvaluator(resultFetcher);
		qo.addSelectClause("variable", "v");
		auto l = queryEvaluator.evaluate(qo);
		Assert::IsTrue(l == std::list<std::string>{"2", "6", "9"}, message);
	}
	TEST_METHOD(testNewState) {
		QueryObject qo;
		QueryEvaluator queryEvaluator(resultFetcher);
		qo.addSelectClause("variable", "v");
		auto l = queryEvaluator.evaluate(qo);
		Assert::IsTrue(l == std::list<std::string>{"2", "6", "9"});
		qo = QueryObject();
		qo.addSelectClause("stmt", "s");
		l = queryEvaluator.evaluate(qo);
		Assert::IsTrue(l == std::list<std::string>{"1", "3", "11", "7"});
	}
private:
	ResultFetcherStub resultFetcher;
	};


	TEST_CLASS(TestQueryEvaluatorSingleFollows) {
public:
	TEST_METHOD_INITIALIZE(constructResultsFetcher) {
		resultFetcher.allStmts = { 1, 2, 3, 4, 10, 11, 12, 14, 16 };
		resultFetcher.allAssigns = { 1, 3, 11 };
		resultFetcher.allWhiles = { 2, 4 };
		resultFetcher.allIfs = { 10, 12 };
		resultFetcher.allCalls = { 14 };
		resultFetcher.followsPairs = {
			{ 1, 2 },
			{ 2, 3 },
			{ 3, 4 },
			{ 10, 11 },
			{ 14, 16}
		};
	}
	TEST_METHOD(testFollowsSynonymSynonym) {
		QueryObject qo;
		QueryEvaluator queryEvaluator(resultFetcher);

		qo.addSelectClause("stmt", "s1");
		qo.addClause("Follows", "stmt", "s1", "stmt", "s2");
		auto l = queryEvaluator.evaluate(qo);
		Assert::IsTrue(l == std::list<std::string>{ "1", "2", "3", "10", "14"});
	}
	TEST_METHOD(testFollowsAssignSynonymSynonym) {
		QueryObject qo;
		QueryEvaluator queryEvaluator(resultFetcher);

		qo.addSelectClause("assign", "a");
		qo.addClause("Follows", "assign", "a", "stmt", "s");

		auto l = queryEvaluator.evaluate(qo);
		Assert::IsTrue(l == std::list<std::string>{ "1", "3"});
	}
	TEST_METHOD(testFollowsSelfSynonymSynonym) {
		QueryObject qo;
		QueryEvaluator queryEvaluator(resultFetcher);

		qo.addSelectClause("stmt", "s1");
		qo.addClause("Follows", "stmt", "s1", "stmt", "s1");

		auto l = queryEvaluator.evaluate(qo);
		Assert::IsTrue(l == std::list<std::string>{ });
	}
	TEST_METHOD(testFollowsSynonymSynonymNoResult) {
		QueryObject qo;
		QueryEvaluator queryEvaluator(resultFetcher);
		resultFetcher.allAssigns = {  };

		qo.addSelectClause("assign", "a1");
		qo.addClause("Follows", "assign", "a1", "assign", "a2");

		auto l = queryEvaluator.evaluate(qo);
		Assert::IsTrue(l == std::list<std::string>{ });
	}

	TEST_METHOD(testFollowsWhileSynonymSynonym) {
		QueryObject qo;
		QueryEvaluator queryEvaluator(resultFetcher);

		qo.addSelectClause("while", "w");
		qo.addClause("Follows", "while", "w", "stmt", "s");

		auto l = queryEvaluator.evaluate(qo);
		Assert::IsTrue(l == std::list<std::string>{ "2"});
	}
	
	TEST_METHOD(testFollowsIfSynonymSynonym) {
		QueryObject qo;
		QueryEvaluator queryEvaluator(resultFetcher);

		qo.addSelectClause("if", "i");
		qo.addClause("Follows", "if", "i", "stmt", "s");

		auto l = queryEvaluator.evaluate(qo);
		Assert::IsTrue(l == std::list<std::string>{ "10" });
	}

	TEST_METHOD(testFollowsCallSynonymSynonym) {
		QueryObject qo;
		QueryEvaluator queryEvaluator(resultFetcher);

		qo.addSelectClause("call", "c");
		qo.addClause("Follows", "call", "c", "stmt", "s");

		auto l = queryEvaluator.evaluate(qo);
		Assert::IsTrue(l == std::list<std::string>{ "14" });
	}
	TEST_METHOD(testFollowsSynonymInteger) {
		QueryObject qo;
		QueryEvaluator queryEvaluator(resultFetcher);
		resultFetcher.followedBy = 1;

		qo.addSelectClause("assign", "a");
		qo.addClause("Follows", "assign", "a", "integer", "2");

		auto l = queryEvaluator.evaluate(qo);
		Assert::IsTrue(l == std::list<std::string>{ "1" });
	}
	TEST_METHOD(testFollowsStmtSynonymInteger) {
		QueryObject qo;
		QueryEvaluator queryEvaluator(resultFetcher);
		resultFetcher.followedBy = 1;

		qo.addSelectClause("stmt", "s");
		qo.addClause("Follows", "stmt", "s", "integer", "2");

		auto l = queryEvaluator.evaluate(qo);
		Assert::IsTrue(l == std::list<std::string>{ "1" });
	}
	TEST_METHOD(testFollowsSynonymIntegerNoResult) {
		QueryObject qo;
		QueryEvaluator queryEvaluator(resultFetcher);
		resultFetcher.followedBy = ResultFetcher::NO_RESULT;

		qo.addSelectClause("stmt", "s");
		qo.addClause("Follows", "stmt", "s", "integer", "10");

		auto l = queryEvaluator.evaluate(qo);
		Assert::IsTrue(l == std::list<std::string>{});
	}
	TEST_METHOD(testFollowsIntegerSynonym) {
		QueryObject qo;
		QueryEvaluator queryEvaluator(resultFetcher);
		resultFetcher.following = ResultFetcher::NO_RESULT;

		qo.addSelectClause("assign", "a");
		qo.addClause("Follows", "integer", "4", "assign", "a");

		auto l = queryEvaluator.evaluate(qo);
		for (auto &s : l) {
			Logger::WriteMessage(s.c_str());
		}
		Assert::IsTrue(l == std::list<std::string>{ });
		
		resultFetcher.following = 3;

		qo.clear();
		qo.addSelectClause("assign", "a");
		qo.addClause("Follows", "integer", "2", "assign", "a");

		l = queryEvaluator.evaluate(qo);
		for (auto &s : l) {
			Logger::WriteMessage(s.c_str());
		}
		Assert::IsTrue(l == std::list<std::string>{ "3" });
		
	}
	TEST_METHOD(testFollowsIntegerSynonymNoResult) {
		QueryObject qo;
		QueryEvaluator queryEvaluator(resultFetcher);
		resultFetcher.following = ResultFetcher::NO_RESULT;

		qo.addSelectClause("assign", "a");
		qo.addClause("Follows", "integer", "2", "assign", "a");

		auto l = queryEvaluator.evaluate(qo);
		Assert::IsTrue(l == std::list<std::string>{ });
	}
	TEST_METHOD(testFollowsIntegerInteger) {
		QueryObject qo;
		QueryEvaluator queryEvaluator(resultFetcher);
		resultFetcher.following = 3;
		resultFetcher.followedBy = 2;
		resultFetcher.followsPairs = { {2, 3 } };

		qo.addSelectClause("assign", "a");
		qo.addClause("Follows", "integer", "2", "integer", "3");

		std::list<std::string> l = queryEvaluator.evaluate(qo);
		for (auto &s : l) {
			Logger::WriteMessage(s.c_str());
		}
		l.sort();
		std::list<std::string> expected = { "1", "3", "11" };
		expected.sort();
		if (l.empty()) {
			Assert::IsTrue(true);
		}
		else Assert::IsTrue(l == expected);
	}
	TEST_METHOD(testFollowsIntegerIntegerNoResult) {
		QueryObject qo;
		QueryEvaluator queryEvaluator(resultFetcher);
		resultFetcher.following = ResultFetcher::NO_RESULT;

		qo.addSelectClause("assign", "a");
		qo.addClause("Follows", "integer", "6", "integer", "7");

		auto l = queryEvaluator.evaluate(qo);
		Assert::IsTrue(l == std::list<std::string>{ });
	}
	TEST_METHOD(testFollowsWildcardSynonym) {
		QueryObject qo;
		QueryEvaluator queryEvaluator(resultFetcher);

		qo.addSelectClause("stmt", "s");
		qo.addClause("Follows", "wildcard", "_", "stmt", "s");

		std::list<std::string> l = queryEvaluator.evaluate(qo);
		l.sort();
		std::list<std::string> expected = { "2", "3", "4", "11", "16" };
		expected.sort();
		Assert::IsTrue(l == expected);
	}
	TEST_METHOD(testFollowsWildcardInteger) {
		QueryObject qo;
		QueryEvaluator queryEvaluator(resultFetcher);
		resultFetcher.followedBy = 2;

		qo.addSelectClause("assign", "a");
		qo.addClause("Follows", "wildcard", "_", "integer", "3");

		auto l = queryEvaluator.evaluate(qo);
		Assert::IsTrue(l == std::list<std::string>{ "1", "3", "11" });
	}
	TEST_METHOD(testFollowsSynonymWildcard) {
		QueryObject qo;
		QueryEvaluator queryEvaluator(resultFetcher);

		qo.addSelectClause("assign", "a");
		qo.addClause("Follows", "assign", "a", "wildcard", "_");

		std::list<std::string> l = queryEvaluator.evaluate(qo);
		l.sort();
		std::list<std::string> expected = { "1", "3"};
		expected.sort();
		Assert::IsTrue(l == expected);
	}
	TEST_METHOD(testFollowsIntegerWildcard) {
		QueryObject qo;
		QueryEvaluator queryEvaluator(resultFetcher);
		resultFetcher.following = 3;

		qo.addSelectClause("assign", "a");
		qo.addClause("Follows", "integer", "2", "wildcard", "_");

		auto l = queryEvaluator.evaluate(qo);
		Assert::IsTrue(l == std::list<std::string>{ "1", "3", "11" });
	}
	TEST_METHOD(testFollowsIntegerWildcardDrillDown) {
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
		resultFetcher.allVariables = { 11, 14, 16 };
		resultFetcher.allStmts = { 1, 2, 3, 4, 5, 6 };
		QueryEvaluator queryEvaluator(resultFetcher);
		resultFetcher.following = ResultFetcher::NO_RESULT;

		QueryObject qo;
		qo.addSelectClause("variable", "v");
		qo.addClause("Follows", "integer", "2", "wildcard", "_");

		auto l = queryEvaluator.evaluate(qo);
		for (auto &s : l) {
			Logger::WriteMessage(s.c_str());
		}
		Assert::IsTrue(l == std::list<std::string>{});


		QueryObject qo1;
		qo1.addSelectClause("variable", "v");
		qo1.addClause("Follows*", "integer", "1", "wildcard", "_");
		resultFetcher.transFollowing = { 6 };

		auto l1 = queryEvaluator.evaluate(qo1);
		for (auto &s : l1) {
			Logger::WriteMessage(s.c_str());
		}
		Assert::IsTrue(l1 == std::list<std::string>{ "11", "14", "16"});

		QueryObject qo2;
		qo2.addSelectClause("variable", "v");
		qo2.addClause("Follows", "wildcard", "_", "integer", "3");
		resultFetcher.transFollowing = { };
		resultFetcher.followedBy = ResultFetcher::NO_RESULT;

		auto l2 = queryEvaluator.evaluate(qo2);
		for (auto &s : l2) {
			Logger::WriteMessage(s.c_str());
		}
		Assert::IsTrue(l2 == std::list<std::string>{ });
	}
	TEST_METHOD(testFollowsWildcardWildcard) {
		QueryObject qo;
		QueryEvaluator queryEvaluator(resultFetcher);

		qo.addSelectClause("if", "i");
		qo.addClause("Follows", "wildcard", "_", "wildcard", "_");

		auto l = queryEvaluator.evaluate(qo);
		int s1 = 1, s2 = 3, s3 = 11;
		Assert::IsTrue(l == std::list<std::string>{ "10", "12"});
	}
	TEST_METHOD(TestgetTransChildrenOf) {
		QueryEvaluator queryEvaluator(resultFetcher);
		resultFetcher.allStmts = std::unordered_set<int>{ 1, 3, 11, 17, 18, 19, 20 };
		resultFetcher.transChildrenOf = std::unordered_set<int>{ 17, 18, 19, 20 };

		QueryObject qo;
		qo.addSelectClause("stmt", "s");
		qo.addClause("Parent*", "integer", "16", "stmt", "s");

		auto l = queryEvaluator.evaluate(qo);
		for (auto &s : l) {
			Logger::WriteMessage(s.c_str());
		}
		Assert::IsTrue(l == std::list<std::string>{ "17", "18", "19", "20" });
	};

	TEST_METHOD(testParentStarSynonymInteger) {
		QueryEvaluator queryEvaluator(resultFetcher);
		resultFetcher.allStmts = { 1, 2, 3, 4, 5, 6 };
		resultFetcher.allWhiles = { 2, 4 };
		resultFetcher.allAssigns = { 3, 5, 6 };

		QueryObject qo;
		qo.addSelectClause("assign", "a");
		qo.addClause("Parent*", "assign", "a", "integer", "6");
		resultFetcher.transParentOf = { 2 , 3};

		std::list<std::string> l = queryEvaluator.evaluate(qo);
		for (auto &s : l) {
			Logger::WriteMessage(s.c_str());
		}
		l.sort();
		std::list<std::string> expected = { "3"};
		expected.sort();
		Assert::IsTrue(l == expected);

		QueryObject qo1;
		qo1.addSelectClause("stmt", "s");
		qo1.addClause("Parent*", "stmt", "s", "integer", "5");
		resultFetcher.transParentOf = { 2 , 3 , 6, 7};

		std::list<std::string> l1 = queryEvaluator.evaluate(qo1);
		for (auto &s : l1) {
			Logger::WriteMessage(s.c_str());
		}
		l1.sort();
		std::list<std::string> expected1 = { "2", "3", "6" };
		expected1.sort();
		Assert::IsTrue(l1 == expected1);

	}
	TEST_METHOD(testParentStarIntegerIntegerDrillDown) {
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
		QueryEvaluator queryEvaluator(resultFetcher);
		resultFetcher.allVariables = { 52, 43, 67 };
		resultFetcher.allStmts = { 1, 2, 3, 4, 5, 6 };
		resultFetcher.allWhiles = { 2, 4 };
		resultFetcher.allAssigns = { 3, 5, 6 };

		//pkb->setFollows(1, 6);

		//pkb->setParent(1, 2);
		//pkb->setParent(2, 3);
		//pkb->setParent(1, 4);
		//pkb->setParent(4, 5);

		QueryObject qo;
		qo.addSelectClause("variable", "v");
		qo.addClause("Parent*", "integer", "2", "integer", "3");
		resultFetcher.transParentPairs = { { 2, 3 } };

		std::list<std::string> l = queryEvaluator.evaluate(qo);
		for (auto &s : l) {
			Logger::WriteMessage(s.c_str());
		}
		l.sort();
		std::list<std::string> expected = { "43", "52", "67" };
		expected.sort();
		Assert::IsTrue(l == expected);
	}
	TEST_METHOD(testTwoVariable) {

		QueryObject qo;
		QueryEvaluator queryEvaluator(resultFetcher);

		qo.addSelectClause("assign", "a");
		qo.addClause("Modifies", "assign", "a", "variable", "v");
		qo.addPatternClause("Assign", "assign", "a", "variable", "v", "subexpression", "lost");
		resultFetcher.allStmts = std::unordered_set<int>{ 1, 3, 11, 17, 18, 19, 20 };
		resultFetcher.allAssigns = std::unordered_set<int>{ 1, 3, 11, 17, 18, 19, 20 };
		resultFetcher.allVariables = { 11, 14, 16 };
		resultFetcher.modifiesStmtPairs = {
			{1, 11},
			{3, 14},
			{11, 11}
		};
		resultFetcher.patternWhitelist = { 3, 11};

		auto l = queryEvaluator.evaluate(qo);
		for (auto &s : l) {
			Logger::WriteMessage(s.c_str());
		}
		Assert::IsTrue(l == std::list<std::string>{"3", "11"});
	}
private:
	ResultFetcherStub resultFetcher;
	};

	TEST_CLASS(TestQueryEvaluatorSelect) {
	public:
		TEST_METHOD_INITIALIZE(constructResultsFetcher) {
			resultFetcher.allStmts = { 1, 3, 7, 11 };
			resultFetcher.allAssigns = { 1, 3, 11 };
			resultFetcher.allProcedures = { 4 };
			resultFetcher.allVariables = { 2, 6, 9 };
		}

		TEST_METHOD(testSelectBooleanNoClause) {
			QueryObject qo;
			QueryEvaluator queryEvaluator(resultFetcher);
			qo.addSelectClause("boolean", "");
			std:list<std::string> l = queryEvaluator.evaluate(qo);

			Assert::IsTrue(l == std::list<std::string>{"TRUE"});
		}
		TEST_METHOD(testSelectBooleanNoResults) {
			QueryObject qo;
			QueryEvaluator queryEvaluator(resultFetcher);
			qo.addSelectClause("boolean", "");
			qo.addClause("Modifies", "assign", "a", "variable", "v");
			std:list<std::string> l = queryEvaluator.evaluate(qo);

			Assert::IsTrue(l == std::list<std::string>{"FALSE"});
		}

		TEST_METHOD(testSelectBooleanWithResults) {
			resultFetcher.modifiesStmtPairs = {
				{ 1, 2 },
				{ 3, 2 },
				{ 11, 6 }
			};

			QueryObject qo;
			QueryEvaluator queryEvaluator(resultFetcher);
			qo.addSelectClause("boolean", "");
			qo.addClause("Modifies", "assign", "a", "variable", "v");
			std:list<std::string> l = queryEvaluator.evaluate(qo);

			Assert::IsTrue(l == std::list<std::string>{"TRUE"});
		}

		TEST_METHOD(testSelectBooleanNoResultsAfterMerge) {
			resultFetcher.modifiesStmtPairs = {
				{ 1, 2 },
			};
			resultFetcher.followsPairs = {
				{ 3, 12 },
			};

			QueryObject qo;
			QueryEvaluator queryEvaluator(resultFetcher);
			qo.addSelectClause("boolean", "");
			qo.addClause("Modifies", "assign", "a", "variable", "v");
			qo.addClause("Follows", "assign", "a", "stmt", "s");

		std:list<std::string> l = queryEvaluator.evaluate(qo);

			Assert::IsTrue(l == std::list<std::string>{"FALSE"});
		}

	private:
		ResultFetcherStub resultFetcher;
	};

};
