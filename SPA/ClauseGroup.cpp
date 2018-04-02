#include "ClauseGroup.h"

ClauseGroup::ClauseGroup() {}

ClauseGroup::~ClauseGroup() {}

struct SortByClauseScore {
    bool operator()(Clause& x, Clause& y) const {
        return x.getScore() < y.getScore();
    }
};

void ClauseGroup::initialize(vector<Clause> clauses) {
    clauses = clauses;
    for (auto clause : clauses) {
        score += clause.getScore();
    }
}

void ClauseGroup::sortGroup() {
    sort(clauses.begin(), clauses.end(), SortByClauseScore());
}

void ClauseGroup::rearrangeGroup() {
    int N = clauses.size();
    vector<Clause> temp (N);
    unordered_set<string> check;
    temp.push_back(clauses.at(0));
    for (auto syn : clauses.at(0).getSynonyms()) {
        check.insert(syn);
    }

    while (clauses.size() > 0) {
        for (int i = 1; i < N; i++) {
            if (clauses.at(i).getNoOfSyonyms() == 1) {
                if (check.find(clauses.at(i).getSynonyms()[0]) != check.end()) {
                    temp.push_back(clauses.at(i));
                    temp.erase(temp.begin() + i);
                    break;
                }
            }
            else { // 2 synonyms
                if (check.find(clauses.at(i).getSynonyms()[0]) != check.end()) {
                    temp.push_back(clauses.at(i));
                    if (check.find(clauses.at(i).getSynonyms()[1]) == check.end()) {
                        check.insert(clauses.at(i).getSynonyms()[1]);
                    }
                    temp.erase(temp.begin() + i);
                    break;
                }
                else if (check.find(clauses.at(i).getSynonyms()[1]) != check.end()) {
                    temp.push_back(clauses.at(i));
                    check.insert(clauses.at(i).getSynonyms()[0]);
                    temp.erase(temp.begin() + i);
                    break;
                }
            }
        }
    }

    clauses = temp;
}

void ClauseGroup::addClause(Clause clause) {
    clauses.push_back(clause);
    score += clause.getScore();
}

int ClauseGroup::getScore() {
    return score;
}
