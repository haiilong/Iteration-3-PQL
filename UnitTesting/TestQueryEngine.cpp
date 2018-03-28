#include "stdafx.h"
#include "ResultsFetcherStub.h"
#include "QueryEngine.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestQueryEvaluator) {
public:
	TEST_METHOD_INITIALIZE(constructResultsFetcher) {
		resultsFetcher.setAssigns(std::list<int>{1, 3, 11});
		resultsFetcher.setProcs(std::list<std::string>{"Proc1", "Proc5", "Proc9"});
		resultsFetcher.setStmts(std::list<int>{1, 7, 3, 11});
		resultsFetcher.setVars(std::list<std::string>{"v", "x", "z"});
	}
	TEST_METHOD(TestUnqualifiedAssignQuery) {
		auto message = L"unqualified assignment synonym query should return '1, 3, 11'";
		QueryEngine queryEngine(resultsFetcher);
		auto l = queryEngine.getResults("assign a; Select a");
		std::string s1 = "1", s2 = "3", s3 = "11";
		Assert::IsTrue(l == std::list<std::string>{s1, s2, s3}, message);
	}
	TEST_METHOD(TestUnqualifiedStmtQuery) {
		auto message = L"unqualified statement synonym query should return '1, 3, 7, 11'";
		QueryEngine queryEngine(resultsFetcher);
		auto l = queryEngine.getResults("stmt s; Select s");
		std::string s1 = "1", s2 = "3", s3 = "7", s4 = "11";
		Assert::IsTrue(l == std::list<std::string>{s1, s2, s3, s4}, message);
	}
	TEST_METHOD(TestUnqualifiedVariableQuery) {
		auto message = L"unqualified variable synonym query should return 'v, x, y'";
		QueryEngine queryEngine(resultsFetcher);
		auto l = queryEngine.getResults("variable v; Select v");
		std::string s1 = "v", s2 = "x", s3 = "z";
		Assert::IsTrue(l == std::list<std::string>{s1, s2, s3}, message);
	}
	TEST_METHOD(TestLeftoverState) {
		auto message = L"unqualified statement synonym query should return '1, 3, 7, 11'";
		QueryEngine queryEngine(resultsFetcher);
		auto l = queryEngine.getResults("stmt s; Select s");
		std::string s1 = "1", s2 = "3", s3 = "7", s4 = "11";
		Assert::IsTrue(l == std::list<std::string>{s1, s2, s3, s4}, message);
		message = L"unqualified variable synonym query should return 'v, x, y'";
		l = queryEngine.getResults("variable v; Select v");
		s1 = "v", s2 = "x", s3 = "z";
		Assert::IsTrue(l == std::list<std::string>{s1, s2, s3}, message);
	}
private:
	ResultsFetcherStub resultsFetcher;
	};
};
