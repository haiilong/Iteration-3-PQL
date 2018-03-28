#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "Pair.h"

using namespace std;

class ResultTable {
    
public:
    // public API for QueryEvaluator
    void mergeOneSynonym(const string &synonym, const unordered_set<int> &synonymList);
    void mergeTwoSynonym(const string &firstSynonym, const string &secondSynonym, const unordered_set<pair<int, int>, pair_hash> &synonymList);

    list<int> getColumn(const string &synonym);
    bool noResults();
	void clear();
private:
    // class variables
    unordered_map<string, int> synToId;
    list<vector<int>> results;

    // five cases of merging
    void mergeOneExistingSynonym(const int &index, const unordered_set<int> &synonymList);
    void mergeTwoExistingSynonym(const int &firstIndex, const int &secondIndex, const unordered_set<pair<int, int>, pair_hash> &synonymList);
    void mergeOneNewSynonym(const unordered_set<int> &synonymList);
    void mergeTwoNewSynonym(const unordered_set<pair<int, int>, pair_hash> &synonymList);
    void mergeExistingAndNew(const int &index, const unordered_map<int, unordered_set<int>> &hashJoinMap);
};
