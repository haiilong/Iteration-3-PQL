#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include "Clause.h"

using namespace std;

class ClauseGroup {

public:
    ClauseGroup();
    ~ClauseGroup();

    void initialize(vector<Clause> clauses);
    void sortGroup();
    void rearrangeGroup();
    void addClause(Clause clause);

    int getScore();
private:
    vector<Clause> clauses;
    int score = 0;
};