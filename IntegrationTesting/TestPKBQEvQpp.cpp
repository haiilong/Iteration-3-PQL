#include "stdafx.h"

#include "PKB.h"
#include "QueryEvaluator.h"
#include "QueryObject.h"
#include "QueryPreProcessor.h"
#include "DesignExtractor.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestPKBQueryEvaluatorQueryPreprocessor) {
public:
	TEST_METHOD(TestgetTransParentOf) {
		PKB* pkb = PKB::getPKBInstance();
		pkb->insertStmt(16, "WHILE");
		pkb->insertStmt(17, "ASSIGN");
		pkb->insertStmt(18, "ASSIGN");
		pkb->insertStmt(19, "ASSIGN");
		pkb->insertStmt(20, "ASSIGN");
		pkb->setParent(16, 17);
		pkb->setParent(16, 19);
		pkb->setParent(17, 18);
		pkb->setParent(19, 20);
		DesignExtractor::populateParentStarTable();

		QueryPreProcessor qpp;
		//qpp.clear();
		qpp.validate("stmt s; Select s such that Parent*(s,18)");
		//Assert::IsTrue(qpp.getQueryObject().getQClauses()[0].getQClauseType() == QClause::PARENT_T);

		QueryEvaluator queryEvaluator(*pkb);

		auto l = queryEvaluator.evaluate(qpp.getQueryObject());
		Assert::IsTrue(l == std::list<std::string>{ "16", "17" });
	};
private:
	};
};
