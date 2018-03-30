#include "stdafx.h"
#include "CppUnitTest.h"
#include "Clause.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
    TEST_CLASS(TestValidate) {
public:

    TEST_METHOD(testClauseToString) {
        Clause s;
        s.setRelation("Affects", "assign", "integer", "", "a", "5", "");
        Assert::IsTrue(s.toString() == "Affects (a, 5)");
    }
    };
}
