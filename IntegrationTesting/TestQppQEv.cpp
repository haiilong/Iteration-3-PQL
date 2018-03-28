#include "stdafx.h"

#include "ResultFetcherStub.h"
#include "QueryEvaluator.h"
#include "QueryObject.h"
#include "QueryPreProcessor.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestQueryPreprocessorQueryEvaluator) {
public:
	TEST_METHOD_INITIALIZE(constructResultsFetcher) {
		resultFetcher.allStmts = { 1, 2, 3, 4, 10, 11 };
		resultFetcher.allAssigns = { 1, 3, 11 };
		resultFetcher.followsPairs = {
			{ 1, 2 },
			{ 2, 3 },
			{ 3, 4 },
			{ 10, 11 }
		};
	}
	TEST_METHOD(TestgetTransParentOf) {
		QueryPreProcessor qpp;
		qpp.clear();
		qpp.validate("stmt s; Select s such that Parent*(s,18)");
		Assert::IsTrue(qpp.getQueryObject().getQClauses()[0].getQClauseType() == QClause::PARENT_T);

		QueryEvaluator queryEvaluator(resultFetcher);
		resultFetcher.transParentOf = std::vector<Result>{ 16, 17 };

		auto l = queryEvaluator.evaluate(qpp.getQueryObject());
		Assert::IsTrue(l == std::list<std::string>{ "16", "17" });
	};
private:
	ResultFetcherStub resultFetcher;
	};
};
