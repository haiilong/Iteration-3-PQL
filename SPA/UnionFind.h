#pragma once
#include<unordered_set>
#include<unordered_map>
#include<vector>
#include<string>

using namespace std;

class UnionFind {
public:
    UnionFind(unordered_set<string> synonyms);
    ~UnionFind();

    int findSet(int i);
    bool isSameSet(string item1, string item2);
    bool isSameSet(int i, int j);
    void unionSet(string item1, string item2);
    unordered_set<int> getRoot();
    vector<unordered_set<string>> getPartitions();
    int numSets();

private:
    int N;
    unordered_set<string> synonyms;
    vector<int> _parent;
    vector<int> _rank;
    vector<int> _size;
    int setCounter;
    unordered_map<string, int> _map;
    unordered_map<int, string> _reverseMap;
};