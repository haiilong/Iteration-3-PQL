#include "QueryOptimizer.h"

QueryOptimizer::QueryOptimizer() {}

QueryOptimizer::~QueryOptimizer() {}

struct SortByClauseGroupScore {
    bool operator()(ClauseGroup& x, ClauseGroup& y) const {
        return x.getScore() < y.getScore();
    }
};

QueryObject QueryOptimizer::optimize(QueryObject qo) {
    queryObject = qo;

    // boolean Group
    ClauseGroup booleanGroup;
    booleanGroup.initialize(queryObject.getBooleanClauses());
    booleanGroup.sortGroup();
    queryObject.setBooleanGroup(booleanGroup);

    // generic Group
    unordered_set<string> allSynonyms = queryObject.getAllSynonyms();
    UnionFind uf(allSynonyms);
    
    vector<Clause> allGenericClauses = queryObject.getGenericClauses();
    for (auto clause : allGenericClauses) {
        if (clause.getNoOfSyonyms() == 2) {
            uf.unionSet(clause.getSynonyms()[0], clause.getSynonyms()[1]);
        }
    }

    vector<unordered_set<string>> partitions = uf.getPartitions();
    vector<ClauseGroup> genericGroup;
    for (auto set : partitions) {
        ClauseGroup temp;
        for (auto clause : allGenericClauses) {
            if (set.find(clause.getSynonyms()[0]) != set.end()) {
                temp.addClause(clause);
            }
        }
        temp.sortGroup();
        temp.rearrangeGroup();
        genericGroup.push_back(temp);
    }
    sort(genericGroup.begin(), genericGroup.end(), SortByClauseGroupScore());;
    queryObject.setGenericGroup(genericGroup);

    // select group
    vector<string> selectSynonyms = queryObject.getSelectSynonyms();
    for (auto item : selectSynonyms) {
        if (allSynonyms.find(item) == allSynonyms.end()) {
            queryObject.addSelectGroup(item, queryObject.getType(item));
        }
    }
    
    return queryObject;
}
