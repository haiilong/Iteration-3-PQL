#pragma once

#include <iostream>

#include "QueryObject.h"
#include "UnionFind.h"

using namespace std;

class QueryOptimizer {

public:
    QueryOptimizer();
    ~QueryOptimizer();

    QueryObject optimize(QueryObject qo);

private:
    QueryObject queryObject;
    
};