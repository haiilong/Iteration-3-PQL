#pragma once
#include <vector>
#include <string>

class ConstTable {
private:
	std::vector<int> constTable;
public:
	std::vector<int> getConstTable();
	int insertConst(int c);
	int getSize();
};

