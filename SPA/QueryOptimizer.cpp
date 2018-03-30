#include "QueryOptimizer.h"

QueryOptimizer::QueryOptimizer() {}

QueryOptimizer::~QueryOptimizer() {}

QueryObject QueryOptimizer::optimize(QueryObject qo) {
    queryObject = qo;

    ClauseGroup booleanGroup;
    booleanGroup.initialize(queryObject.getBooleanClauses());
    booleanGroup.sort();

    unordered_set<string> allSynonyms = queryObject.getAllSynonyms();
    UnionFind uf(allSynonyms);




    return queryObject;
}
