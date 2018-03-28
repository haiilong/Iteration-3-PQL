#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPA/VarTable.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestVarTable) {
	public:

		TEST_METHOD(TestinsertVar) {
			VarTable varTable;

			// check that the variable is inserted correctly
			std::string var1 = "x", var2 = "y";
			int varIndex1 = varTable.insertVar(var1);
			int expected1 = 1;
			Assert::AreEqual(varIndex1, expected1);

			int varIndex2 = varTable.insertVar(var2);
			int expected2 = 2;
			Assert::AreEqual(varIndex2, expected2);

			//insert duplicated variable, the method should return -1;
			int varIndex3 = varTable.insertVar(var1);
			int expected3 = 1;
			Assert::AreEqual(varIndex3, expected3);
		};

		TEST_METHOD(TestgetSize) {
			VarTable varTable;

			// check that the varTable size is computed correctly
			std::string var1 = "x", var2 = "y", var3 = "z";
			varTable.insertVar(var1);
			varTable.insertVar(var2);
			varTable.insertVar(var3);

			int size = varTable.getSize();
			int expected = 3;
			Assert::AreEqual(size, expected);
		};

		TEST_METHOD(TestgetVarName) {
			VarTable varTable;

			// check that the varTable name is returned correctly
			std::string var1 = "x", var2 = "y", var3 = "z";
			varTable.insertVar(var1);
			varTable.insertVar(var2);
			varTable.insertVar(var3);

			std::string varName = varTable.getVarName(3);
			std::string expected = "z";
			Assert::AreEqual(varName, expected);
		};

		TEST_METHOD(TestgetVarIndex) {
			VarTable varTable;

			// check that the varTable name is returned correctly
			std::string var1 = "x", var2 = "y", var3 = "z";
			varTable.insertVar(var1);
			varTable.insertVar(var2);
			varTable.insertVar(var3);

			int varIndex1 = varTable.getVarIndex(var1);
			int expected1 = 1;
			Assert::AreEqual(varIndex1, expected1);

			// for a non-existing variable, the method should return -1
			std::string var4 = "abc";
			int varIndex2 = varTable.getVarIndex(var4);
			int expected2 = -1;
			Assert::AreEqual(varIndex2, expected2);
		}
	};
}