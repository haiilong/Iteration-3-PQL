#include "ResultTable.h"

/**
 * Merge one synonym given its name and corresponding set
 */
void ResultTable::mergeOneSynonym(const string &synonym, const unordered_set<int> &synonymList) {
	if (synonymList.empty()) {
		results.clear();
		return;
	}
    if (synToId.find(synonym) != synToId.end()) {
        mergeOneExistingSynonym(synToId[synonym], synonymList);
    }
    else {
        int col = synToId.size();
        synToId[synonym] = col;
        mergeOneNewSynonym(synonymList);
    }
}

/**
 * Merge two synonyms given its name and corresponding set of pairs
 */
void ResultTable::mergeTwoSynonym(const string &firstSynonym, const string &secondSynonym, const unordered_set<pair<int, int>, pair_hash> &synonymList) {
	if (synonymList.empty()) {
		results.clear();
		return;
	}
    int col;

    if (synToId.find(firstSynonym) != synToId.end()) {
        int index = synToId[firstSynonym];

        if (synToId.find(secondSynonym) != synToId.end()) {
            mergeTwoExistingSynonym(index, synToId[secondSynonym], synonymList);
        }
        else {
            col = synToId.size();
            synToId[secondSynonym] = col;
            unordered_map<int, unordered_set<int>> hashJoinMap;

            for (const auto &item : synonymList) {
                if (hashJoinMap.find((item.first)) != hashJoinMap.end()) {
                    hashJoinMap[item.first].insert(item.second);
                }
                else {
                    unordered_set<int> temp({ item.second });
                    hashJoinMap[item.first] = temp;
                }
            }

            mergeExistingAndNew(index, hashJoinMap);
        }
    }
    else {
        col = synToId.size();
        synToId[firstSynonym] = col;

        if (synToId.find(secondSynonym) != synToId.end()) {
            unordered_map<int, unordered_set<int>> hashJoinMap;

            for (const auto &item : synonymList) {
                if (hashJoinMap.find((item.second)) != hashJoinMap.end()) {
                    hashJoinMap[item.second].insert(item.first);
                }
                else {
                    unordered_set<int> temp({ item.first });
                    hashJoinMap[item.second] = temp;
                }
            }

            mergeExistingAndNew(synToId[secondSynonym], hashJoinMap);
        }
        else {
            col = synToId.size();
            synToId[secondSynonym] = col;
            mergeTwoNewSynonym(synonymList);
        }
    }
}

/**
 * return the list of a synonym, this list has no duplicate
 */
list<int> ResultTable::getColumn(const string &synonym) {
    list<int> ret;
    unordered_set<int> retSet;

    if (synToId.find(synonym) != synToId.end()) {
        int index = synToId[synonym];

        for (const auto &row : results) {
            auto item = row[index];
            if (retSet.find(item) == retSet.end()) {
                ret.push_back(item);
                retSet.insert(item);
            }
        }
    }

    return ret;
}

/**
 * check whether the immediate result table is empty
 */
bool ResultTable::noResults(){
     return results.empty();
}

/**
 * The case where the set to be merged in belongs to one synonym that already exists.
 * We only do pruning of rows here.
 */
void ResultTable::mergeOneExistingSynonym(const int &index, const unordered_set<int> &synonymList) {
    list<vector<int>>::iterator i = results.begin();

    while (i != results.end()) {
        auto item = (*i)[index];

        if (synonymList.find(item) == synonymList.end()) {
            results.erase(i++);
        }
        else {
            ++i;
        }
    }
}

/**
 * The case where the sets to be merged in belongs to two synonyms that already exists.
 * We only do pruning of rows here.
 */
void ResultTable::mergeTwoExistingSynonym(const int &firstIndex, const int &secondIndex, const unordered_set<pair<int, int>, pair_hash> &synonymList) {
    list<vector<int>>::iterator i = results.begin();

    while (i != results.end()) {
        auto item = make_pair((*i)[firstIndex], (*i)[secondIndex]);
        if (synonymList.find(item) == synonymList.end()) {
            results.erase(i++);
        }
        else {
            ++i;
        }
    }
}

/**
 * The case where the set to be merged in belongs to one synonym that is new.
 * We only do cartesian product with this new synonym
 */
void ResultTable::mergeOneNewSynonym(const unordered_set<int> &synonymList) {
    if (results.empty()) {
        vector<int> row;

        for (const auto &item : synonymList) {
            row = { item };
            results.push_back(row);
        }
        return;
    }

    list<vector<int>>::iterator i = results.begin();

    bool first;
    while (i != results.end()) {
        first = true;

        for (const auto &item : synonymList) {
            if (first) {
                (*i).push_back(item);
            }
            else {
                results.insert(i, *i);
                (*i).back() = item;
            }
            first = false;
        }
        ++i;
    }
}

/**
 * The case where the sets to be merged in belongs to two synonyms that are new.
 * We only do cartesian product here.
 */
void ResultTable::mergeTwoNewSynonym(const unordered_set<pair<int, int>, pair_hash> &synonymList) {
    if (results.empty()) {
        vector<int> row;

        for (const auto &item : synonymList) {
            row = { item.first, item.second };
            results.push_back(row);
        }
        return;
    }

    list<vector<int>>::iterator i = results.begin();

    bool first;
    while (i != results.end()) {
        first = true;

        for (const auto &item : synonymList) {
            if (first) {
                (*i).push_back(item.first);
                (*i).push_back(item.second);
            }
            else {
                results.insert(i, *i);
                (*i).back() = item.second;
                (*i).end()[-2] = item.first;
            }
            first = false;
        }
        ++i;
    }
}

/**
 * The case where the sets to be merged in belongs to two synonyms, one new and one already existed.
 * We only do hash join operation here.
 */
void ResultTable::mergeExistingAndNew(const int &index, const unordered_map<int, unordered_set<int>> &hashJoinMap) {
    list<vector<int>>::iterator i = results.begin();
    bool first;

    while (i != results.end()) {
        auto item = (*i)[index];

        if (hashJoinMap.find(item) != hashJoinMap.end()) {
            first = true;

            for (const auto &x : hashJoinMap.at(item)) {
                if (first) {
                    (*i).push_back(x);
                }
                else {
                    results.insert(i, *i);
                    (*i).back() = x;
                }
                first = false;
            }
            ++i;
        }
        else {
            results.erase(i++);
        }
    }
}

void ResultTable::clear() {
	results.clear();
}