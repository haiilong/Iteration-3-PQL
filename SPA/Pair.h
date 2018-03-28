#pragma once
#include <iostream>
#include <utility>

struct pair_hash {
	inline size_t operator()(const std::pair<int, int> &v) const {
		return v.first * 31 + v.second;
	}
};

