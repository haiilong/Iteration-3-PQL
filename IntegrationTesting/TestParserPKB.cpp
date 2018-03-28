#include "stdafx.h"
#include "CppUnitTest.h"
#include "pkb.h"
#include "SourceParser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting
{		
	TEST_CLASS(UnitTest1)
	{
	public:
        bool contains(std::vector<std::pair<int, int>> allPairs, std::pair<int, int> element) {
            for(std::pair<int, int> p : allPairs) {
                if(p == element) return true;
            }
            return false;
        }

        TEST_METHOD(TestParsesSourceWithSimpleAssignVariable) {
            PKB::clearPKB();
            SourceParser::clear();

            SourceCode source = string("procedure First { x = y; }");
            PKB* pkb = PKB::getPKBInstance();
            SourceParser::getInstance(*pkb)->parse(source);
            
            Assert::AreEqual(string("First"), pkb->procTable.getProcName(1));
            Assert::IsTrue(pkb->procTable.getSize() == 1);

            Assert::AreEqual(string("ASSIGN"), pkb->stmtTable.getStmtType(1));
            Assert::IsTrue(pkb->stmtTable.getSize() == 1);

            Assert::AreEqual(string("x"), pkb->varTable.getVarName(1));
            Assert::AreEqual(string("y"), pkb->varTable.getVarName(2));
            Assert::IsTrue(pkb->varTable.getSize() == 2);

            Assert::AreEqual(string(" y "), pkb->patternTable.getExpression(1));

            // check modifies
            std::vector<std::pair<int, int>> modifiesPairs = pkb->modifiesTable.getModifiesPairs();
            Assert::IsTrue(contains(modifiesPairs, std::pair<int, int>(1, 1)));
            Assert::IsTrue(modifiesPairs.size() == 1);

            // check uses
            std::vector<std::pair<int, int>> usesPairs = pkb->usesTable.getUsesPairs();
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(1, 2)));
            Assert::IsTrue(modifiesPairs.size() == 1);

            // check proc modifies
            std::vector<std::pair<int, int>> actualProcModifiesPairs = pkb->modifiesTable.getProcModifiesPairs();
            std::vector<std::pair<int, int>> expectedProModifiesPairs = {
                std::pair<int, int>(1, 1),
            };
            for(std::pair<int, int> p : expectedProModifiesPairs) {
                Assert::IsTrue(contains(actualProcModifiesPairs, p));
            }
            Assert::IsTrue(actualProcModifiesPairs.size() == expectedProModifiesPairs.size());

            // check proc uses
            std::vector<std::pair<int, int>> actualProcUsesPairs = pkb->usesTable.getProcUsesPairs();
            std::vector<std::pair<int, int>> expectedProcUsesPairs = {
                std::pair<int, int>(1, 2),
            };
            for(std::pair<int, int> p : expectedProcUsesPairs) {
                Assert::IsTrue(contains(actualProcUsesPairs, p));
            }
            Assert::IsTrue(actualProcUsesPairs.size() == expectedProcUsesPairs.size());

            PKB::clearPKB();
            SourceParser::clear();
        }

        TEST_METHOD(TestParsesSourceWithTwoSimpleProcedures) {
            PKB::clearPKB();
            SourceParser::clear();

            SourceCode source = R"(
                procedure First { 
                    x = y;
                }
                procedure Second {
                    x=y;
                }   
            )";

            PKB* pkb = PKB::getPKBInstance();
            SourceParser::getInstance(*pkb)->parse(source);

            Assert::AreEqual(string("First"), pkb->procTable.getProcName(1));
            Assert::AreEqual(string("Second"), pkb->procTable.getProcName(2));
            Assert::IsTrue(pkb->procTable.getSize() == 2);

            Assert::AreEqual(string("ASSIGN"), pkb->stmtTable.getStmtType(1));
            Assert::AreEqual(string("ASSIGN"), pkb->stmtTable.getStmtType(2));
            Assert::IsTrue(pkb->stmtTable.getSize() == 2);

            Assert::AreEqual(string("x"), pkb->varTable.getVarName(1));
            Assert::AreEqual(string("y"), pkb->varTable.getVarName(2));
            Assert::IsTrue(pkb->varTable.getSize() == 2);

            Assert::AreEqual(string(" y "), pkb->patternTable.getExpression(1));
            Assert::AreEqual(string(" y "), pkb->patternTable.getExpression(2));

            // check modifies
            std::vector<std::pair<int, int>> modifiesPairs = pkb->modifiesTable.getModifiesPairs();
            Assert::IsTrue(contains(modifiesPairs, std::pair<int, int>(1, 1)));
            Assert::IsTrue(contains(modifiesPairs, std::pair<int, int>(2, 1)));
            Assert::IsTrue(modifiesPairs.size() == 2);

            // check uses
            std::vector<std::pair<int, int>> usesPairs = pkb->usesTable.getUsesPairs();
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(1, 2)));
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(2, 2)));
            Assert::IsTrue(modifiesPairs.size() == 2);

            // check proc modifies
            std::vector<std::pair<int, int>> actualProcModifiesPairs = pkb->modifiesTable.getProcModifiesPairs();
            std::vector<std::pair<int, int>> expectedProModifiesPairs = {
                std::pair<int, int>(1, 1),
                std::pair<int, int>(2, 1),
            };
            for(std::pair<int, int> p : expectedProModifiesPairs) {
                Assert::IsTrue(contains(actualProcModifiesPairs, p));
            }
            Assert::IsTrue(actualProcModifiesPairs.size() == expectedProModifiesPairs.size());

            // check proc uses
            std::vector<std::pair<int, int>> actualProcUsesPairs = pkb->usesTable.getProcUsesPairs();
            std::vector<std::pair<int, int>> expectedProcUsesPairs = {
                std::pair<int, int>(1, 2),
                std::pair<int, int>(2, 2),
            };
            for(std::pair<int, int> p : expectedProcUsesPairs) {
                Assert::IsTrue(contains(actualProcUsesPairs, p));
            }
            Assert::IsTrue(actualProcUsesPairs.size() == expectedProcUsesPairs.size());

            PKB::clearPKB();
            SourceParser::clear();
        }

        TEST_METHOD(TestParsesSourceWithSimpleCall) {
            PKB::clearPKB();
            SourceParser::clear();

            SourceCode source = string("procedure First { call Second; }");
            PKB* pkb = PKB::getPKBInstance();
            SourceParser::getInstance(*pkb)->parse(source);

            Assert::AreEqual(string("First"), pkb->procTable.getProcName(1));
            Assert::AreEqual(string("Second"), pkb->procTable.getProcName(2));
            Assert::IsTrue(pkb->procTable.getSize() == 2);

            Assert::AreEqual(string("CALL"), pkb->stmtTable.getStmtType(1));
            Assert::IsTrue(pkb->stmtTable.getSize() == 1);

            // check call 
            std::vector<std::pair<int, int>> actualCallPairs = pkb->callsTable.getCallsPairs();
            std::vector<std::pair<int, int>> expectedCallPairs = {
                std::pair<int, int>(1, 2),
            };
            for(std::pair<int, int> p : expectedCallPairs) {
                Assert::IsTrue(contains(actualCallPairs, p));
            }
            Assert::IsTrue(actualCallPairs.size() == expectedCallPairs.size());

            // var table
            Assert::IsTrue(pkb->varTable.getSize() == 0);

            // check modifies
            Assert::IsTrue(pkb->modifiesTable.getModifiesPairs().size() == 0);

            // check uses
            Assert::IsTrue(pkb->usesTable.getUsesPairs().size() == 0);

            PKB::clearPKB();
            SourceParser::clear();
        }

        TEST_METHOD(TestParsesSourceWithSimpleAssignConstant) {
            PKB::clearPKB();
            SourceParser::clear();

            SourceCode source = string("procedure First { x = 1; }");
            PKB* pkb = PKB::getPKBInstance();
            SourceParser::getInstance(*pkb)->parse(source);

            Assert::AreEqual(string("First"), pkb->procTable.getProcName(1));
            Assert::AreEqual(string("ASSIGN"), pkb->stmtTable.getStmtType(1));
            Assert::AreEqual(string("x"), pkb->varTable.getVarName(1));
            Assert::AreEqual(string(" 1 "), pkb->patternTable.getExpression(1));

            Assert::IsTrue(pkb->procTable.getSize() == 1);
            Assert::IsTrue(pkb->stmtTable.getSize() == 1);
            Assert::IsTrue(pkb->varTable.getSize() == 1);
            Assert::IsTrue(pkb->constTable.getSize() == 1);

            // check modifies
            std::vector<std::pair<int, int>> modifiesPairs = pkb->modifiesTable.getModifiesPairs();
            Assert::IsTrue(contains(modifiesPairs, std::pair<int, int>(1, 1)));
            Assert::IsTrue(modifiesPairs.size() == 1);

            // check uses
            std::vector<std::pair<int, int>> usesPairs = pkb->usesTable.getUsesPairs();
            Assert::IsTrue(usesPairs.size() == 0);

            // check proc modifies
            std::vector<std::pair<int, int>> actualProcModifiesPairs = pkb->modifiesTable.getProcModifiesPairs();
            std::vector<std::pair<int, int>> expectedProModifiesPairs = {
                std::pair<int, int>(1, 1),
            };
            for(std::pair<int, int> p : expectedProModifiesPairs) {
                Assert::IsTrue(contains(actualProcModifiesPairs, p));
            }
            Assert::IsTrue(actualProcModifiesPairs.size() == expectedProModifiesPairs.size());

            // check proc uses
            std::vector<std::pair<int, int>> actualProcUsesPairs = pkb->usesTable.getProcUsesPairs();
            std::vector<std::pair<int, int>> expectedProcUsesPairs = {
            };
            for(std::pair<int, int> p : expectedProcUsesPairs) {
                Assert::IsTrue(contains(actualProcUsesPairs, p));
            }
            Assert::IsTrue(actualProcUsesPairs.size() == expectedProcUsesPairs.size());

            PKB::clearPKB();
            SourceParser::clear();
        }

        TEST_METHOD(TestParsesSourceWithSimpleIfStatement) {
            PKB::clearPKB();
            SourceParser::clear();

            SourceCode source = R"(
                procedure First { 
                    x = c;
                    if x then {
                        z = b;
                    } else {
                        y = a;
                    } 
                }
            )"; 
            PKB* pkb = PKB::getPKBInstance();
            SourceParser::getInstance(*pkb)->parse(source);

            Assert::AreEqual(string("First"), pkb->procTable.getProcName(1));
            Assert::AreEqual(string("ASSIGN"), pkb->stmtTable.getStmtType(1));
            Assert::AreEqual(string("IF"), pkb->stmtTable.getStmtType(2));
            Assert::AreEqual(string("ASSIGN"), pkb->stmtTable.getStmtType(3));
            Assert::AreEqual(string("ASSIGN"), pkb->stmtTable.getStmtType(4));
            Assert::AreEqual(string("x"), pkb->varTable.getVarName(1));
            Assert::AreEqual(string("c"), pkb->varTable.getVarName(2));
            Assert::AreEqual(string("z"), pkb->varTable.getVarName(3));
            Assert::AreEqual(string("b"), pkb->varTable.getVarName(4));
            Assert::AreEqual(string("y"), pkb->varTable.getVarName(5));
            Assert::AreEqual(string("a"), pkb->varTable.getVarName(6));
            Assert::AreEqual(string(" c "), pkb->patternTable.getExpression(1));
            Assert::AreEqual(string(" b "), pkb->patternTable.getExpression(3));
            Assert::AreEqual(string(" a "), pkb->patternTable.getExpression(4));

            // check control variable
            vector<pair<int, int>> actualControlPairs = pkb->stmtTable.getStmtControlVarPairs();
            vector<pair<int, int>> expectedControlPairs = {
                pair<int, int>(2, 1),
            };
            for(pair<int, int> p : expectedControlPairs) {
                Assert::IsTrue(contains(actualControlPairs, p));
            }
            Assert::IsTrue(actualControlPairs.size() == expectedControlPairs.size());

            // check parents
            Assert::AreEqual(2, pkb->parentTable.getParent(3));
            Assert::AreEqual(2, pkb->parentTable.getParent(4));
            std::vector<std::pair<int, int>> actualTransParentsPairs = pkb->parentStarTable.getParentStarPairs();
            std::vector<std::pair<int, int>> expectedTransParentsPairs = {
                std::pair<int, int>(2, 3),
                std::pair<int, int>(2, 4),
            };
            Assert::IsTrue(actualTransParentsPairs == expectedTransParentsPairs);

            // check follows
            Assert::AreEqual(1, pkb->followsTable.getFollowedBy(2));
            std::vector<std::pair<int, int>> actualFollowsPairs = pkb->followsStarTable.getFollowsStarPairs();
            std::vector<std::pair<int, int>> expectedTransFollowsPairs = {
                std::pair<int, int>(1, 2),
            };
            Assert::IsTrue(actualFollowsPairs == expectedTransFollowsPairs);

            Assert::IsTrue(pkb->procTable.getSize() == 1);
            Assert::IsTrue(pkb->stmtTable.getSize() == 4);
            Assert::IsTrue(pkb->varTable.getSize() == 6);
            Assert::IsTrue(pkb->constTable.getSize() == 0);
            Assert::IsTrue(pkb->parentTable.getParentPairs().size() == 2);
            Assert::IsTrue(pkb->followsTable.getFollowsPairs().size() == 1);

            // check modifies
            std::vector<std::pair<int, int>> modifiesPairs = pkb->modifiesTable.getModifiesPairs();
            Assert::IsTrue(contains(modifiesPairs, std::pair<int, int>(1, 1)));
            Assert::IsTrue(contains(modifiesPairs, std::pair<int, int>(2, 3)));
            Assert::IsTrue(contains(modifiesPairs, std::pair<int, int>(2, 5)));
            Assert::IsTrue(contains(modifiesPairs, std::pair<int, int>(3, 3)));
            Assert::IsTrue(contains(modifiesPairs, std::pair<int, int>(4, 5)));
            Assert::IsTrue(modifiesPairs.size() == 5);

            // check uses
            std::vector<std::pair<int, int>> usesPairs = pkb->usesTable.getUsesPairs();
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(1, 2)));
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(2, 1)));
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(2, 4)));
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(2, 6)));
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(3, 4)));
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(4, 6)));
            Assert::IsTrue(usesPairs.size() == 6);

            // check proc modifies
            std::vector<std::pair<int, int>> actualProcModifiesPairs = pkb->modifiesTable.getProcModifiesPairs();
            std::vector<std::pair<int, int>> expectedProModifiesPairs = {
                std::pair<int, int>(1, 1),
                std::pair<int, int>(1, 3),
                std::pair<int, int>(1, 5),
            };
            for(std::pair<int, int> p : expectedProModifiesPairs) {
                Assert::IsTrue(contains(actualProcModifiesPairs, p));
            }
            Assert::IsTrue(actualProcModifiesPairs.size() == expectedProModifiesPairs.size());

            // check proc uses
            std::vector<std::pair<int, int>> actualProcUsesPairs = pkb->usesTable.getProcUsesPairs();
            std::vector<std::pair<int, int>> expectedProcUsesPairs = {
                std::pair<int, int>(1, 1),
                std::pair<int, int>(1, 2),
                std::pair<int, int>(1, 4),
                std::pair<int, int>(1, 6),
            };
            for(std::pair<int, int> p : expectedProcUsesPairs) {
                Assert::IsTrue(contains(actualProcUsesPairs, p));
            }
            Assert::IsTrue(actualProcUsesPairs.size() == expectedProcUsesPairs.size());

            PKB::clearPKB();
            SourceParser::clear();
        }

        TEST_METHOD(TestParsesSourceWithSimpleWhileStatement) {
            PKB::clearPKB();
            SourceParser::clear();

            SourceCode source = R"(
                procedure First { 
                    c = c;
                    while x {
                        z = b;
                        a = y;
                    } 
                }
            )";

            PKB* pkb = PKB::getPKBInstance();
            SourceParser::getInstance(*pkb)->parse(source);

            Assert::AreEqual(string("First"), pkb->procTable.getProcName(1));
            Assert::AreEqual(string("ASSIGN"), pkb->stmtTable.getStmtType(1));
            Assert::AreEqual(string("WHILE"), pkb->stmtTable.getStmtType(2));
            Assert::AreEqual(string("ASSIGN"), pkb->stmtTable.getStmtType(3));
            Assert::AreEqual(string("ASSIGN"), pkb->stmtTable.getStmtType(4));
            Assert::AreEqual(string("c"), pkb->varTable.getVarName(1));
            Assert::AreEqual(string("x"), pkb->varTable.getVarName(2));
            Assert::AreEqual(string("z"), pkb->varTable.getVarName(3));
            Assert::AreEqual(string("b"), pkb->varTable.getVarName(4));
            Assert::AreEqual(string("a"), pkb->varTable.getVarName(5));
            Assert::AreEqual(string("y"), pkb->varTable.getVarName(6));
            Assert::AreEqual(string(" c "), pkb->patternTable.getExpression(1));
            Assert::AreEqual(string(" b "), pkb->patternTable.getExpression(3));
            Assert::AreEqual(string(" y "), pkb->patternTable.getExpression(4));

            // check control variable
            vector<pair<int, int>> actualControlPairs = pkb->stmtTable.getStmtControlVarPairs();
            vector<pair<int, int>> expectedControlPairs = {
                pair<int, int>(2, 2),
            };
            for(pair<int, int> p : expectedControlPairs) {
                Assert::IsTrue(contains(actualControlPairs, p));
            }
            Assert::IsTrue(actualControlPairs.size() == expectedControlPairs.size());

            // check parents
            Assert::AreEqual(2, pkb->parentTable.getParent(3));
            Assert::AreEqual(2, pkb->parentTable.getParent(4));
            std::vector<std::pair<int, int>> actualTransParentsPairs = pkb->parentStarTable.getParentStarPairs();
            std::vector<std::pair<int, int>> expectedTransParentsPairs = {
                std::pair<int, int>(2, 3),
                std::pair<int, int>(2, 4),
            };
            for(std::pair<int, int> pair : expectedTransParentsPairs) {
                Assert::IsTrue(contains(actualTransParentsPairs, pair));
            }
            Assert::AreEqual(actualTransParentsPairs.size(), expectedTransParentsPairs.size());

            // check follows
            Assert::AreEqual(1, pkb->followsTable.getFollowedBy(2));
            Assert::AreEqual(3, pkb->followsTable.getFollowedBy(4));
            std::vector<std::pair<int, int>> actualFollowsPairs = pkb->followsStarTable.getFollowsStarPairs();
            std::vector<std::pair<int, int>> expectedTransFollowsPairs = {
                std::pair<int, int>(1, 2),
                std::pair<int, int>(3, 4),
            };
            for(std::pair<int, int> pair : expectedTransFollowsPairs) {
                Assert::IsTrue(contains(actualFollowsPairs, pair));
            }
            Assert::AreEqual(actualFollowsPairs.size(), expectedTransFollowsPairs.size());


            Assert::IsTrue(pkb->procTable.getSize() == 1);
            Assert::IsTrue(pkb->stmtTable.getSize() == 4);
            Assert::IsTrue(pkb->varTable.getSize() == 6);
            Assert::IsTrue(pkb->constTable.getSize() == 0);
            Assert::IsTrue(pkb->parentTable.getParentPairs().size() == 2);
            Assert::IsTrue(pkb->followsTable.getFollowsPairs().size() == 2);

            // check modifies
            std::vector<std::pair<int, int>> modifiesPairs = pkb->modifiesTable.getModifiesPairs();
            Assert::IsTrue(contains(modifiesPairs, std::pair<int, int>(1, 1)));
            Assert::IsTrue(contains(modifiesPairs, std::pair<int, int>(2, 3)));
            Assert::IsTrue(contains(modifiesPairs, std::pair<int, int>(2, 5)));
            Assert::IsTrue(contains(modifiesPairs, std::pair<int, int>(3, 3)));
            Assert::IsTrue(contains(modifiesPairs, std::pair<int, int>(4, 5)));
            Assert::IsTrue(modifiesPairs.size() == 5);

            // check uses
            std::vector<std::pair<int, int>> usesPairs = pkb->usesTable.getUsesPairs();
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(1, 1)));
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(2, 2)));
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(2, 4)));
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(2, 6)));
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(3, 4)));
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(4, 6)));
            Assert::IsTrue(usesPairs.size() == 6);

            // check proc modifies
            std::vector<std::pair<int, int>> actualProcModifiesPairs = pkb->modifiesTable.getProcModifiesPairs();
            std::vector<std::pair<int, int>> expectedProModifiesPairs = {
                std::pair<int, int>(1, 1),
                std::pair<int, int>(1, 3),
                std::pair<int, int>(1, 5),
            };
            for(std::pair<int, int> p : expectedProModifiesPairs) {
                Assert::IsTrue(contains(actualProcModifiesPairs, p));
            }
            Assert::IsTrue(actualProcModifiesPairs.size() == expectedProModifiesPairs.size());

            // check proc uses
            std::vector<std::pair<int, int>> actualProcUsesPairs = pkb->usesTable.getProcUsesPairs();
            std::vector<std::pair<int, int>> expectedProcUsesPairs = {
                std::pair<int, int>(1, 1),
                std::pair<int, int>(1, 2),
                std::pair<int, int>(1, 4),
                std::pair<int, int>(1, 6),
            };
            for(std::pair<int, int> p : expectedProcUsesPairs) {
                Assert::IsTrue(contains(actualProcUsesPairs, p));
            }
            Assert::IsTrue(actualProcUsesPairs.size() == expectedProcUsesPairs.size());

            PKB::clearPKB();
            SourceParser::clear();
        }

        TEST_METHOD(TestParsesSourceWithSimplePattern) {
            PKB::clearPKB();
            SourceParser::clear();

            SourceCode source = R"(
                procedure First { 
                    x = a + b * c; 
                }
            )";
            PKB* pkb = PKB::getPKBInstance();
            SourceParser::getInstance(*pkb)->parse(source);

            Assert::AreEqual(string("First"), pkb->procTable.getProcName(1));
            Assert::AreEqual(string("ASSIGN"), pkb->stmtTable.getStmtType(1));
            Assert::AreEqual(string("x"), pkb->varTable.getVarName(1));
            Assert::AreEqual(string("a"), pkb->varTable.getVarName(2));
            Assert::AreEqual(string("b"), pkb->varTable.getVarName(3));
            Assert::AreEqual(string("c"), pkb->varTable.getVarName(4));
            Assert::AreEqual(string(" a b c * + "), pkb->patternTable.getExpression(1));

            Assert::IsTrue(pkb->procTable.getSize() == 1);
            Assert::IsTrue(pkb->stmtTable.getSize() == 1);
            Assert::IsTrue(pkb->varTable.getSize() == 4);
            Assert::IsTrue(pkb->constTable.getSize() == 0);
            Assert::IsTrue(pkb->parentTable.getParentPairs().size() == 0);
            Assert::IsTrue(pkb->followsTable.getFollowsPairs().size() == 0);

            // check modifies
            std::vector<std::pair<int, int>> modifiesPairs = pkb->modifiesTable.getModifiesPairs();
            Assert::IsTrue(contains(modifiesPairs, std::pair<int, int>(1, 1)));
            Assert::IsTrue(modifiesPairs.size() == 1);

            // check uses
            std::vector<std::pair<int, int>> usesPairs = pkb->usesTable.getUsesPairs();
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(1, 2)));
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(1, 3)));
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(1, 4)));
            Assert::IsTrue(usesPairs.size() == 3);

            // check proc modifies
            std::vector<std::pair<int, int>> actualProcModifiesPairs = pkb->modifiesTable.getProcModifiesPairs();
            std::vector<std::pair<int, int>> expectedProModifiesPairs = {
                std::pair<int, int>(1, 1),
            };
            for(std::pair<int, int> p : expectedProModifiesPairs) {
                Assert::IsTrue(contains(actualProcModifiesPairs, p));
            }
            Assert::IsTrue(actualProcModifiesPairs.size() == expectedProModifiesPairs.size());

            // check proc uses
            std::vector<std::pair<int, int>> actualProcUsesPairs = pkb->usesTable.getProcUsesPairs();
            std::vector<std::pair<int, int>> expectedProcUsesPairs = {
                std::pair<int, int>(1, 2),
                std::pair<int, int>(1, 3),
                std::pair<int, int>(1, 4),
            };
            for(std::pair<int, int> p : expectedProcUsesPairs) {
                Assert::IsTrue(contains(actualProcUsesPairs, p));
            }
            Assert::IsTrue(actualProcUsesPairs.size() == expectedProcUsesPairs.size());

            PKB::clearPKB();
            SourceParser::clear();
        }

        TEST_METHOD(TestParsesSourceWithNestedIf) {
            PKB::clearPKB();
            SourceParser::clear();

            SourceCode source = R"(
                procedure First { 
                    x = c;
                    while i {
                        if x then {
                            z = b;
                        } else {
                            y = a;
                        }
                    }
                }
            )";
            PKB* pkb = PKB::getPKBInstance();
            SourceParser::getInstance(*pkb)->parse(source);

            Assert::AreEqual(string("First"), pkb->procTable.getProcName(1));
            Assert::AreEqual(string("ASSIGN"), pkb->stmtTable.getStmtType(1));
            Assert::AreEqual(string("WHILE"), pkb->stmtTable.getStmtType(2));
            Assert::AreEqual(string("IF"), pkb->stmtTable.getStmtType(3));
            Assert::AreEqual(string("ASSIGN"), pkb->stmtTable.getStmtType(4));
            Assert::AreEqual(string("ASSIGN"), pkb->stmtTable.getStmtType(5));
            Assert::AreEqual(string("x"), pkb->varTable.getVarName(1));
            Assert::AreEqual(string("c"), pkb->varTable.getVarName(2));
            Assert::AreEqual(string("i"), pkb->varTable.getVarName(3));
            Assert::AreEqual(string("z"), pkb->varTable.getVarName(4));
            Assert::AreEqual(string("b"), pkb->varTable.getVarName(5));
            Assert::AreEqual(string("y"), pkb->varTable.getVarName(6));
            Assert::AreEqual(string("a"), pkb->varTable.getVarName(7));
            Assert::AreEqual(string(" c "), pkb->patternTable.getExpression(1));
            Assert::AreEqual(string(" b "), pkb->patternTable.getExpression(4));
            Assert::AreEqual(string(" a "), pkb->patternTable.getExpression(5));

            // check control variable
            vector<pair<int, int>> actualControlPairs = pkb->stmtTable.getStmtControlVarPairs();
            vector<pair<int, int>> expectedControlPairs = {
                pair<int, int>(2, 3),
                pair<int, int>(3, 1),
            };
            for(pair<int, int> p : expectedControlPairs) {
                Assert::IsTrue(contains(actualControlPairs, p));
            }
            Assert::IsTrue(actualControlPairs.size() == expectedControlPairs.size());


            // check parents
            Assert::AreEqual(2, pkb->parentTable.getParent(3));
            Assert::AreEqual(3, pkb->parentTable.getParent(4));
            Assert::AreEqual(3, pkb->parentTable.getParent(5));
            std::vector<std::pair<int, int>> actualTransParentsPairs = pkb->parentStarTable.getParentStarPairs();
            std::vector<std::pair<int, int>> expectedTransParentsPairs = {
                std::pair<int, int>(2, 3),
                std::pair<int, int>(2, 4),
                std::pair<int, int>(2, 5),
                std::pair<int, int>(3, 4),
                std::pair<int, int>(3, 5),
            };
            for(std::pair<int, int> pair : expectedTransParentsPairs) {
                Assert::IsTrue(contains(actualTransParentsPairs, pair));
            }
            Assert::AreEqual(actualTransParentsPairs.size(), expectedTransParentsPairs.size());

            // check follows
            Assert::AreEqual(1, pkb->followsTable.getFollowedBy(2));
            std::vector<std::pair<int, int>> actualFollowsPairs = pkb->followsStarTable.getFollowsStarPairs();
            std::vector<std::pair<int, int>> expectedTransFollowsPairs = {
                std::pair<int, int>(1, 2),
            };
            for(std::pair<int, int> pair : expectedTransFollowsPairs) {
                Assert::IsTrue(contains(actualFollowsPairs, pair));
            }
            Assert::AreEqual(actualFollowsPairs.size(), expectedTransFollowsPairs.size());


            Assert::IsTrue(pkb->procTable.getSize() == 1);
            Assert::IsTrue(pkb->stmtTable.getSize() == 5);
            Assert::IsTrue(pkb->varTable.getSize() == 7);
            Assert::IsTrue(pkb->constTable.getSize() == 0);
            Assert::IsTrue(pkb->parentTable.getParentPairs().size() == 3);
            Assert::IsTrue(pkb->followsTable.getFollowsPairs().size() == 1);

            // check modifies
            std::vector<std::pair<int, int>> modifiesPairs = pkb->modifiesTable.getModifiesPairs();
            Assert::IsTrue(contains(modifiesPairs, std::pair<int, int>(1, 1)));
            Assert::IsTrue(contains(modifiesPairs, std::pair<int, int>(2, 4)));
            Assert::IsTrue(contains(modifiesPairs, std::pair<int, int>(2, 6)));
            Assert::IsTrue(contains(modifiesPairs, std::pair<int, int>(3, 4)));
            Assert::IsTrue(contains(modifiesPairs, std::pair<int, int>(3, 6)));
            Assert::IsTrue(contains(modifiesPairs, std::pair<int, int>(4, 4)));
            Assert::IsTrue(contains(modifiesPairs, std::pair<int, int>(5, 6)));
            Assert::IsTrue(modifiesPairs.size() == 7);

            // check uses
            std::vector<std::pair<int, int>> usesPairs = pkb->usesTable.getUsesPairs();
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(1, 2)));
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(2, 1)));
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(2, 3)));
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(2, 5)));
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(2, 7)));
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(3, 1)));
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(3, 5)));
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(3, 7)));
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(4, 5)));
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(5, 7)));
            Assert::IsTrue(usesPairs.size() == 10);


            // check proc modifies
            std::vector<std::pair<int, int>> actualProcModifiesPairs = pkb->modifiesTable.getProcModifiesPairs();
            std::vector<std::pair<int, int>> expectedProModifiesPairs = {
                std::pair<int, int>(1, 1),
                std::pair<int, int>(1, 4),
                std::pair<int, int>(1, 6),
            };
            for(std::pair<int, int> p : expectedProModifiesPairs) {
                Assert::IsTrue(contains(actualProcModifiesPairs, p));
            }
            Assert::IsTrue(actualProcModifiesPairs.size() == expectedProModifiesPairs.size());

            // check proc uses
            std::vector<std::pair<int, int>> actualProcUsesPairs = pkb->usesTable.getProcUsesPairs();
            std::vector<std::pair<int, int>> expectedProcUsesPairs = {
                std::pair<int, int>(1, 1),
                std::pair<int, int>(1, 2),
                std::pair<int, int>(1, 3),
                std::pair<int, int>(1, 5),
                std::pair<int, int>(1, 7),
            };
            for(std::pair<int, int> p : expectedProcUsesPairs) {
                Assert::IsTrue(contains(actualProcUsesPairs, p));
            }
            Assert::IsTrue(actualProcUsesPairs.size() == expectedProcUsesPairs.size());

            PKB::clearPKB();
            SourceParser::clear();
        }

        TEST_METHOD(TestParsesSourceWithNestedWhile) {
            PKB::clearPKB();
            SourceParser::clear();

            SourceCode source = R"(
                procedure First { 
                    x = c;
                    if x then {
                        while i {
                            z = b;
                        }
                    } else {
                        y = a;
                    }
                }
            )";
            PKB* pkb = PKB::getPKBInstance();
            SourceParser::getInstance(*pkb)->parse(source);

            Assert::AreEqual(string("First"), pkb->procTable.getProcName(1));
            Assert::AreEqual(string("ASSIGN"), pkb->stmtTable.getStmtType(1));
            Assert::AreEqual(string("IF"), pkb->stmtTable.getStmtType(2));
            Assert::AreEqual(string("WHILE"), pkb->stmtTable.getStmtType(3));
            Assert::AreEqual(string("ASSIGN"), pkb->stmtTable.getStmtType(4));
            Assert::AreEqual(string("ASSIGN"), pkb->stmtTable.getStmtType(5));
            Assert::AreEqual(string("x"), pkb->varTable.getVarName(1));
            Assert::AreEqual(string("c"), pkb->varTable.getVarName(2));
            Assert::AreEqual(string("i"), pkb->varTable.getVarName(3));
            Assert::AreEqual(string("z"), pkb->varTable.getVarName(4));
            Assert::AreEqual(string("b"), pkb->varTable.getVarName(5));
            Assert::AreEqual(string("y"), pkb->varTable.getVarName(6));
            Assert::AreEqual(string("a"), pkb->varTable.getVarName(7));
            Assert::AreEqual(string(" c "), pkb->patternTable.getExpression(1));
            Assert::AreEqual(string(" b "), pkb->patternTable.getExpression(4));
            Assert::AreEqual(string(" a "), pkb->patternTable.getExpression(5));

            // check control variable
            vector<pair<int, int>> actualControlPairs = pkb->stmtTable.getStmtControlVarPairs();
            vector<pair<int, int>> expectedControlPairs = {
                pair<int, int>(2, 1),
                pair<int, int>(3, 3),
            };
            for(pair<int, int> p : expectedControlPairs) {
                Assert::IsTrue(contains(actualControlPairs, p));
            }
            Assert::IsTrue(actualControlPairs.size() == expectedControlPairs.size());

            // check parents
            Assert::AreEqual(2, pkb->parentTable.getParent(3));
            Assert::AreEqual(3, pkb->parentTable.getParent(4));
            Assert::AreEqual(2, pkb->parentTable.getParent(5));
            std::vector<std::pair<int, int>> actualTransParentsPairs = pkb->parentStarTable.getParentStarPairs();
            std::vector<std::pair<int, int>> expectedTransParentsPairs = {
                std::pair<int, int>(2, 3),
                std::pair<int, int>(2, 4),
                std::pair<int, int>(2, 5),
                std::pair<int, int>(3, 4),
            };
            for(std::pair<int, int> pair : expectedTransParentsPairs) {
                Assert::IsTrue(contains(actualTransParentsPairs, pair));
            }
            Assert::AreEqual(actualTransParentsPairs.size(), expectedTransParentsPairs.size());

            // check follows
            Assert::AreEqual(1, pkb->followsTable.getFollowedBy(2));
            std::vector<std::pair<int, int>> actualFollowsPairs = pkb->followsStarTable.getFollowsStarPairs();
            std::vector<std::pair<int, int>> expectedTransFollowsPairs = {
                std::pair<int, int>(1, 2),
            };
            for(std::pair<int, int> pair : expectedTransFollowsPairs) {
                Assert::IsTrue(contains(actualFollowsPairs, pair));
            }
            Assert::AreEqual(actualFollowsPairs.size(), expectedTransFollowsPairs.size());

            
            Assert::IsTrue(pkb->procTable.getSize() == 1);
            Assert::IsTrue(pkb->stmtTable.getSize() == 5);
            Assert::IsTrue(pkb->varTable.getSize() == 7);
            Assert::IsTrue(pkb->constTable.getSize() == 0);
            Assert::IsTrue(pkb->parentTable.getParentPairs().size() == 3);
            Assert::IsTrue(pkb->followsTable.getFollowsPairs().size() == 1);

            // check modifies
            std::vector<std::pair<int, int>> modifiesPairs = pkb->modifiesTable.getModifiesPairs();
            Assert::IsTrue(contains(modifiesPairs, std::pair<int, int>(1, 1)));
            Assert::IsTrue(contains(modifiesPairs, std::pair<int, int>(2, 4)));
            Assert::IsTrue(contains(modifiesPairs, std::pair<int, int>(2, 6)));
            Assert::IsTrue(contains(modifiesPairs, std::pair<int, int>(3, 4)));
            Assert::IsTrue(contains(modifiesPairs, std::pair<int, int>(4, 4)));
            Assert::IsTrue(contains(modifiesPairs, std::pair<int, int>(5, 6)));
            Assert::IsTrue(modifiesPairs.size() == 6);

            // check uses
            std::vector<std::pair<int, int>> usesPairs = pkb->usesTable.getUsesPairs();
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(1, 2)));
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(2, 1)));
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(2, 3)));
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(2, 5)));
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(2, 7)));
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(3, 3)));
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(3, 5)));
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(4, 5)));
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(5, 7)));
            Assert::IsTrue(usesPairs.size() == 9);

            // check proc modifies
            std::vector<std::pair<int, int>> actualProcModifiesPairs = pkb->modifiesTable.getProcModifiesPairs();
            std::vector<std::pair<int, int>> expectedProModifiesPairs = {
                std::pair<int, int>(1, 1),
                std::pair<int, int>(1, 4),
                std::pair<int, int>(1, 6),
            };
            for(std::pair<int, int> p : expectedProModifiesPairs) {
                Assert::IsTrue(contains(actualProcModifiesPairs, p));
            }
            Assert::IsTrue(actualProcModifiesPairs.size() == expectedProModifiesPairs.size());

            // check proc uses
            std::vector<std::pair<int, int>> actualProcUsesPairs = pkb->usesTable.getProcUsesPairs();
            std::vector<std::pair<int, int>> expectedProcUsesPairs = {
                std::pair<int, int>(1, 1),
                std::pair<int, int>(1, 2),
                std::pair<int, int>(1, 3),
                std::pair<int, int>(1, 5),
                std::pair<int, int>(1, 7),
            };
            for(std::pair<int, int> p : expectedProcUsesPairs) {
                Assert::IsTrue(contains(actualProcUsesPairs, p));
            }
            Assert::IsTrue(actualProcUsesPairs.size() == expectedProcUsesPairs.size());

            PKB::clearPKB();
            SourceParser::clear();
        }

        TEST_METHOD(TestParsesSourceWithWhileAndIf) {
            PKB::clearPKB();
            SourceParser::clear();

            SourceCode source = R"(
                procedure First { 
                    x = c;
                    if x then {
                        x = c;
                    } else {
                        y = a;
                    }
                    while i {
                        z = b;
                    }
                }
            )";
            PKB* pkb = PKB::getPKBInstance();
            SourceParser::getInstance(*pkb)->parse(source);

            Assert::AreEqual(string("First"), pkb->procTable.getProcName(1));
            Assert::AreEqual(string("ASSIGN"), pkb->stmtTable.getStmtType(1));
            Assert::AreEqual(string("IF"), pkb->stmtTable.getStmtType(2));
            Assert::AreEqual(string("ASSIGN"), pkb->stmtTable.getStmtType(3));
            Assert::AreEqual(string("ASSIGN"), pkb->stmtTable.getStmtType(4));
            Assert::AreEqual(string("WHILE"), pkb->stmtTable.getStmtType(5));
            Assert::AreEqual(string("ASSIGN"), pkb->stmtTable.getStmtType(6));
            Assert::AreEqual(string("x"), pkb->varTable.getVarName(1));
            Assert::AreEqual(string("c"), pkb->varTable.getVarName(2));
            Assert::AreEqual(string("y"), pkb->varTable.getVarName(3));
            Assert::AreEqual(string("a"), pkb->varTable.getVarName(4));
            Assert::AreEqual(string("i"), pkb->varTable.getVarName(5));
            Assert::AreEqual(string("z"), pkb->varTable.getVarName(6));
            Assert::AreEqual(string("b"), pkb->varTable.getVarName(7));
            Assert::AreEqual(string(" c "), pkb->patternTable.getExpression(1));
            Assert::AreEqual(string(" c "), pkb->patternTable.getExpression(3));
            Assert::AreEqual(string(" a "), pkb->patternTable.getExpression(4));
            Assert::AreEqual(string(" b "), pkb->patternTable.getExpression(6));

            // check control variable
            vector<pair<int, int>> actualControlPairs = pkb->stmtTable.getStmtControlVarPairs();
            vector<pair<int, int>> expectedControlPairs = {
                pair<int, int>(2, 1),
                pair<int, int>(5, 5),
            };
            for(pair<int, int> p : expectedControlPairs) {
                Assert::IsTrue(contains(actualControlPairs, p));
            }
            Assert::IsTrue(actualControlPairs.size() == expectedControlPairs.size());

            // check parents
            Assert::AreEqual(2, pkb->parentTable.getParent(3));
            Assert::AreEqual(2, pkb->parentTable.getParent(4));
            Assert::AreEqual(5, pkb->parentTable.getParent(6));
            std::vector<std::pair<int, int>> actualTransParentsPairs = pkb->parentStarTable.getParentStarPairs();
            std::vector<std::pair<int, int>> expectedTransParentsPairs = {
                std::pair<int, int>(2, 3),
                std::pair<int, int>(2, 4),
                std::pair<int, int>(5, 6),
            };
            for(std::pair<int, int> pair : expectedTransParentsPairs) {
                Assert::IsTrue(contains(actualTransParentsPairs, pair));
            }
            Assert::AreEqual(actualTransParentsPairs.size(), expectedTransParentsPairs.size());

            // check follows
            Assert::AreEqual(1, pkb->followsTable.getFollowedBy(2));
            Assert::AreEqual(2, pkb->followsTable.getFollowedBy(5));
            std::vector<std::pair<int, int>> actualFollowsPairs = pkb->followsStarTable.getFollowsStarPairs();
            std::vector<std::pair<int, int>> expectedTransFollowsPairs = {
                std::pair<int, int>(1, 2),
                std::pair<int, int>(1, 5),
                std::pair<int, int>(2, 5),
            };
            for(std::pair<int, int> pair : expectedTransFollowsPairs) {
                Assert::IsTrue(contains(actualFollowsPairs, pair));
            }
            Assert::AreEqual(actualFollowsPairs.size(), expectedTransFollowsPairs.size());


            Assert::IsTrue(pkb->procTable.getSize() == 1);
            Assert::IsTrue(pkb->stmtTable.getSize() == 6);
            Assert::IsTrue(pkb->varTable.getSize() == 7);
            Assert::IsTrue(pkb->constTable.getSize() == 0);
            Assert::IsTrue(pkb->parentTable.getParentPairs().size() == 3);
            Assert::IsTrue(pkb->followsTable.getFollowsPairs().size() == 2);

            // check modifies
            std::vector<std::pair<int, int>> modifiesPairs = pkb->modifiesTable.getModifiesPairs();
            Assert::IsTrue(contains(modifiesPairs, std::pair<int, int>(1, 1)));
            Assert::IsTrue(contains(modifiesPairs, std::pair<int, int>(2, 1)));
            Assert::IsTrue(contains(modifiesPairs, std::pair<int, int>(3, 1)));
            Assert::IsTrue(contains(modifiesPairs, std::pair<int, int>(4, 3)));
            Assert::IsTrue(contains(modifiesPairs, std::pair<int, int>(6, 6)));
            Assert::IsTrue(contains(modifiesPairs, std::pair<int, int>(2, 3)));
            Assert::IsTrue(contains(modifiesPairs, std::pair<int, int>(5, 6)));
            Assert::IsTrue(modifiesPairs.size() == 7);

            // check uses
            std::vector<std::pair<int, int>> usesPairs = pkb->usesTable.getUsesPairs();
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(1, 2)));
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(2, 1)));
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(2, 2)));
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(2, 4)));
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(3, 2)));
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(4, 4)));
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(5, 5)));
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(5, 7)));
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(6, 7)));
            Assert::IsTrue(usesPairs.size() == 9);

            // check proc modifies
            std::vector<std::pair<int, int>> actualProcModifiesPairs = pkb->modifiesTable.getProcModifiesPairs();
            std::vector<std::pair<int, int>> expectedProModifiesPairs = {
                std::pair<int, int>(1, 1),
                std::pair<int, int>(1, 3),
                std::pair<int, int>(1, 6),
            };
            for(std::pair<int, int> p : expectedProModifiesPairs) {
                Assert::IsTrue(contains(actualProcModifiesPairs, p));
            }
            Assert::IsTrue(actualProcModifiesPairs.size() == expectedProModifiesPairs.size());

            // check proc uses
            std::vector<std::pair<int, int>> actualProcUsesPairs = pkb->usesTable.getProcUsesPairs();
            std::vector<std::pair<int, int>> expectedProcUsesPairs = {
                std::pair<int, int>(1, 1),
                std::pair<int, int>(1, 2),
                std::pair<int, int>(1, 4),
                std::pair<int, int>(1, 5),
                std::pair<int, int>(1, 7),
            };
            for(std::pair<int, int> p : expectedProcUsesPairs) {
                Assert::IsTrue(contains(actualProcUsesPairs, p));
            }
            Assert::IsTrue(actualProcUsesPairs.size() == expectedProcUsesPairs.size());

            PKB::clearPKB();
            SourceParser::clear();
        }

        TEST_METHOD(TestParsesSourceWithWhileMultipleStatement) {
            PKB::clearPKB();
            SourceParser::clear();

            SourceCode source = R"(
                procedure First { 
                    while i {
                        x = a;
                        x = b;
                    }
                }
            )";
            PKB* pkb = PKB::getPKBInstance();
            SourceParser::getInstance(*pkb)->parse(source);

            Assert::AreEqual(string("First"), pkb->procTable.getProcName(1));
            Assert::AreEqual(string("WHILE"), pkb->stmtTable.getStmtType(1));
            Assert::AreEqual(string("ASSIGN"), pkb->stmtTable.getStmtType(2));
            Assert::AreEqual(string("ASSIGN"), pkb->stmtTable.getStmtType(3));
            Assert::AreEqual(string("i"), pkb->varTable.getVarName(1));
            Assert::AreEqual(string("x"), pkb->varTable.getVarName(2));
            Assert::AreEqual(string("a"), pkb->varTable.getVarName(3));
            Assert::AreEqual(string("b"), pkb->varTable.getVarName(4));
            Assert::AreEqual(string(" a "), pkb->patternTable.getExpression(2));
            Assert::AreEqual(string(" b "), pkb->patternTable.getExpression(3));

            // check control variable
            vector<pair<int, int>> actualControlPairs = pkb->stmtTable.getStmtControlVarPairs();
            vector<pair<int, int>> expectedControlPairs = {
                pair<int, int>(1, 1),
            };
            for(pair<int, int> p : expectedControlPairs) {
                Assert::IsTrue(contains(actualControlPairs, p));
            }
            Assert::IsTrue(actualControlPairs.size() == expectedControlPairs.size());

            // check parents
            Assert::AreEqual(1, pkb->parentTable.getParent(2));
            Assert::AreEqual(1, pkb->parentTable.getParent(3));
            std::vector<std::pair<int, int>> actualTransParentsPairs = pkb->parentStarTable.getParentStarPairs();
            std::vector<std::pair<int, int>> expectedTransParentsPairs = {
                std::pair<int, int>(1, 2),
                std::pair<int, int>(1, 3),
            };
            for(std::pair<int, int> pair : expectedTransParentsPairs) {
                Assert::IsTrue(contains(actualTransParentsPairs, pair));
            }
            Assert::AreEqual(actualTransParentsPairs.size(), expectedTransParentsPairs.size());

            // check follows
            Assert::AreEqual(2, pkb->followsTable.getFollowedBy(3));
            std::vector<std::pair<int, int>> actualFollowsPairs = pkb->followsStarTable.getFollowsStarPairs();
            std::vector<std::pair<int, int>> expectedTransFollowsPairs = {
                std::pair<int, int>(2, 3),
            };
            for(std::pair<int, int> pair : expectedTransFollowsPairs) {
                Assert::IsTrue(contains(actualFollowsPairs, pair));
            }
            Assert::AreEqual(actualFollowsPairs.size(), expectedTransFollowsPairs.size());

            Assert::IsTrue(pkb->procTable.getSize() == 1);
            Assert::IsTrue(pkb->stmtTable.getSize() == 3);
            Assert::IsTrue(pkb->varTable.getSize() == 4);
            Assert::IsTrue(pkb->constTable.getSize() == 0);
            Assert::IsTrue(pkb->parentTable.getParentPairs().size() == 2);
            Assert::IsTrue(pkb->followsTable.getFollowsPairs().size() == 1);

            // check modifies
            std::vector<std::pair<int, int>> modifiesPairs = pkb->modifiesTable.getModifiesPairs();
            Assert::IsTrue(contains(modifiesPairs, std::pair<int, int>(1, 2)));
            Assert::IsTrue(contains(modifiesPairs, std::pair<int, int>(2, 2)));
            Assert::IsTrue(contains(modifiesPairs, std::pair<int, int>(3, 2)));
            Assert::IsTrue(modifiesPairs.size() == 3);

            // check uses
            std::vector<std::pair<int, int>> usesPairs = pkb->usesTable.getUsesPairs();
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(1, 1)));
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(1, 3)));
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(1, 4)));
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(2, 3)));
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(3, 4)));
            Assert::IsTrue(usesPairs.size() == 5);

            // check proc modifies
            std::vector<std::pair<int, int>> actualProcModifiesPairs = pkb->modifiesTable.getProcModifiesPairs();
            std::vector<std::pair<int, int>> expectedProModifiesPairs = {
                std::pair<int, int>(1, 2),
            };
            for(std::pair<int, int> p : expectedProModifiesPairs) {
                Assert::IsTrue(contains(actualProcModifiesPairs, p));
            }
            Assert::IsTrue(actualProcModifiesPairs.size() == expectedProModifiesPairs.size());

            // check proc uses
            std::vector<std::pair<int, int>> actualProcUsesPairs = pkb->usesTable.getProcUsesPairs();
            std::vector<std::pair<int, int>> expectedProcUsesPairs = {
                std::pair<int, int>(1, 1),
                std::pair<int, int>(1, 3),
                std::pair<int, int>(1, 4),
            };
            for(std::pair<int, int> p : expectedProcUsesPairs) {
                Assert::IsTrue(contains(actualProcUsesPairs, p));
            }
            Assert::IsTrue(actualProcUsesPairs.size() == expectedProcUsesPairs.size());

            PKB::clearPKB();
            SourceParser::clear();
        }

        TEST_METHOD(TestParsesSourceWithIfMultipleStatement) {
            PKB::clearPKB();
            SourceParser::clear();

            SourceCode source = R"(
                procedure First { 
                    if i then {
                        x = a;
                        x = b;
                    } else {
                        x = b;
                        x = a;
                    }
                }
            )";
            PKB* pkb = PKB::getPKBInstance();
            SourceParser::getInstance(*pkb)->parse(source);

            Assert::AreEqual(string("First"), pkb->procTable.getProcName(1));
            Assert::AreEqual(string("IF"), pkb->stmtTable.getStmtType(1));
            Assert::AreEqual(string("ASSIGN"), pkb->stmtTable.getStmtType(2));
            Assert::AreEqual(string("ASSIGN"), pkb->stmtTable.getStmtType(3));
            Assert::AreEqual(string("ASSIGN"), pkb->stmtTable.getStmtType(4));
            Assert::AreEqual(string("ASSIGN"), pkb->stmtTable.getStmtType(5));
            Assert::AreEqual(string("i"), pkb->varTable.getVarName(1));
            Assert::AreEqual(string("x"), pkb->varTable.getVarName(2));
            Assert::AreEqual(string("a"), pkb->varTable.getVarName(3));
            Assert::AreEqual(string("b"), pkb->varTable.getVarName(4));
            Assert::AreEqual(string(" a "), pkb->patternTable.getExpression(2));
            Assert::AreEqual(string(" b "), pkb->patternTable.getExpression(3));
            Assert::AreEqual(string(" b "), pkb->patternTable.getExpression(4));
            Assert::AreEqual(string(" a "), pkb->patternTable.getExpression(5));

            // check control variable
            vector<pair<int, int>> actualControlPairs = pkb->stmtTable.getStmtControlVarPairs();
            vector<pair<int, int>> expectedControlPairs = {
                pair<int, int>(1, 1),
            };
            for(pair<int, int> p : expectedControlPairs) {
                Assert::IsTrue(contains(actualControlPairs, p));
            }
            Assert::IsTrue(actualControlPairs.size() == expectedControlPairs.size());

            // check parents
            Assert::AreEqual(1, pkb->parentTable.getParent(2));
            Assert::AreEqual(1, pkb->parentTable.getParent(3));
            Assert::AreEqual(1, pkb->parentTable.getParent(4));
            Assert::AreEqual(1, pkb->parentTable.getParent(5));
            std::vector<std::pair<int, int>> actualTransParentsPairs = pkb->parentStarTable.getParentStarPairs();
            std::vector<std::pair<int, int>> expectedTransParentsPairs = {
                std::pair<int, int>(1, 2),
                std::pair<int, int>(1, 3),
                std::pair<int, int>(1, 4),
                std::pair<int, int>(1, 5),
            };
            for(std::pair<int, int> pair : expectedTransParentsPairs) {
                Assert::IsTrue(contains(actualTransParentsPairs, pair));
            }
            Assert::AreEqual(actualTransParentsPairs.size(), expectedTransParentsPairs.size());

            // check follows
            Assert::AreEqual(2, pkb->followsTable.getFollowedBy(3));
            Assert::AreEqual(4, pkb->followsTable.getFollowedBy(5));
            std::vector<std::pair<int, int>> actualFollowsPairs = pkb->followsStarTable.getFollowsStarPairs();
            std::vector<std::pair<int, int>> expectedTransFollowsPairs = {
                std::pair<int, int>(2, 3),
                std::pair<int, int>(4, 5),
            };
            for(std::pair<int, int> pair : expectedTransFollowsPairs) {
                Assert::IsTrue(contains(actualFollowsPairs, pair));
            }
            Assert::AreEqual(actualFollowsPairs.size(), expectedTransFollowsPairs.size());


            Assert::IsTrue(pkb->procTable.getSize() == 1);
            Assert::IsTrue(pkb->stmtTable.getSize() == 5);
            Assert::IsTrue(pkb->varTable.getSize() == 4);
            Assert::IsTrue(pkb->constTable.getSize() == 0);
            Assert::IsTrue(pkb->parentTable.getParentPairs().size() == 4);
            Assert::IsTrue(pkb->followsTable.getFollowsPairs().size() == 2);

            // check modifies
            std::vector<std::pair<int, int>> modifiesPairs = pkb->modifiesTable.getModifiesPairs();
            Assert::IsTrue(contains(modifiesPairs, std::pair<int, int>(1, 2)));
            Assert::IsTrue(contains(modifiesPairs, std::pair<int, int>(2, 2)));
            Assert::IsTrue(contains(modifiesPairs, std::pair<int, int>(3, 2)));
            Assert::IsTrue(contains(modifiesPairs, std::pair<int, int>(4, 2)));
            Assert::IsTrue(contains(modifiesPairs, std::pair<int, int>(5, 2)));
            Assert::IsTrue(modifiesPairs.size() == 5);

            // check uses
            std::vector<std::pair<int, int>> usesPairs = pkb->usesTable.getUsesPairs();
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(1, 1)));
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(1, 3)));
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(1, 4)));
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(2, 3)));
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(3, 4)));
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(5, 3)));
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(4, 4)));
            Assert::IsTrue(usesPairs.size() == 7);

            // check proc modifies
            std::vector<std::pair<int, int>> actualProcModifiesPairs = pkb->modifiesTable.getProcModifiesPairs();
            std::vector<std::pair<int, int>> expectedProModifiesPairs = {
                std::pair<int, int>(1, 2),
            };
            for(std::pair<int, int> p : expectedProModifiesPairs) {
                Assert::IsTrue(contains(actualProcModifiesPairs, p));
            }
            Assert::IsTrue(actualProcModifiesPairs.size() == expectedProModifiesPairs.size());

            // check proc uses
            std::vector<std::pair<int, int>> actualProcUsesPairs = pkb->usesTable.getProcUsesPairs();
            std::vector<std::pair<int, int>> expectedProcUsesPairs = {
                std::pair<int, int>(1, 1),
                std::pair<int, int>(1, 3),
                std::pair<int, int>(1, 4),
            };
            for(std::pair<int, int> p : expectedProcUsesPairs) {
                Assert::IsTrue(contains(actualProcUsesPairs, p));
            }
            Assert::IsTrue(actualProcUsesPairs.size() == expectedProcUsesPairs.size());

            PKB::clearPKB();
            SourceParser::clear();
        }

        TEST_METHOD(TestParsesSourceWithWhileAndIfArbitraryNesting) {
            PKB::clearPKB();
            SourceParser::clear();

            SourceCode source = R"(
                procedure First { 
                    if i then {
                        while i {
                            if i then {
                                x = a;
                                x = b;
                            } else {
                                x = c;
                            }
                        }
                    } else {
                        while i {
                            x = d;
                        }
                    }
                }
            )";
            PKB* pkb = PKB::getPKBInstance();
            SourceParser::getInstance(*pkb)->parse(source);

            Assert::AreEqual(string("First"), pkb->procTable.getProcName(1));
            Assert::AreEqual(string("IF"), pkb->stmtTable.getStmtType(1));
            Assert::AreEqual(string("WHILE"), pkb->stmtTable.getStmtType(2));
            Assert::AreEqual(string("IF"), pkb->stmtTable.getStmtType(3));
            Assert::AreEqual(string("ASSIGN"), pkb->stmtTable.getStmtType(4));
            Assert::AreEqual(string("ASSIGN"), pkb->stmtTable.getStmtType(5));
            Assert::AreEqual(string("ASSIGN"), pkb->stmtTable.getStmtType(6));
            Assert::AreEqual(string("WHILE"), pkb->stmtTable.getStmtType(7));
            Assert::AreEqual(string("ASSIGN"), pkb->stmtTable.getStmtType(8));
            Assert::AreEqual(string("i"), pkb->varTable.getVarName(1));
            Assert::AreEqual(string("x"), pkb->varTable.getVarName(2));
            Assert::AreEqual(string("a"), pkb->varTable.getVarName(3));
            Assert::AreEqual(string("b"), pkb->varTable.getVarName(4));
            Assert::AreEqual(string("c"), pkb->varTable.getVarName(5));
            Assert::AreEqual(string("d"), pkb->varTable.getVarName(6));
            Assert::AreEqual(string(" a "), pkb->patternTable.getExpression(4));
            Assert::AreEqual(string(" b "), pkb->patternTable.getExpression(5));
            Assert::AreEqual(string(" c "), pkb->patternTable.getExpression(6));
            Assert::AreEqual(string(" d "), pkb->patternTable.getExpression(8));

            // check control variable
            vector<pair<int, int>> actualControlPairs = pkb->stmtTable.getStmtControlVarPairs();
            vector<pair<int, int>> expectedControlPairs = {
                pair<int, int>(1, 1),
                pair<int, int>(2, 1),
                pair<int, int>(3, 1),
                pair<int, int>(7, 1),
            };
            for(pair<int, int> p : expectedControlPairs) {
                Assert::IsTrue(contains(actualControlPairs, p));
            }
            Assert::IsTrue(actualControlPairs.size() == expectedControlPairs.size());

            // check parents
            Assert::AreEqual(1, pkb->parentTable.getParent(2));
            Assert::AreEqual(2, pkb->parentTable.getParent(3));
            Assert::AreEqual(3, pkb->parentTable.getParent(4));
            Assert::AreEqual(3, pkb->parentTable.getParent(5));
            Assert::AreEqual(3, pkb->parentTable.getParent(6));
            Assert::AreEqual(1, pkb->parentTable.getParent(7));
            Assert::AreEqual(7, pkb->parentTable.getParent(8));
            std::vector<std::pair<int, int>> actualTransParentsPairs = pkb->parentStarTable.getParentStarPairs();
            std::vector<std::pair<int, int>> expectedTransParentsPairs = {
                std::pair<int, int>(1, 2),
                std::pair<int, int>(1, 3),
                std::pair<int, int>(1, 4),
                std::pair<int, int>(1, 5),
                std::pair<int, int>(1, 6),
                std::pair<int, int>(1, 7),
                std::pair<int, int>(1, 8),
                std::pair<int, int>(2, 3),
                std::pair<int, int>(2, 4),
                std::pair<int, int>(2, 5),
                std::pair<int, int>(2, 6),
                std::pair<int, int>(3, 4),
                std::pair<int, int>(3, 5),
                std::pair<int, int>(3, 6),
                std::pair<int, int>(7, 8),
            };
            for(std::pair<int, int> pair : expectedTransParentsPairs) {
                Assert::IsTrue(contains(actualTransParentsPairs, pair));
            }
            Assert::AreEqual(actualTransParentsPairs.size(), expectedTransParentsPairs.size());

            // check follows
            Assert::AreEqual(4, pkb->followsTable.getFollowedBy(5));
            std::vector<std::pair<int, int>> actualFollowsPairs = pkb->followsStarTable.getFollowsStarPairs();
            std::vector<std::pair<int, int>> expectedTransFollowsPairs = {
                std::pair<int, int>(4, 5),
            };
            for(std::pair<int, int> pair : expectedTransFollowsPairs) {
                Assert::IsTrue(contains(actualFollowsPairs, pair));
            }
            Assert::AreEqual(actualFollowsPairs.size(), expectedTransFollowsPairs.size());

            Assert::IsTrue(pkb->procTable.getSize() == 1);
            Assert::IsTrue(pkb->stmtTable.getSize() == 8);
            Assert::IsTrue(pkb->varTable.getSize() == 6);
            Assert::IsTrue(pkb->constTable.getSize() == 0);
            Assert::IsTrue(pkb->parentTable.getParentPairs().size() == 7);
            Assert::IsTrue(pkb->followsTable.getFollowsPairs().size() == 1);

            // check modifies
            std::vector<std::pair<int, int>> modifiesPairs = pkb->modifiesTable.getModifiesPairs();
            Assert::IsTrue(contains(modifiesPairs, std::pair<int, int>(1, 2)));
            Assert::IsTrue(contains(modifiesPairs, std::pair<int, int>(2, 2)));
            Assert::IsTrue(contains(modifiesPairs, std::pair<int, int>(3, 2)));
            Assert::IsTrue(contains(modifiesPairs, std::pair<int, int>(4, 2)));
            Assert::IsTrue(contains(modifiesPairs, std::pair<int, int>(5, 2)));
            Assert::IsTrue(contains(modifiesPairs, std::pair<int, int>(6, 2)));
            Assert::IsTrue(contains(modifiesPairs, std::pair<int, int>(7, 2)));
            Assert::IsTrue(contains(modifiesPairs, std::pair<int, int>(8, 2)));
            Assert::IsTrue(modifiesPairs.size() == 8);

            // check uses
            std::vector<std::pair<int, int>> usesPairs = pkb->usesTable.getUsesPairs();
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(1, 1)));
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(1, 3)));
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(1, 4)));
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(1, 5)));
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(1, 6)));
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(2, 1)));
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(2, 3)));
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(2, 4)));
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(2, 5)));
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(3, 1)));
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(3, 3)));
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(3, 4)));
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(3, 5)));
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(4, 3)));
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(5, 4)));
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(6, 5)));
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(7, 1)));
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(7, 6)));
            Assert::IsTrue(contains(usesPairs, std::pair<int, int>(8, 6)));
            Assert::IsTrue(usesPairs.size() == 19);

            // check proc modifies
            std::vector<std::pair<int, int>> actualProcModifiesPairs = pkb->modifiesTable.getProcModifiesPairs();
            std::vector<std::pair<int, int>> expectedProModifiesPairs = {
                std::pair<int, int>(1, 2),
            };
            for(std::pair<int, int> p : expectedProModifiesPairs) {
                Assert::IsTrue(contains(actualProcModifiesPairs, p));
            }
            Assert::IsTrue(actualProcModifiesPairs.size() == expectedProModifiesPairs.size());

            // check proc uses
            std::vector<std::pair<int, int>> actualProcUsesPairs = pkb->usesTable.getProcUsesPairs();
            std::vector<std::pair<int, int>> expectedProcUsesPairs = {
                std::pair<int, int>(1, 1),
                std::pair<int, int>(1, 3),
                std::pair<int, int>(1, 4),
                std::pair<int, int>(1, 5),
                std::pair<int, int>(1, 6),
            };
            for(std::pair<int, int> p : expectedProcUsesPairs) {
                Assert::IsTrue(contains(actualProcUsesPairs, p));
            }
            Assert::IsTrue(actualProcUsesPairs.size() == expectedProcUsesPairs.size());

            PKB::clearPKB();
            SourceParser::clear();
        }

		// test whether follows* relationship is stored correctly in pkb
		TEST_METHOD(TestPKBFollowsStarRelations) {
            PKB::clearPKB();
            SourceParser::clear();

			SourceCode source = R"(
                procedure First { 
                    if ddd then {		
		               while y {			
			              ddd = 0*9; }      }	
	                else {
		               while y {				
			              ddd = z*x; }	
			        }
                }
            )";
			PKB::clearPKB();
			PKB* pkb = PKB::getPKBInstance();
			SourceParser::getInstance(*pkb)->parse(source);

			std::unordered_set<int> expected = {};
			Assert::IsTrue(expected == pkb->getTransFollowing(3));
			Assert::IsTrue(expected == pkb->getTransFollowedBy(5));

			PKB::clearPKB();
			SourceParser::clear();
		}

        TEST_METHOD(TestPKBFollowsStarRelations2) {
            PKB::clearPKB();
            SourceParser::clear();

            SourceCode source = R"(
procedure One {
     x = 123;       
     y = 0;          
    if i then {      
        i = x; }    
    else {
        i = y;             } 
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
            PKB::clearPKB();
            PKB* pkb = PKB::getPKBInstance();
            SourceParser::getInstance(*pkb)->parse(source);

            std::unordered_set<int> expected = {7,8,10,11,12,16};
            Assert::IsTrue(expected == pkb->getTransFollowing(6));

            PKB::clearPKB();
            SourceParser::clear();
        }

		TEST_METHOD(TestSourceA) {
			SourceCode source = R"(
               procedure One {
     x = 123;       
     y = 0;            
    if i then {       
        i = x; }   
    else {
        i = y;             } 
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
			std::unordered_set<std::pair<int, int>, pair_hash> allFollowsPairs = pkb->getFollowsPairs();
			Logger::WriteMessage("all follows pairs:");
			for (auto &s : allFollowsPairs) {
				std::string pairStr = std::to_string(s.first) + "," + std::to_string(s.second);
				Logger::WriteMessage(pairStr.c_str());
			}

            std::unordered_set<std::pair<int, int>, pair_hash> allFollowsStarPairs = pkb->getTransFollowsPairs();
			Logger::WriteMessage("all follows star pairs:");
			for (auto &s : allFollowsStarPairs) {
				std::string pairStr = std::to_string(s.first) + "," + std::to_string(s.second);
				Logger::WriteMessage(pairStr.c_str());
			}
			/**
			debug result: all the follows and follows* relations are stored correctly by manually checking the output
			*/

            std::unordered_set<std::pair<int, int>, pair_hash> allParentPairs = pkb->getParentPairs();
			Logger::WriteMessage("all parent pairs:");
			for (auto &s : allParentPairs) {
				std::string pairStr = std::to_string(s.first) + "," + std::to_string(s.second);
				Logger::WriteMessage(pairStr.c_str());
			}

            std::unordered_set<std::pair<int, int>, pair_hash> allParentStarPairs = pkb->getTransParentPairs();
			Logger::WriteMessage("all parent star pairs:");
			for (auto &s : allParentStarPairs) {
				std::string pairStr = std::to_string(s.first) + "," + std::to_string(s.second);
				Logger::WriteMessage(pairStr.c_str());
			}
			/**
			debug result: all the parents and parents* relations are stored correctly by manually checking the output
			*/
			// debug for transfollowing
			std::unordered_set<int> result = pkb->getTransFollowing(6);
			Logger::WriteMessage("stmts follows 6:");
			for (auto &s : result) {
			    Logger::WriteMessage(std::to_string(s).c_str());
			}
			/**
			// debug for out_Follows.xml
			int result1 = pkb->getFollowedBy(2);
			Assert::AreEqual(1, result1);

			int result2 = pkb->getFollowing(10);
			Assert::AreEqual(11, result2);

			std::vector<int> result3 = pkb->getTransChildrenOf(17);
			Logger::WriteMessage("children of 17:");
			for (auto &s : result3) {
				Logger::WriteMessage(std::to_string(s).c_str());
			}
			*/
			PKB::clearPKB();
			SourceParser::clear();
		};

		TEST_METHOD(TestSourcewithMultipleNested) {
			SourceCode source = R"(
               procedure nested {
    while a {
        if b then {
            c = d;
            d = e*f + g + h*k; } 
        else {
            e = c;
            g = h + i*j;}
        k = b + 6;
        m = n * (x+y);
        if k then {
            a = g*f + 5;
            while b {
                if x then {
                    m = i*j + 11;
                    if c then {
                        e = d + c*f; } 
                    else {
                        m = 5;}
                    x = g + 5 + a*b + c; } 
                else {
                    while g {
                        h = a*6;}}}
            n = c + g;} 
        else {
            if x then {
                n = b + e*6; }
            else {
                x = (a+b) * (c+d);}}}
    a = a;
    if a then {
        b = c*2 + 1;}
    else {
        o = d + 1;}}
            )";
			PKB* pkb = PKB::getPKBInstance();
			SourceParser::getInstance(*pkb)->parse(source);
			std::unordered_set<int> expected = { 2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23 };
			Assert::IsTrue(expected == pkb->getTransChildrenOf(1));

			std::unordered_set<int> expected2 = { 14, 17 };
			Assert::IsTrue(expected2 == pkb->getTransFollowing(13));
			PKB::clearPKB();
			SourceParser::clear();
		}

		TEST_METHOD(Test2a41) {
			SourceCode source = R"(
               procedure alpha{
	call beta;
	x = 7;
	y = 123 * z;
	z = b - y;
	if a then {
		z = x + 24;
		call gamma;
	}
	else{
		while z{
			y = x - 5;
			z = z - 7;
		}
		y = x + y * z + b * a;
	}
	i = x + j + z;
	call delta; 
}

procedure beta {
	a = y - z * 99;
	while t {
		a = 5 * (b - 5);
		x = 7 + y;
		if i then {
			b = 500;
		}
		else {
			call gamma;
		}
	}
	a = b * 5;
	b = b * 99; 
}

procedure gamma{
	i = x * 99;
	j = x + y * 99;
	if i then {
		x = j + 123;
		call delta;
	}
	else {
		y = i + a + b;
	}
}

procedure delta {
	a = (b + i - j) * p - q;
}
            )";
			PKB* pkb = PKB::getPKBInstance();
			SourceParser::getInstance(*pkb)->parse(source);
			int a = pkb->getProcedure("alpha");
			int b = pkb->getProcedure("beta");
			int g = pkb->getProcedure("gamma");
			int d = pkb->getProcedure("delta");
			// debug: call setProcCallModifies() in sequence
			//DesignExtractor::setProcCallModifies(g, d);
			//DesignExtractor::setProcCallModifies(b, g);
			//DesignExtractor::setProcCallModifies(b, g);
			//DesignExtractor::setProcCallModifies(a, b);
			//DesignExtractor::setProcCallModifies(a, g);
			//DesignExtractor::setProcCallModifies(a, d);

			int proc = pkb->getProcedure("alpha");
			std::unordered_set<int> answer = pkb->getModifiedByProcedure(proc);
			unordered_set<std::string> synonymList;
			for (std::unordered_set<int>::iterator it = answer.begin(); it != answer.end(); ++it) {
				synonymList.insert(pkb->getVariableRepresentation(*it));
			}
			Logger::WriteMessage("proc \"alpha\" Modifies pairs:");
			for (auto &s : answer) {
				Logger::WriteMessage(pkb->getVariableRepresentation(s).c_str());
			}
			Assert::IsTrue(synonymList == std::unordered_set<std::string>{"a", "b", "i", "j", "x", "y", "z"});

			PKB::clearPKB();
			SourceParser::clear();
		}
        TEST_METHOD(TestMultiProcedureScrambledOrder) {
            PKB::clearPKB();
            SourceParser::clear();
            SourceCode source = R"(
                procedure C {
                    y = b;
                    call D;
                }
                procedure D {
                    z = c;
                }
                procedure A {
                    call B;
                }
                procedure B {
                    x = a;
                    call C;
                }
            )";
            PKB* pkb = PKB::getPKBInstance();
            SourceParser::getInstance(*pkb)->parse(source);
            int a = pkb->getProcedure("A");
            int b = pkb->getProcedure("B");
            int c = pkb->getProcedure("C");
            int d = pkb->getProcedure("D");

            // call relation
            std::unordered_set<std::string> calledByA;
            for (int i : pkb->getCalledByProcedure(a)) {
                calledByA.insert(pkb->getProcedureRepresentation(i));
            }
            Assert::IsTrue(calledByA == std::unordered_set<std::string>{"B"});

            std::unordered_set<std::string> calledByB;
            for (int i : pkb->getCalledByProcedure(b)) {
                calledByB.insert(pkb->getProcedureRepresentation(i));
            }
            Assert::IsTrue(calledByB == std::unordered_set<std::string>{"C"});

            std::unordered_set<std::string> calledByC;
            for (int i : pkb->getCalledByProcedure(c)) {
                calledByC.insert(pkb->getProcedureRepresentation(i));
            }
            Assert::IsTrue(calledByC == std::unordered_set<std::string>{"D"});

            std::unordered_set<std::string> calledByD;
            for (int i : pkb->getCalledByProcedure(d)) {
                calledByD.insert(pkb->getProcedureRepresentation(i));
            }
            Assert::IsTrue(calledByD == std::unordered_set<std::string>{});

            // trans call relation
            std::unordered_set<std::string> transCalledByA;
            for (int i : pkb->getTransCalledByProcedure(a)) {
                transCalledByA.insert(pkb->getProcedureRepresentation(i));
            }
            Assert::IsTrue(transCalledByA == std::unordered_set<std::string>{"B", "C", "D"});
            std::unordered_set<std::string> reverseTransCallingA;
            for (int i : pkb->getProcedureTransCalling(a)) {
                reverseTransCallingA.insert(pkb->getProcedureRepresentation(i));
            }
            Assert::IsTrue(reverseTransCallingA == std::unordered_set<std::string>{});

            std::unordered_set<std::string> transCalledByB;
            for (int i : pkb->getTransCalledByProcedure(b)) {
                transCalledByB.insert(pkb->getProcedureRepresentation(i));
            }
            Assert::IsTrue(transCalledByB == std::unordered_set<std::string>{"C", "D"});
            std::unordered_set<std::string> reverseTransCallingB;
            for (int i : pkb->getProcedureTransCalling(b)) {
                reverseTransCallingB.insert(pkb->getProcedureRepresentation(i));
            }
            Assert::IsTrue(reverseTransCallingB == std::unordered_set<std::string>{"A"});

            std::unordered_set<std::string> transCalledByC;
            for (int i : pkb->getTransCalledByProcedure(c)) {
                transCalledByC.insert(pkb->getProcedureRepresentation(i));
            }
            Assert::IsTrue(transCalledByC == std::unordered_set<std::string>{"D"});
            std::unordered_set<std::string> reverseTransCallingC;
            for (int i : pkb->getProcedureTransCalling(c)) {
                reverseTransCallingC.insert(pkb->getProcedureRepresentation(i));
            }
            Assert::IsTrue(reverseTransCallingC == std::unordered_set<std::string>{"A", "B"});

            std::unordered_set<std::string> transCalledByD;
            for (int i : pkb->getTransCalledByProcedure(d)) {
                transCalledByD.insert(pkb->getProcedureRepresentation(i));
            }
            Assert::IsTrue(transCalledByD == std::unordered_set<std::string>{});
            std::unordered_set<std::string> reverseTransCallingD;
            for (int i : pkb->getProcedureTransCalling(d)) {
                reverseTransCallingD.insert(pkb->getProcedureRepresentation(i));
            }
            Assert::IsTrue(reverseTransCallingD == std::unordered_set<std::string>{"A", "B", "C"});

            // check modified variables
            std::unordered_set<std::string> modifiedByA;
            for (int i : pkb->getModifiedByProcedure(a)) {
                modifiedByA.insert(pkb->getVariableRepresentation(i));
            }
            Assert::IsTrue(modifiedByA == std::unordered_set<std::string>{"x", "y", "z"});

            std::unordered_set<std::string> modifiedByB;
            for (int i : pkb->getModifiedByProcedure(b)) {
                modifiedByB.insert(pkb->getVariableRepresentation(i));
            }
            Assert::IsTrue(modifiedByB == std::unordered_set<std::string>{"x", "y", "z"});
            std::unordered_set<std::string> modifiedByCallB;
            for (int i : pkb->getModifiedByStmt(4)) {
                modifiedByCallB.insert(pkb->getVariableRepresentation(i));
            }
            Assert::IsTrue(modifiedByCallB == std::unordered_set<std::string>{"x", "y", "z"});

            std::unordered_set<std::string> modifiedByC;
            for (int i : pkb->getModifiedByProcedure(c)) {
                modifiedByC.insert(pkb->getVariableRepresentation(i));
            }
            Assert::IsTrue(modifiedByC == std::unordered_set<std::string>{"y", "z"});
            std::unordered_set<std::string> modifiedByCallC;
            for (int i : pkb->getModifiedByStmt(6)) {
                modifiedByCallC.insert(pkb->getVariableRepresentation(i));
            }
            Assert::IsTrue(modifiedByCallC == std::unordered_set<std::string>{"y", "z"});

            std::unordered_set<std::string> modifiedByD;
            for (int i : pkb->getModifiedByProcedure(d)) {
                modifiedByD.insert(pkb->getVariableRepresentation(i));
            }
            Assert::IsTrue(modifiedByD == std::unordered_set<std::string>{"z"});
            std::unordered_set<std::string> modifiedByCallD;
            for (int i : pkb->getModifiedByStmt(2)) {
                modifiedByCallD.insert(pkb->getVariableRepresentation(i));
            }
            Assert::IsTrue(modifiedByCallD == std::unordered_set<std::string>{"z"});

            // check used variables
            std::unordered_set<std::string> usedByA;
            for (int i : pkb->getUsedByProcedure(a)) {
                usedByA.insert(pkb->getVariableRepresentation(i));
            }
            Assert::IsTrue(usedByA == std::unordered_set<std::string>{"a", "b", "c"});

            std::unordered_set<std::string> usedByB;
            for (int i : pkb->getUsedByProcedure(b)) {
                usedByB.insert(pkb->getVariableRepresentation(i));
            }
            Assert::IsTrue(usedByB == std::unordered_set<std::string>{"a", "b", "c"});
            std::unordered_set<std::string> usedByCallB;
            for (int i : pkb->getUsedByStmt(4)) {
                usedByCallB.insert(pkb->getVariableRepresentation(i));
            }
            Assert::IsTrue(usedByCallB == std::unordered_set<std::string>{"a", "b", "c"});

            std::unordered_set<std::string> usedByC;
            for (int i : pkb->getUsedByProcedure(c)) {
                usedByC.insert(pkb->getVariableRepresentation(i));
            }
            Assert::IsTrue(usedByC == std::unordered_set<std::string>{"b", "c"});
            std::unordered_set<std::string> usedByCallC;
            for (int i : pkb->getUsedByStmt(6)) {
                usedByCallC.insert(pkb->getVariableRepresentation(i));
            }
            Assert::IsTrue(usedByCallC == std::unordered_set<std::string>{"b", "c"});

            std::unordered_set<std::string> usedByD;
            for (int i : pkb->getUsedByProcedure(d)) {
                usedByD.insert(pkb->getVariableRepresentation(i));
            }
            Assert::IsTrue(usedByD == std::unordered_set<std::string>{"c"});
            std::unordered_set<std::string> usedByCallD;
            for (int i : pkb->getUsedByStmt(2)) {
                usedByCallD.insert(pkb->getVariableRepresentation(i));
            }
            Assert::IsTrue(usedByCallD == std::unordered_set<std::string>{"c"});

            PKB::clearPKB();
            SourceParser::clear();
        }
	};
}
