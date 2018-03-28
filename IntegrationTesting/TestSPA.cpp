#include "stdafx.h"
#include <experimental/filesystem>

#include "SPA.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestSPA) {
public:
	TEST_METHOD(spaTest1) {
		SPA spa;
		std::list<std::string> results;

		spa.parse("../IntegrationTesting/source_1a.txt");
		spa.evaluate("stmt s1, s2; Select s2 such that Follows(s1, s2)", results);
		Assert::IsTrue(results == std::list<std::string>{
			"2", "3", "6", "7", "8", "10", "11", "12", "16"
		});
	};
	TEST_METHOD(spaTest2) {
		SPA spa;
		std::list<std::string> results;

		spa.parse("../IntegrationTesting/source_1a.txt");
		spa.evaluate("stmt s; variable v; Select v such that Uses(s, \"x\")", results);
		Assert::IsTrue(results == std::list<std::string>{
			"x", "y", "i", "ddd", "z", "w"
		});
	};
	TEST_METHOD(spaTest3) {
		SPA spa;
		std::list<std::string> results;

		spa.parse("../IntegrationTesting/source_1a.txt");
		spa.evaluate("stmt s; while w; Select w such that Follows(s,w)", results);
		Assert::IsTrue(results == std::list<std::string>{
			"8", "12"
		});
	};
	TEST_METHOD(spaTest4) {
		SPA spa;
		std::list<std::string> results;

		spa.parse("../IntegrationTesting/source_1a.txt");
		spa.evaluate("assign a; while w; Select a such that Follows(w,a)", results);
		Assert::IsTrue(results == std::list<std::string>{
			"10"
		});
	};
	TEST_METHOD(spaTest5) {
		SPA spa;
		std::list<std::string> results;

		spa.parse("../IntegrationTesting/source_1a.txt");
		spa.evaluate("prog_line n1; Select n1 such that Follows*(6,n1)", results);
		Assert::IsTrue(results == std::list<std::string>{
			"7", "8", "10", "11", "12", "16"
		});
	};
	TEST_METHOD(spaTest6) {
		SPA spa;
		std::list<std::string> results;

		spa.parse("../IntegrationTesting/source_1a.txt");
		spa.evaluate("stmt s1, s2; assign a; Select s1 such that Follows(s1,s2) pattern a(_,_)", results);
		for (auto &s : results) {
			Logger::WriteMessage(s.c_str());
		}
		Assert::IsTrue(results == std::list<std::string>{
			"1", "2", "3", "6", "7", "8", "10", "11", "12"
		});
	};
	TEST_METHOD(spaTest7) {
		SPA spa;
		std::list<std::string> results;

		spa.parse("../IntegrationTesting/source_1a.txt");
		spa.evaluate("assign a; variable v; while w; Select v such that Parent(w,a) pattern a(v,_)", results);
		Assert::IsTrue(results == std::list<std::string>{
			"ddd", "w"
		});
	};
	TEST_METHOD(spaTest8) {
		SPA spa;
		std::list<std::string> results;

		spa.parse("../IntegrationTesting/source_1a.txt");
		spa.evaluate("assign a; stmt s; Select a such that Follows*(s,a) pattern a(_,_\"2\"_)", results);
		for (auto &s : results) {
			Logger::WriteMessage(s.c_str());
		}
		Assert::IsTrue(results == std::list<std::string>{
			"11"
		});
	};
	TEST_METHOD(spaTest9) {
		SPA spa;
		std::list<std::string> results;

		spa.parse("../IntegrationTesting/source_1a.txt");
		spa.evaluate("assign a; while w; Select a such that Follows*(w,a) pattern a(_,_\"ddd\"_)", results);
		for (auto &s : results) {
			Logger::WriteMessage(s.c_str());
		}
		Assert::IsTrue(results == std::list<std::string>{
			"11"
		});
	};
	TEST_METHOD(spaTest10) {
		SPA spa;
		std::list<std::string> results;

		spa.parse("../IntegrationTesting/source_1a.txt");
		spa.evaluate("stmt s; variable v; assign a; Select a such that Modifies(s,v) pattern a(v,_)", results);
		Assert::IsTrue(results == std::list<std::string>{
			"1", "2", "4", "5", "6", "7", "9", "10", "11", "14", "15", "18", "20"
		});
	};
	TEST_METHOD(spaTest11) {
		SPA spa;
		std::list<std::string> results;

		spa.parse("../IntegrationTesting/source_1a.txt");
		spa.evaluate("assign a; variable v; Select a such that Modifies(a,v) pattern a(v,_\"123\"_)", results);
		for (auto &s : results) {
			Logger::WriteMessage(s.c_str());
		}
		Assert::IsTrue(results == std::list<std::string>{
			"1", "6"
		});
	};
	TEST_METHOD(spaTest12) {
		SPA spa;
		std::list<std::string> results;

		spa.parse("../IntegrationTesting/source_1a.txt");
		spa.evaluate("assign a; variable v; Select a such that Modifies(a,v) pattern a(v,_)", results);
		for (auto &s : results) {
			Logger::WriteMessage(s.c_str());
		}
		Assert::IsTrue(results == std::list<std::string>{
			"1", "2", "4", "5", "6", "7", "9", "10", "11", "14", "15", "18", "20"
		});
	};
	TEST_METHOD(spaTest13) {
		SPA spa;
		std::list<std::string> results;

		spa.parse("../IntegrationTesting/source_nested.txt");
		spa.evaluate("assign a; variable v; Select a such that Uses(a,v) pattern a(v,_)", results);
		for (auto &s : results) {
			Logger::WriteMessage(s.c_str());
		}
		Assert::IsTrue(results == std::list<std::string>{
			"24"
		});
	};
private:
	};
};
