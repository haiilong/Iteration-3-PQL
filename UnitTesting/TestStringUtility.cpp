#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/StringUtility.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
    TEST_CLASS(TestStringUtility) {
public:
    TEST_METHOD(TestTokenizeWorksOnProcedure) {
        std::string source = "procedure A {";
        std::vector<std::string> result = StringUtility::tokenize(source, "{;}+=-*");

        std::vector<std::string> expected = { "procedure", "A", "{" };
        
        bool res = result == expected;
        Assert::IsTrue(res);
    }
    TEST_METHOD(TestTokenizeWorksOnProcedureWithoutSpaces) {
        std::string source = "procedure A{";
        std::vector<std::string> result = StringUtility::tokenize(source, "{;}+=-*");

        std::vector<std::string> expected = { "procedure", "A", "{" };

        bool res = result == expected;
        Assert::IsTrue(res);
    }
    TEST_METHOD(TestTokenizeWorksOnAssignment) {
        std::string source = "a = 1+ 2 *3 +x-4+ y;";
        std::vector<std::string> result = StringUtility::tokenize(source, "{;}+=-*");

        std::vector<std::string> expected = { 
            "a", "=", "1", 
            "+", "2", "*", 
            "3", "+", "x", 
            "-", "4", "+", 
            "y", ";" 
        };

        bool res = result == expected;
        Assert::IsTrue(res);
    }
    TEST_METHOD(TestTokenizeWorksOnIf) {
        std::string source = "if varNAme then{";
        std::vector<std::string> result = StringUtility::tokenize(source, "{;}+=-*");

        std::vector<std::string> expected = {
            "if", "varNAme", "then", "{"
        };

        bool res = result == expected;
        Assert::IsTrue(res);
    }
    TEST_METHOD(TestTokenizeWorksOnElse) {
        std::string source = "else{";
        std::vector<std::string> result = StringUtility::tokenize(source, "{;}+=-*");

        std::vector<std::string> expected = {
             "else", "{"
        };

        bool res = result == expected;
        Assert::IsTrue(res);
    }
    TEST_METHOD(TestTokenizeWorksOnWhile) {
        std::string source = "while i{";
        std::vector<std::string> result = StringUtility::tokenize(source, "{;}+=-*");

        std::vector<std::string> expected = {
            "while", "i", "{"
        };

        bool res = result == expected;
        Assert::IsTrue(res);
    }
    TEST_METHOD(TestTokenizeWorksOnCloseBrace) {
        std::string source = "}";
        std::vector<std::string> result = StringUtility::tokenize(source, "{;}+=-*");

        std::vector<std::string> expected = {
            "}"
        };

        bool res = result == expected;
        Assert::IsTrue(res);
    }
    TEST_METHOD(TestTokenizeWorksOnMalformedVariableNames) {
        std::string source = "a = 473a + b31/a - 34]01;";
        std::vector<std::string> result = StringUtility::tokenize(source, "{;}+=-*");

        std::vector<std::string> expected = {
            "a", "=", "473a", "+",
            "b31/a", "-", "34]01", ";"
        };

        bool res = result == expected;
        Assert::IsTrue(res);
    }
    };
}