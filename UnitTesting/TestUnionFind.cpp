#include "stdafx.h"
#include "CppUnitTest.h"
#include "UnionFind.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
    TEST_CLASS(TestValidate) {
public:

    TEST_METHOD(testUnionFind) {
        unordered_set<string> universe({ "a", "b", "c", "d", "e", "f", "m", "x", "y" });
        UnionFind uf (universe);
        Assert::IsTrue(uf.numSets() == 9);
        uf.unionSet("a", "b");
        Assert::IsTrue(uf.isSameSet("a", "b"));
        Assert::IsFalse(uf.isSameSet("c", "a"));
        uf.unionSet("c", "d");
        Assert::IsTrue(uf.isSameSet("c", "d"));
        uf.unionSet("e", "f");
        Assert::IsTrue(uf.isSameSet("e", "f"));
        uf.unionSet("m", "b");
        Assert::IsTrue(uf.isSameSet("m", "b"));
        Assert::IsTrue(uf.isSameSet("m", "a"));
        uf.unionSet("x", "y");
        Assert::IsTrue(uf.isSameSet("x", "y"));
        uf.unionSet("y", "b");
        Assert::IsTrue(uf.isSameSet("a", "x"));
        Assert::IsTrue(uf.isSameSet("a", "y"));
        Assert::IsTrue(uf.isSameSet("b", "x"));
        Assert::IsTrue(uf.isSameSet("b", "y"));
        Assert::IsTrue(uf.isSameSet("m", "x"));
        Assert::IsTrue(uf.isSameSet("m", "y"));
        uf.unionSet("c", "f");
        Assert::IsTrue(uf.isSameSet("c", "e"));
        Assert::IsTrue(uf.isSameSet("d", "f"));
        Assert::IsTrue(uf.numSets() == 2);

        vector<unordered_set<string>> res = uf.getPartitions();
        Assert::IsTrue(res.size() == 2);
        unordered_set<string> first ({"a","b","m","x","y"});
        Assert::IsTrue(res[0] == first);
        unordered_set<string> second({ "c","d","e","f" });
        Assert::IsTrue(res[1] == second);
    }

    TEST_METHOD(testUnionFindCorner) {
        unordered_set<string> universe({});
        UnionFind uf(universe);
        Assert::IsTrue(uf.numSets() == 0);

        vector<unordered_set<string>> res = uf.getPartitions();
        Assert::IsTrue(res.size() == 0);
    }
    };
}
