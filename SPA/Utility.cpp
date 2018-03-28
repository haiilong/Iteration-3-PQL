#include "Utility.h"
using namespace std;

bool Utility::contains(vector<int> v, int i) {
	for (auto it = v.begin(); it != v.end(); ++it) {
		if (*it == i) {
			return true;
		}
	}
	return false;
}
