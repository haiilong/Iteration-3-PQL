#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/SourceParser.h"
#include "PKBSimulator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
    TEST_CLASS(TestSourceParser) {
    public:
        TEST_METHOD(TestParsesSourceWithSimpleAssignVariable) {
            SourceParser::clear();
            SourceCode source = string("procedure First { x = y; }");
            PKBSimulator result = PKBSimulator();
            SourceParser::getInstance(result)->parse(source);
            
            PKBSimulator expected = PKBSimulator();
            expected.insertProc(string("First"));
            expected.insertVar(string("x"));
            expected.insertVar(string("y"));
            expected.insertStmt(1, string("ASSIGN"), 1);
            expected.insertPatternExpression(1, " y ");
            
            bool res = result == expected;
            Assert::IsTrue(res);
            SourceParser::clear();
        }


        TEST_METHOD(TestParsesSourceWithSimpleAssignConstant) {
            SourceParser::clear();
            SourceCode source = string("procedure First { x = 1; }");
            PKBSimulator result = PKBSimulator();
            SourceParser::getInstance(result)->parse(source);

            PKBSimulator expected = PKBSimulator();
            expected.insertProc(string("First"));
            expected.insertVar(string("x"));
            expected.insertConst(1);
            expected.insertStmt(1, string("ASSIGN"), 1);
            expected.insertPatternExpression(1, string(" 1 "));

            bool res = result == expected;
            Assert::IsTrue(res);
            SourceParser::clear();
        }

        TEST_METHOD(TestParsesSourceWithSimpleIfStatement) {
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
            PKBSimulator result = PKBSimulator();
            SourceParser::getInstance(result)->parse(source);

            PKBSimulator expected = PKBSimulator();
            expected.insertProc(string("First"));
            expected.insertVar(string("x"));
            expected.insertVar(string("y"));
            expected.insertVar(string("z"));
            expected.insertVar(string("a"));
            expected.insertVar(string("b"));
            expected.insertVar(string("c"));
            expected.insertStmt(1, string("ASSIGN"), 1);
            expected.insertStmt(2, string("IF"), 1);
            expected.insertStmt(3, string("ASSIGN"), 1);
            expected.insertStmt(4, string("ASSIGN"), 1);
            expected.setControlVariable(2, 1);
            expected.insertPatternExpression(1, " c ");
            expected.insertPatternExpression(3, " b ");
            expected.insertPatternExpression(4, " a ");
            expected.setParent(2, 3);
            expected.setParent(2, 4);
            expected.setFollows(1, 2);
            expected.setNext(1, 2);
            expected.setNext(2, 3);
            expected.setNext(2, 4);

            bool res = result == expected;
            Assert::IsTrue(res);
            SourceParser::clear();
        }

        TEST_METHOD(TestParsesSourceWithSimpleWhileStatement) {
            SourceParser::clear();
            SourceCode source = R"(
                procedure First { 
                    x = c;
                    while x {
                        z = b;
                        a = y;
                    } 
                }
            )";
            PKBSimulator result = PKBSimulator();
            SourceParser::getInstance(result)->parse(source);

            PKBSimulator expected = PKBSimulator();
            expected.insertProc(string("First"));
            expected.insertVar(string("x"));
            expected.insertVar(string("y"));
            expected.insertVar(string("z"));
            expected.insertVar(string("a"));
            expected.insertVar(string("b"));
            expected.insertVar(string("c"));
            expected.insertStmt(1, string("ASSIGN"), 1);
            expected.insertStmt(2, string("WHILE"), 1);
            expected.insertStmt(3, string("ASSIGN"), 1);
            expected.insertStmt(4, string("ASSIGN"), 1);
            expected.insertPatternExpression(1, " c ");
            expected.insertPatternExpression(3, " b ");
            expected.insertPatternExpression(4, " y ");
            expected.setParent(2, 3);
            expected.setParent(2, 4);
            expected.setFollows(1, 2);
            expected.setFollows(3, 4);
            expected.setNext(1, 2);
            expected.setNext(2, 3);
            expected.setNext(3, 4);
            expected.setNext(4, 2);

            bool res = result == expected;
            Assert::IsTrue(res);
            SourceParser::clear();
        }


        TEST_METHOD(TestSetsNextForComplicatedNesting) {
            SourceParser::clear();
            SourceCode source = R"(
                procedure First { 
                    while x {
                        if x then {
                            if x then { 
                                a= a;
                            } else { while x {
                                a=a;}}
                        } else { a=b;
                        }
                    }
                }
            )";
            PKBSimulator result = PKBSimulator();
            SourceParser::getInstance(result)->parse(source);

            PKBSimulator expected = PKBSimulator();
            expected.setNext(1, 2);
            expected.setNext(2, 3);
            expected.setNext(3, 4);
            expected.setNext(3, 5);
            expected.setNext(5, 6);
            expected.setNext(6, 5);
            expected.setNext(2, 7);
            expected.setNext(4, 1);
            expected.setNext(5, 1);
            expected.setNext(7, 1);

            Assert::IsTrue(result.nextCommands == expected.nextCommands);
            SourceParser::clear();
        }

        TEST_METHOD(TestParsesSourceWithSimplePattern) {
            SourceParser::clear();
            SourceCode source = R"(
                procedure First { 
                    x = a + 1 * 2; 
                }
            )";
            PKBSimulator result = PKBSimulator();
            SourceParser::getInstance(result)->parse(source);

            PKBSimulator expected = PKBSimulator();
            expected.insertProc(string("First"));
            expected.insertVar(string("x"));
            expected.insertVar(string("a"));
            expected.insertConst(1);
            expected.insertConst(2);
            expected.insertStmt(1, string("ASSIGN"), 1);
            expected.insertPatternExpression(1, " a 1 2 * + ");

            bool res = result == expected;
            Assert::IsTrue(res);
            SourceParser::clear();
        }

        TEST_METHOD(TestParsesSourceWithNestedIf) {
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
            PKBSimulator result = PKBSimulator();
            SourceParser::getInstance(result)->parse(source);
            
            PKBSimulator expected = PKBSimulator();
            expected.insertProc(string("First"));
            expected.insertVar(string("x"));
            expected.insertVar(string("y"));
            expected.insertVar(string("z"));
            expected.insertVar(string("a"));
            expected.insertVar(string("b"));
            expected.insertVar(string("c"));
            expected.insertVar(string("i"));
            expected.insertStmt(1, string("ASSIGN"), 1);
            expected.insertStmt(2, string("WHILE"), 1);
            expected.insertStmt(3, string("IF"), 1);
            expected.insertStmt(4, string("ASSIGN"), 1);
            expected.insertStmt(5, string("ASSIGN"), 1);
            expected.insertPatternExpression(1, " c ");
            expected.insertPatternExpression(4, " b ");
            expected.insertPatternExpression(5, " a ");
            expected.setParent(2, 3);
            expected.setParent(3, 4);
            expected.setParent(3, 5);
            expected.setFollows(1, 2);
            expected.setNext(1, 2);
            expected.setNext(2, 3);
            expected.setNext(3, 4);
            expected.setNext(3, 5);
            expected.setNext(4, 2);
            expected.setNext(5, 2);

            bool res = result == expected;
            Assert::IsTrue(res);
            SourceParser::clear();
        }

        TEST_METHOD(TestParsesSourceWithNestedWhile) {
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
            PKBSimulator result = PKBSimulator();
            SourceParser::getInstance(result)->parse(source);

            PKBSimulator expected = PKBSimulator();
            expected.insertProc(string("First"));
            expected.insertVar(string("x"));
            expected.insertVar(string("y"));
            expected.insertVar(string("z"));
            expected.insertVar(string("a"));
            expected.insertVar(string("b"));
            expected.insertVar(string("c"));
            expected.insertVar(string("i"));
            expected.insertStmt(1, string("ASSIGN"), 1);
            expected.insertStmt(2, string("IF"), 1);
            expected.insertStmt(3, string("WHILE"), 1);
            expected.insertStmt(4, string("ASSIGN"), 1);
            expected.insertStmt(5, string("ASSIGN"), 1);
            expected.insertPatternExpression(1, " c ");
            expected.insertPatternExpression(4, " b ");
            expected.insertPatternExpression(5, " a ");
            expected.setParent(2, 3);
            expected.setParent(2, 5);
            expected.setParent(3, 4);
            expected.setFollows(1, 2);
            expected.setNext(1, 2);
            expected.setNext(2, 3);
            expected.setNext(3, 4);
            expected.setNext(4, 3);
            expected.setNext(2, 5);

            bool res = result == expected;
            Assert::IsTrue(res);
            SourceParser::clear();
        }

        TEST_METHOD(TestParsesSourceWithWhileAndIf) {
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
            PKBSimulator result = PKBSimulator();
            SourceParser::getInstance(result)->parse(source);

            PKBSimulator expected = PKBSimulator();
            expected.insertProc(string("First"));
            expected.insertVar(string("x"));
            expected.insertVar(string("y"));
            expected.insertVar(string("z"));
            expected.insertVar(string("a"));
            expected.insertVar(string("b"));
            expected.insertVar(string("c"));
            expected.insertVar(string("i"));
            expected.insertStmt(1, string("ASSIGN"), 1);
            expected.insertStmt(2, string("IF"), 1);
            expected.insertStmt(5, string("WHILE"), 1);
            expected.insertStmt(3, string("ASSIGN"), 1);
            expected.insertStmt(4, string("ASSIGN"), 1);
            expected.insertStmt(6, string("ASSIGN"), 1);
            expected.insertPatternExpression(1, " c ");
            expected.insertPatternExpression(3, " c ");
            expected.insertPatternExpression(6, " b ");
            expected.insertPatternExpression(4, " a ");
            expected.setParent(2, 3);
            expected.setParent(2, 4);
            expected.setParent(5, 6);
            expected.setFollows(1, 2);
            expected.setFollows(2, 5);
            expected.setNext(1, 2);
            expected.setNext(2, 3);
            expected.setNext(2, 4);
            expected.setNext(3, 5);
            expected.setNext(4, 5);
            expected.setNext(5, 6);
            expected.setNext(6, 5);

            bool res = result == expected;
            Assert::IsTrue(res);
            SourceParser::clear();
        }

        TEST_METHOD(TestParsesSourceWithWhileMultipleStatement) {
            SourceParser::clear();
            SourceCode source = R"(
                procedure First { 
                    while i {
                        x = a;
                        x = b;
                    }
                    x = a;
                }
            )";
            PKBSimulator result = PKBSimulator();
            SourceParser::getInstance(result)->parse(source);

            PKBSimulator expected = PKBSimulator();
            expected.insertProc(string("First"));
            expected.insertVar(string("x"));
            expected.insertVar(string("a"));
            expected.insertVar(string("b"));
            expected.insertVar(string("i"));
            expected.insertStmt(1, string("WHILE"), 1);
            expected.insertStmt(2, string("ASSIGN"), 1);
            expected.insertStmt(3, string("ASSIGN"), 1);
            expected.insertStmt(4, string("ASSIGN"), 1);
            expected.insertPatternExpression(2, " a ");
            expected.insertPatternExpression(3, " b ");
            expected.insertPatternExpression(4, " a ");
            expected.setParent(1, 3);
            expected.setParent(1, 2);
            expected.setFollows(2, 3);
            expected.setFollows(1, 4);
            expected.setNext(1, 2);
            expected.setNext(2, 3);
            expected.setNext(3, 1);
            expected.setNext(1, 4);

            bool res = result == expected;
            Assert::IsTrue(res);
            SourceParser::clear();
        }

        TEST_METHOD(TestParsesSourceWithIfMultipleStatement) {
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
                    x = a;
                }
            )";
            PKBSimulator result = PKBSimulator();
            SourceParser::getInstance(result)->parse(source);

            PKBSimulator expected = PKBSimulator();
            expected.insertProc(string("First"));
            expected.insertVar(string("x"));
            expected.insertVar(string("a"));
            expected.insertVar(string("b"));
            expected.insertVar(string("i"));
            expected.insertStmt(1, string("IF"), 1);
            expected.insertStmt(2, string("ASSIGN"), 1);
            expected.insertStmt(3, string("ASSIGN"), 1);
            expected.insertStmt(4, string("ASSIGN"), 1);
            expected.insertStmt(5, string("ASSIGN"), 1);
            expected.insertStmt(6, string("ASSIGN"), 1);
            expected.insertPatternExpression(2, " a ");
            expected.insertPatternExpression(3, " b ");
            expected.insertPatternExpression(5, " a ");
            expected.insertPatternExpression(4, " b ");
            expected.insertPatternExpression(6, " a ");
            expected.setParent(1, 2);
            expected.setParent(1, 3);
            expected.setParent(1, 4);
            expected.setParent(1, 5);
            expected.setFollows(2, 3);
            expected.setFollows(4, 5);
            expected.setFollows(1, 6);
            expected.setNext(1, 2);
            expected.setNext(2, 3);
            expected.setNext(3, 6);
            expected.setNext(1, 4);
            expected.setNext(4, 5);
            expected.setNext(5, 6);

            bool res = result == expected;
            Assert::IsTrue(res);
            SourceParser::clear();
        }

        TEST_METHOD(TestParsesSourceWithWhileAndIfArbitraryNesting) {
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
            PKBSimulator result = PKBSimulator();
            SourceParser::getInstance(result)->parse(source);

            PKBSimulator expected = PKBSimulator();
            expected.insertProc(string("First"));
            expected.insertVar(string("x"));
            expected.insertVar(string("a"));
            expected.insertVar(string("b"));
            expected.insertVar(string("c"));
            expected.insertVar(string("d"));
            expected.insertVar(string("i"));
            expected.insertStmt(1, string("IF"), 1);
            expected.insertStmt(2, string("WHILE"), 1);
            expected.insertStmt(3, string("IF"), 1);
            expected.insertStmt(4, string("ASSIGN"), 1);
            expected.insertStmt(5, string("ASSIGN"), 1);
            expected.insertStmt(6, string("ASSIGN"), 1);
            expected.insertStmt(7, string("WHILE"), 1);
            expected.insertStmt(8, string("ASSIGN"), 1);
            expected.insertPatternExpression(4, " a ");
            expected.insertPatternExpression(5, " b ");
            expected.insertPatternExpression(6, " c ");
            expected.insertPatternExpression(8, " d ");
            expected.setParent(1, 2);
            expected.setParent(1, 7);
            expected.setParent(2, 3);
            expected.setParent(3, 4);
            expected.setParent(3, 5);
            expected.setParent(3, 6);
            expected.setParent(7, 8);
            expected.setNext(1, 2);
            expected.setNext(2, 3);
            expected.setNext(3, 4);
            expected.setNext(4, 5);
            expected.setNext(3, 6);
            expected.setNext(5, 2);
            expected.setNext(6, 2);
            expected.setNext(1, 7);
            expected.setNext(7, 8);
            expected.setNext(8, 7);

            expected.setFollows(4, 5);

            bool res = result == expected;
            Assert::IsTrue(res);
            SourceParser::clear();
        }

        TEST_METHOD(TestInvalidProcedure) {
            SourceCode source;
            // invalid procedure name
            SourceParser::clear(); 
            source = R"(
                procedure 3ain { 
                    a = b;
                }
            )";
            PKBSimulator result = PKBSimulator();
            try {
                SourceParser::getInstance(result)->parse(source);
                Assert::Fail();
            } catch(BadTokenException& e) {
                Logger::WriteMessage(e.what());
            }
            SourceParser::clear();

            // invalid procedure token
            SourceParser::clear();
            source = R"(
                proce 3ain { 
                    a = b;
                }
            )";
            try {
                SourceParser::getInstance(result)->parse(source);
                Assert::Fail();
            }
            catch(BadTokenException& e) {
                Logger::WriteMessage(e.what());
            }
            SourceParser::clear();


            // no open brace for procedure
            SourceParser::clear();
            source = R"(
                procedure main  
                    a = x;
                }
            )";
            try {
                SourceParser::getInstance(result)->parse(source);
                Assert::Fail();
            }
            catch(BadTokenException& e) {
                Logger::WriteMessage(e.what());
            }
            SourceParser::clear();

            // no close brace for procedure
            SourceParser::clear();
            source = R"(
                procedure main {
                    a = x;
            )";
            try {
                SourceParser::getInstance(result)->parse(source);
                Assert::Fail();
            }
            catch(BadTokenException& e) {
                Logger::WriteMessage(e.what());
            }
            SourceParser::clear();

            // additional close brace for procedure
            SourceParser::clear();
            source = R"(
                procedure main {
                    a = x;
                }}
            )";
            try {
                SourceParser::getInstance(result)->parse(source);
                Assert::Fail();
            }
            catch(BadTokenException& e) {
                Logger::WriteMessage(e.what());
            }
            SourceParser::clear();


            // no statement list for procedure
            SourceParser::clear();
            source = R"(
                procedure main {}
            )";
            try {
                SourceParser::getInstance(result)->parse(source);
                Assert::Fail();
            }
            catch(EmptyStatementListException& e) {
                Logger::WriteMessage(e.what());
            }
            SourceParser::clear();
        }
        
        TEST_METHOD(TestInvalidAssign) {
            SourceCode source;
            // bad var name on LHS
            SourceParser::clear();
            source = R"(
                procedure main { 
                    1a = 1;
                }
            )";
            PKBSimulator result = PKBSimulator();
            try {
                SourceParser::getInstance(result)->parse(source);
                Assert::Fail();
            }
            catch(BadTokenException& e) {
                Logger::WriteMessage(e.what());
            }
            SourceParser::clear();

            // empty LHS
            SourceParser::clear();
            source = R"(
                procedure main { 
                    = 1;
                }
            )";
            try {
                SourceParser::getInstance(result)->parse(source);
                Assert::Fail();
            }
            catch(BadTokenException& e) {
                Logger::WriteMessage(e.what());
            }
            SourceParser::clear();

            // no equal
            SourceParser::clear();
            source = R"(
                procedure main { 
                    a  1;
                }
            )";
            try {
                SourceParser::getInstance(result)->parse(source);
                Assert::Fail();
            }
            catch(BadTokenException& e) {
                Logger::WriteMessage(e.what());
            }
            SourceParser::clear();

            // empty RHS
            SourceParser::clear();
            source = R"(
                procedure main { 
                    a = ;
                }
            )";
            try {
                SourceParser::getInstance(result)->parse(source);
                Assert::Fail();
            }
            catch(BadTokenException& e) {
                Logger::WriteMessage(e.what());
            }
            SourceParser::clear();

            // empty RHS no semicolon
            SourceParser::clear();
            source = R"(
                procedure main { 
                    a = 
                }
            )";
            try {
                SourceParser::getInstance(result)->parse(source);
                Assert::Fail();
            }
            catch(BadTokenException& e) {
                Logger::WriteMessage(e.what());
            }
            SourceParser::clear();

            // bad var name RHS
            SourceParser::clear();
            source = R"(
                procedure main { 
                    a = 21a;
                }
            )";
            try {
                SourceParser::getInstance(result)->parse(source);
                Assert::Fail();
            }
            catch(BadTokenException& e) {
                Logger::WriteMessage(e.what());
            }
            SourceParser::clear();

            // unclosed paranthesis
            SourceParser::clear();
            source = R"(
                procedure main { 
                    a = (a;
                }
            )";
            try {
                SourceParser::getInstance(result)->parse(source);
                Assert::Fail();
            }
            catch(BadTokenException& e) {
                Logger::WriteMessage(e.what());
            }
            SourceParser::clear();

            // unopened paranthesis
            SourceParser::clear();
            source = R"(
                procedure main { 
                    a = a);
                }
            )";
            try {
                SourceParser::getInstance(result)->parse(source);
                Assert::Fail();
            }
            catch(BadTokenException& e) {
                Logger::WriteMessage(e.what());
            }
            SourceParser::clear();

            // empty enclosed paranthesis
            SourceParser::clear();
            source = R"(
                procedure main { 
                    a = ();
                }
            )";
            try {
                SourceParser::getInstance(result)->parse(source);
                Assert::Fail();
            }
            catch(BadTokenException& e) {
                Logger::WriteMessage(e.what());
            }
            SourceParser::clear();

            // expression has no RHS
            SourceParser::clear();
            source = R"(
                procedure main { 
                    a = a + ;
                }
            )";
            try {
                SourceParser::getInstance(result)->parse(source);
                Assert::Fail();
            }
            catch(BadTokenException& e) {
                Logger::WriteMessage(e.what());
            }
            SourceParser::clear();


            // expression has no LHS
            SourceParser::clear();
            source = R"(
                procedure main { 
                    a = +a;
                }
            )";
            try {
                SourceParser::getInstance(result)->parse(source);
                Assert::Fail();
            }
            catch(BadTokenException& e) {
                Logger::WriteMessage(e.what());
            }
            SourceParser::clear();

            // no semicolon
            SourceParser::clear();
            source = R"(
                procedure main { 
                    a = a+b
                }
            )";
            try {
                SourceParser::getInstance(result)->parse(source);
                Assert::Fail();
            }
            catch(BadTokenException& e) {
                Logger::WriteMessage(e.what());
            }
            SourceParser::clear();
        }


        TEST_METHOD(TestInvalidSymbols) {
            SourceCode source;
            // bad var name
            SourceParser::clear();
            source = R"(
                procedure main { 
                    1a = 1;
                }
            )";
            PKBSimulator result = PKBSimulator();
            try {
                SourceParser::getInstance(result)->parse(source);
                Assert::Fail();
            }
            catch(BadTokenException& e) {
                Logger::WriteMessage(e.what());
            }
            SourceParser::clear();

            // bad constant
            SourceParser::clear();
            source = R"(
                procedure main { 
                   a = 1ah;
                }
            )";
            try {
                SourceParser::getInstance(result)->parse(source);
                Assert::Fail();
            }
            catch(BadTokenException& e) {
                Logger::WriteMessage(e.what());
            }
            SourceParser::clear();

            // invalid symbol
            SourceParser::clear();
            source = R"(
                procedure main { 
                   a = 1/2;
                }
            )";
            try {
                SourceParser::getInstance(result)->parse(source);
                Assert::Fail();
            }
            catch(BadTokenException& e) {
                Logger::WriteMessage(e.what());
            }
            SourceParser::clear();
        }

        TEST_METHOD(TestInvalidWhile) {
            SourceCode source;
            // empty while statement list
            SourceParser::clear();
            source = R"(
                procedure main { 
                    while x {}
                }
            )";
            PKBSimulator result = PKBSimulator();
            try {
                SourceParser::getInstance(result)->parse(source);
                Assert::Fail();
            }
            catch(EmptyStatementListException& e) {
                Logger::WriteMessage(e.what());
            }
            SourceParser::clear();

            // empty while token 
            SourceParser::clear();
            source = R"(
                procedure main { 
                    x {a=b;}
                }
            )";
            try {
                SourceParser::getInstance(result)->parse(source);
                Assert::Fail();
            }
            catch(BadTokenException& e) {
                Logger::WriteMessage(e.what());
            }
            SourceParser::clear();

            // bad while token 
            SourceParser::clear();
            source = R"(
                procedure main { 
                    whlie x {a=b;}
                }
            )";
            try {
                SourceParser::getInstance(result)->parse(source);
                Assert::Fail();
            }
            catch(BadTokenException& e) {
                Logger::WriteMessage(e.what());
            }
            SourceParser::clear();

            // bad control variable  
            SourceParser::clear();
            source = R"(
                procedure main { 
                    while 1x {a=b;}
                }
            )";
            try {
                SourceParser::getInstance(result)->parse(source);
                Assert::Fail();
            }
            catch(BadTokenException& e) {
                Logger::WriteMessage(e.what());
            }
            SourceParser::clear();

            // no open brace 
            SourceParser::clear();
            source = R"(
                procedure main { 
                    while x 
                        a = b;
                    }
                }
            )";
            try {
                SourceParser::getInstance(result)->parse(source);
                Assert::Fail();
            }
            catch(BadTokenException& e) {
                Logger::WriteMessage(e.what());
            }
            SourceParser::clear();

            // no close brace 
            SourceParser::clear();
            source = R"(
                procedure main { 
                    while x {
                        a = b;
                }
            )";
            try {
                SourceParser::getInstance(result)->parse(source);
                Assert::Fail();
            }
            catch(BadTokenException& e) {
                Logger::WriteMessage(e.what());
            }
            SourceParser::clear();
        }

        TEST_METHOD(TestInvalidIf) {
            SourceCode source;
            // empty then and else statement list
            SourceParser::clear();
            source = R"(
                procedure main { 
                    if x then {
                    } else {
                    }
                }
            )";
            PKBSimulator result = PKBSimulator();
            try {
                SourceParser::getInstance(result)->parse(source);
                Assert::Fail();
            }
            catch(EmptyStatementListException& e) {
                Logger::WriteMessage(e.what());
            }
            SourceParser::clear();

            // empty then statement list
            SourceParser::clear();
            source = R"(
                procedure main { 
                    if x then {
                    } else {
                        a=b;
                    }
                }
            )";
            try {
                SourceParser::getInstance(result)->parse(source);
                Assert::Fail();
            }
            catch(EmptyStatementListException& e) {
                Logger::WriteMessage(e.what());
            }
            SourceParser::clear();

            // empty else statement list
            SourceParser::clear();
            source = R"(
                procedure main { 
                    if x then {
                        a=b;
                    } else {
                    }
                }
            )";
            try {
                SourceParser::getInstance(result)->parse(source);
                Assert::Fail();
            }
            catch(EmptyStatementListException& e) {
                Logger::WriteMessage(e.what());
            }
            SourceParser::clear();

            // no if token 
            SourceParser::clear();
            source = R"(
                procedure main {                     
                    x then {
                        a=b;
                    } else {
                        a= b;
                    }
                }
            )";
            try {
                SourceParser::getInstance(result)->parse(source);
                Assert::Fail();
            }
            catch(BadTokenException& e) {
                Logger::WriteMessage(e.what());
            }
            SourceParser::clear();

            // malformed if token 
            SourceParser::clear();
            source = R"(
                procedure main {                     
                    fi x then {
                        a=b;
                    } else {
                        a= b;
                    }
                }
            )";
            try {
                SourceParser::getInstance(result)->parse(source);
                Assert::Fail();
            }
            catch(BadTokenException& e) {
                Logger::WriteMessage(e.what());
            }
            SourceParser::clear();

            // no control variable
            SourceParser::clear();
            source = R"(
                procedure main {                     
                    if then {
                        a=b;
                    } else {
                        a= b;
                    }
                }
            )";
            try {
                SourceParser::getInstance(result)->parse(source);
                Assert::Fail();
            }
            catch(BadTokenException& e) {
                Logger::WriteMessage(e.what());
            }
            SourceParser::clear();

            // bad control variable
            SourceParser::clear();
            source = R"(
                procedure main {                     
                    if 18a then {
                        a=b;
                    } else {
                        a= b;
                    }
                }
            )";
            try {
                SourceParser::getInstance(result)->parse(source);
                Assert::Fail();
            }
            catch(BadTokenException& e) {
                Logger::WriteMessage(e.what());
            }
            SourceParser::clear();

            // no then token 
            SourceParser::clear();
            source = R"(
                procedure main {                     
                    if x {
                        a=b;
                    } else {
                        a= b;
                    }
                }
            )";
            try {
                SourceParser::getInstance(result)->parse(source);
                Assert::Fail();
            }
            catch(BadTokenException& e) {
                Logger::WriteMessage(e.what());
            }
            SourceParser::clear();

            // malformed then token
            SourceParser::clear();
            source = R"(
                procedure main {                     
                    if x the {
                        a=b;
                    } else {
                        a= b;
                    }
                }
            )";
            try {
                SourceParser::getInstance(result)->parse(source);
                Assert::Fail();
            }
            catch(BadTokenException& e) {
                Logger::WriteMessage(e.what());
            }
            SourceParser::clear();

            // no then open brace
            SourceParser::clear();
            source = R"(
                procedure main {                     
                    if x then 
                        a=b;
                    } else {
                        a= b;
                    }
                }
            )";
            try {
                SourceParser::getInstance(result)->parse(source);
                Assert::Fail();
            }
            catch(BadTokenException& e) {
                Logger::WriteMessage(e.what());
            }
            SourceParser::clear();

            // no then close brace
            SourceParser::clear();
            source = R"(
                procedure main {                     
                    if x then {
                        a=b;
                    else {
                        a= b;
                    }
                }
            )";
            try {
                SourceParser::getInstance(result)->parse(source);
                Assert::Fail();
            }
            catch(BadTokenException& e) {
                Logger::WriteMessage(e.what());
            }
            SourceParser::clear();

            // no else token
            SourceParser::clear();
            source = R"(
                procedure main {                     
                    if x then {
                        a=b;
                    }  {
                        a= b;
                    }
                }
            )";
            try {
                SourceParser::getInstance(result)->parse(source);
                Assert::Fail();
            }
            catch(BadTokenException& e) {
                Logger::WriteMessage(e.what());
            }
            SourceParser::clear();


            // malformed else token
            SourceParser::clear();
            source = R"(
                procedure main {                     
                    if x then {
                        a=b;
                    } sles {
                        a= b;
                    }
                }
            )";
            try {
                SourceParser::getInstance(result)->parse(source);
                Assert::Fail();
            }
            catch(BadTokenException& e) {
                Logger::WriteMessage(e.what());
            }
            SourceParser::clear();


            // no else open brace
            SourceParser::clear();
            source = R"(
                procedure main {                     
                    if x then {
                        a=b;
                    } else 
                        a= b;
                    }
                }
            )";
            try {
                SourceParser::getInstance(result)->parse(source);
                Assert::Fail();
            }
            catch(BadTokenException& e) {
                Logger::WriteMessage(e.what());
            }
            SourceParser::clear();


            // no else close brace
            SourceParser::clear();
            source = R"(
                procedure main {                     
                    if x then {
                        a=b;
                    } else {
                        a= b;
                }
            )";
            try {
                SourceParser::getInstance(result)->parse(source);
                Assert::Fail();
            }
            catch(BadTokenException& e) {
                Logger::WriteMessage(e.what());
            }
            SourceParser::clear();
        }

        TEST_METHOD(TestInvalidNesting) {
            SourceCode source;
            // nested while does not close brace
            SourceParser::clear();
            source = R"(
                procedure main { 
                    if x then {
                        while y {
                            a=b;
                    } else {
                        a= b;
                    }
                    }
                }
            )";
            PKBSimulator result = PKBSimulator();
            try {
                SourceParser::getInstance(result)->parse(source);
                Assert::Fail();
            }
            catch(BadTokenException& e) {
                Logger::WriteMessage(e.what());
            }
            SourceParser::clear();

            // empty nested while
            SourceParser::clear();
            source = R"(
                procedure main { 
                    if x then {
                        while y {
                        }
                    } else {
                        a= b;
                    }
                }
            )";
            try {
                SourceParser::getInstance(result)->parse(source);
                Assert::Fail();
            }
            catch(EmptyStatementListException& e) {
                Logger::WriteMessage(e.what());
            }
            SourceParser::clear();

            // 3rd layer inner nested if without else statement
            SourceParser::clear();
            source = R"(
                procedure main { 
                    if x then {
                        while y {
                            if x then {
                                a=b;
                            }       
                        } else {
                            a= b;
                        }
                    } else {
                        a= b;
                    }
                }
            )";
            try {
                SourceParser::getInstance(result)->parse(source);
                Assert::Fail();
            }
            catch(BadTokenException& e) {
                Logger::WriteMessage(e.what());
            }
            SourceParser::clear();
        }
    };
}