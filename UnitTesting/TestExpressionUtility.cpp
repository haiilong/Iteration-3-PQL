#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/ExpressionUtility.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
    TEST_CLASS(TestExpressionUtility) {
public:
    TEST_METHOD(TestMatchesHaiLongCases) {
        string s1 = "a + b + c";
        string s2 = "a + ( b + c )";
        string s3 = "a * b + c";
        string s4 = "a + b * c";
        string s5 = "( a + b ) * c";
        string s6 = "a + b * c + d + e";
        string s7 = "( a + b ) * ( c + d )";

        Assert::IsFalse(ExpressionUtility::isMatched("a + b * c", "a + b")); // expect 0 (false)
        Assert::IsTrue(ExpressionUtility::isMatched("a + b * c", "b * c")); // expect 1 (true)
        Assert::IsFalse(ExpressionUtility::isMatched("a + b * c + d + e", "d + e")); // expect 0 (false)
        Assert::IsFalse(ExpressionUtility::isMatched("(a + b) * c", "(b * c)")); // expect 0 (false)
        Assert::IsTrue(ExpressionUtility::isMatched("(a + b * c * d", "b * c")); // expect 1 (true)
        Assert::IsFalse(ExpressionUtility::isMatched("(a + b * c * d", "c * d")); // expect 0 (false)
    }

	TEST_METHOD(TestValidVerify) {
		string s1 = "a + b + c";
		string s2 = "a + ( b + c )";
		string s3 = "a * b + c";
		string s4 = "a + b * c";
		string s5 = "( a + b ) * c";
		string s6 = "a + b * c + d + e";
		string s7 = "( a + b ) * ( c + d ) + ( e + f)";
		string s8 = "( a + b  *  c + d )";
		string s9 = " a * ( c + d )";
		string s10 = "( a + b ) * c";
		string s11 = "( a + ( c + d )) * e";


		Assert::IsTrue(ExpressionUtility::verify(s1));
		Assert::IsTrue(ExpressionUtility::verify(s2));
		Assert::IsTrue(ExpressionUtility::verify(s3));
		Assert::IsTrue(ExpressionUtility::verify(s4));
		Assert::IsTrue(ExpressionUtility::verify(s5));
		Assert::IsTrue(ExpressionUtility::verify(s6));
		Assert::IsTrue(ExpressionUtility::verify(s7));
		Assert::IsTrue(ExpressionUtility::verify(s8));
		Assert::IsTrue(ExpressionUtility::verify(s9));
		Assert::IsTrue(ExpressionUtility::verify(s10));
		Assert::IsTrue(ExpressionUtility::verify(s11));

	}

	TEST_METHOD(TestInvalidVerify) {
		string s1 = "a + b + ";
		string s2 = "a +  b + c )";
		string s3 = "a + ( b + c ";
		string s4 = " + b * c";
		string s5 = "( a + b  * c";
		string s6 = "a + b * c * + e";
		string s7 = "( a + ( c + d ) * e";

		Assert::IsFalse(ExpressionUtility::verify(s1));
		Assert::IsFalse(ExpressionUtility::verify(s2));
		Assert::IsFalse(ExpressionUtility::verify(s3));
		Assert::IsFalse(ExpressionUtility::verify(s4));
		Assert::IsFalse(ExpressionUtility::verify(s5));
		Assert::IsFalse(ExpressionUtility::verify(s6));
		Assert::IsFalse(ExpressionUtility::verify(s7));

	}
    };
}