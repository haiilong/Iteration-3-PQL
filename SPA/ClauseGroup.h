#pragma once

#include <iostream>
#include <vector>
#include "Clause.h"

using namespace std;

class ClauseGroup {

public:
    ClauseGroup();
    ~ClauseGroup();

    void initialize(vector<Clause> clauses);
    void sort();
private:
    vector<Clause> clauses;
};