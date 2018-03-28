#include "stdafx.h"
#include "CppUnitTest.h"
#include "pkb.h"
#include "SourceParser.h"
#include "QueryEvaluator.h"
#include "QueryObject.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting
{
	TEST_CLASS(UnitTest1)
	{
	public:

		// test whether follows* relationship is stored correctly in pkb
		TEST_METHOD(TestParentStarRelations) {
			SourceCode source = R"(
				procedure One {
					while z {
						while y {
							ddd = 0*9; }     }
				}
            )";
			PKB::clearPKB();
			SourceParser::clear();
			PKB* pkb = PKB::getPKBInstance();
			SourceParser::getInstance(*pkb)->parse(source);

			std::vector<int> expected = {2, 1};
			auto l = pkb->getTransParentOf(3);
			for (auto &s : l) {
				Logger::WriteMessage(std::to_string(s).c_str());
			}
			Assert::IsTrue(expected == pkb->getTransParentOf(3));

			PKB::clearPKB();
			SourceParser::clear();
		}

		TEST_METHOD(TestSourceAwithQEV) {
			SourceCode source = R"(
               procedure One {
 	              x = 123;
 	              y = 0;
	              if i then {
		              i = x; }
	              else {
		              i = y; 			}
	             ddd = x + y * 123;
	             z = x * y + ddd + 321;
	            while w {
		            w = w + z;      }
	            x = z;
 	            y = ddd     +     2;
	           while x {
		           if y then {
			           z = 75 + x;
		           }
		           else {
			           z = 75 - x; }
	           }
	           if ddd then {
		           while y {
			           ddd = 0*9; }      }
	           else {
		           while y {
			           ddd = z*x; }
			   }
            }
            )";
			PKB* pkb = PKB::getPKBInstance();
			SourceParser::getInstance(*pkb)->parse(source);
			
			QueryObject qo;
			qo.addSelectClause("stmt", "s");
			qo.addClause("Follows", "stmt", "s", "integer", "2");

			QueryEvaluator queryEvaluator(*pkb);
			auto l = queryEvaluator.evaluate(qo);
			for (auto &s : l) {
				Logger::WriteMessage(s.c_str());
			}
			Assert::IsTrue(l == std::list<std::string>{ "1" });

			//variable v; Select v such that Parent*(17, 19)
			QueryObject qo1;
			qo1.addSelectClause("variable", "v");
			qo1.addClause("Parent*", "integer", "17", "integer", "19");

			auto l1 = queryEvaluator.evaluate(qo1);
			for (auto &s : l1) {
				Logger::WriteMessage(s.c_str());
			}
			Assert::IsTrue(l1 == std::list<std::string>{ });

			PKB::clearPKB();
			SourceParser::clear();
		}

	};
}
