#include "UnionFind.h"

UnionFind::UnionFind(unordered_set<string> synonyms) {
    synonyms = synonyms;
    N = synonyms.size();
    int i = 0;
    for (auto item : synonyms) {
        _map[item] = i;
        _reverseMap[i] = item;
        i++;
    }

    setCounter = N;
    _parent.assign(N, 0);
    _rank.assign(N, 0);
    _size.assign(N, 1);
    for (int j = 0; j < N; j++) {
        _parent[j] = j;
    }
}

UnionFind::~UnionFind() {}

int UnionFind::findSet(int i) {
    return (_parent[i] == i) ? i : (_parent[i] = findSet(_parent[i]));
}

bool UnionFind::isSameSet(string item1, string item2) {
    int i = _map[item1];
    int j = _map[item2];
    return findSet(i) == findSet(j);
}

bool UnionFind::isSameSet(int i, int j) {
    return findSet(i) == findSet(j);
}

void UnionFind::unionSet(string item1, string item2) {
    int i = _map[item1];
    int j = _map[item2];
    if (!isSameSet(i, j)) {
        int x = findSet(i);
        int y = findSet(j);

        // Weighted union
        if (_rank[x] > _rank[y]) {
            _parent[y] = x;
            _size[x] += _size[y];
        }
        else {
            _parent[x] = y;
            _size[y] += _size[x];

            if (_rank[x] == _rank[y]) {
                _rank[y]++;
            }
        }
        setCounter--;
    }
}

unordered_set<int> UnionFind::getRoot() {
    unordered_set<int> res;
    for (auto i : _parent) {
        if (res.find(i) == res.end()) {
            res.insert(i);
        }
    }
    return res;
}

vector<unordered_set<string>> UnionFind::getPartitions() {
    vector<unordered_set<string>> ret;
    unordered_set<int> res;
    for (auto item : _parent) {
        int i = findSet(item);
        if (res.find(i) == res.end()) {
            res.insert(i);
        }
    }
    int x;
    for (auto root : res) {
        x = root;
        unordered_set<string> s;
        for (int i = 0; i < N; i++) {
            if (isSameSet(root, i)) {
                s.insert(_reverseMap[i]);
            }
        }
        ret.push_back(s);
    }
    return ret;
}

int UnionFind::numSets() {
    return setCounter;
}

