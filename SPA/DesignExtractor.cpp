#include "DesignExtractor.h"
#include <algorithm>
using namespace std;

int DesignExtractor::populateFollowsStarTable() {
	PKB* pkb = PKB::getPKBInstance();
	vector<pair<int, int>> followsPairs = pkb->getFollowsPairsList();
	sort(followsPairs.begin(), followsPairs.end());

	for (auto it = followsPairs.begin(); it != followsPairs.end(); ++it) {
		int s1 = it->first;
		int s2 = it->second;
		
		unordered_map<int, vector<int>>::const_iterator it1 = pkb->followsStarTable.followsStarTable.find(s1);
		unordered_map<int, vector<int>>::const_iterator it2 = pkb->followsStarTable.followsStarReverseTable.find(s2);
		if (it1 == pkb->followsStarTable.followsStarTable.end() 
			&& it2 == pkb->followsStarTable.followsStarReverseTable.end()) {
			// stmts are not in the table, insert them into table
			vector<int> v1 = { s2 };
			vector<int> v2 = { s1 };
			pkb->followsStarTable.followsStarTable[s1] = v1;
			pkb->followsStarTable.followsStarReverseTable[s2] = v2;

			// update followsStarTable for the trans relation
			for (auto it = pkb->followsStarTable.followsStarTable.begin(); 
				it != pkb->followsStarTable.followsStarTable.end(); ++it) {
				if (Utility::contains(it->second, s1)) {
					it->second.push_back(s2);
				}
				if (it->first == s2) {
					pkb->followsStarTable.followsStarTable[s1].insert(
						pkb->followsStarTable.followsStarTable[s1].end(),
						it->second.begin(),
						it->second.end());
				}
			}

			// update followsReverseStarTable for the trans relation
			for (auto it = pkb->followsStarTable.followsStarReverseTable.begin(); 
				it != pkb->followsStarTable.followsStarReverseTable.end(); ++it) {
				if (it->first == s1) {
					pkb->followsStarTable.followsStarReverseTable[s2].insert(
						pkb->followsStarTable.followsStarReverseTable[s2].end(),
						it->second.begin(), 
						it->second.end()
					);
				}
				if (Utility::contains(it->second, s2)) {
					it->second.push_back(s1);
				}
			}
		}
	}
	return 1;
}

int DesignExtractor::populateParentStarTable() {
	PKB* pkb = PKB::getPKBInstance();
	vector<pair<int, int>> parentPairs = pkb->getParentPairsList();
	for (auto it = parentPairs.begin(); it != parentPairs.end(); ++it) {
		int s1 = it->first;
		int s2 = it->second;
		unordered_map<int, vector<int>>::const_iterator it1 = pkb->parentStarTable.parentStarTable.find(s1);
		unordered_map<int, vector<int>>::const_iterator it2 = pkb->parentStarTable.childrenStarTable.find(s2);
		if (it2 == pkb->parentStarTable.childrenStarTable.end()) {
			// stmt s2 are not in the table, insert it into children star table

			vector<int> v1 = { s1 };
			pkb->parentStarTable.childrenStarTable[s2] = v1;
			// check whether s1 is in the parent star table:
			if (it1 == pkb->parentStarTable.parentStarTable.end()) {
				vector<int> v2 = { s2 };
				pkb->parentStarTable.parentStarTable[s1] = v2;
			}
			else {
				// update the children vector of s1
				pkb->parentStarTable.parentStarTable[s1].push_back(s2);
			}

			// update parentStarTable for the trans relation
			for (auto it = pkb->parentStarTable.parentStarTable.begin();
				it != pkb->parentStarTable.parentStarTable.end(); ++it) {
				if (Utility::contains(it->second, s1)) {
					it->second.push_back(s2);
				}
				if (it->first == s2) {
					pkb->parentStarTable.parentStarTable[s1].insert(
						pkb->parentStarTable.parentStarTable[s1].end(),
						it->second.begin(),
						it->second.end());
				}
			}


			// update childrenStarTable for the trans relation
			for (auto it = pkb->parentStarTable.childrenStarTable.begin();
				it != pkb->parentStarTable.childrenStarTable.end(); ++it) {
				if (Utility::contains(it->second, s2)) {
					it->second.push_back(s1);
				}
				if (it->first == s1) {
					pkb->parentStarTable.childrenStarTable[s2].insert(
						pkb->parentStarTable.childrenStarTable[s2].end(),
						it->second.begin(),
						it->second.end());
				}
			}
		}
	}
	return 1;
}

// do a DFS to get all calls* relation for p
void DesignExtractor::setTransCalls(int p, int current) {
    PKB* pkb = PKB::getPKBInstance();
    for (int called : pkb->getCalledByProcedure(current)) {
        pkb->callsStarTable.callsStarTable[p].push_back(called);
        setTransCalls(p, called);
    }
}

// do a DFS to get all reverse calls* relation for p
void DesignExtractor::setTransReverseCalls(int p, int current) {
    PKB* pkb = PKB::getPKBInstance();
    for (int callee : pkb->getProcedureCalling(current)) {
        pkb->callsStarTable.callsStarReverseTable[p].push_back(callee);
        setTransReverseCalls(p, callee);
    }
}

int DesignExtractor::populateCallsStarTable() {
	PKB* pkb = PKB::getPKBInstance();

    std::unordered_set<int> procs = pkb->getAllProcedures();

    for (int p : procs) {
        pkb->callsStarTable.callsStarTable[p] = {};
        setTransCalls(p, p);
        pkb->callsStarTable.callsStarReverseTable[p] = {};
        setTransReverseCalls(p, p);
    }

	return 1;
}

void DesignExtractor::setPostParsingRelations() {
    PKB* pkb = PKB::getPKBInstance();
    // set proc call modifies and uses
    std::unordered_set<std::pair<int, int>, pair_hash> callPairs = pkb->getCallsProcedurePairs();
    for (std::pair<int, int> p : callPairs) {
        for (std::pair<int, int> p : callPairs) {
            setProcCallModifies(p.first, p.second);
            setProcCallUses(p.first, p.second);
        }
    }

	populateParentStarTable();
	populateFollowsStarTable();
	populateCallsStarTable();
    setCallStmtModifies();
    setCallStmtUses();
    
}

// set proc modifies relation for p1 when Calls(p1, p2) is true
int DesignExtractor::setProcCallModifies(int p1, int p2) {
	PKB* pkb = PKB::getPKBInstance();

	// get all var indexs that are modified by p2
	unordered_set<int> vars = pkb->getModifiedByProcedure(p2);

	// set Modifies(p1, v) for each v in the vars set
	for (auto it = vars.begin(); it != vars.end(); ++it) {
		pkb->setProcModifies(p1, *it);
	}
	return p1;
}

// set proc uses relation for p1 when Calls(p1, p2) is true
int DesignExtractor::setProcCallUses(int p1, int p2) {
	PKB* pkb = PKB::getPKBInstance();

	// get all var indexs that are used by p2
	unordered_set<int> vars = pkb->getUsedByProcedure(p2);

	// set Uses(p1, v) for each v in the vars set
	for (auto it = vars.begin(); it != vars.end(); ++it) {
		pkb->setProcUses(p1, *it);
	}
	return p1;
}

// set modifies relation for all call stmts and update it's *parents
int DesignExtractor::setCallStmtModifies() {
	PKB* pkb = PKB::getPKBInstance();
	unordered_set<int> calls = pkb->getAllCalls();
	for (auto it = calls.begin(); it != calls.end(); ++it) {
		// get the procedure index that is called in the call stmt
		int p = pkb->getProcinCallStmt(*it);
		// get all var indexs that are modified by p
		unordered_set<int> vars = pkb->getModifiedByProcedure(p);
		for (auto itVar = vars.begin(); itVar != vars.end(); ++itVar) {
			pkb->setModifies(*it, *itVar);
		}

		// update the modifies relationship for *parents
		unordered_set<int> parents = pkb->getTransParentOf(*it);
		for (auto itP = parents.begin(); itP != parents.end(); ++itP) {
			unordered_set<int> vars2 = pkb->getModifiedByStmt(*it);
			for (auto itVar2 = vars2.begin(); itVar2 != vars2.end(); ++itVar2) {
				pkb->setModifies(*itP, *itVar2);
			}
		}
	}
	return 1;
}

// set uses relation for all call stmts and update it's *parents
int DesignExtractor::setCallStmtUses() {
	PKB* pkb = PKB::getPKBInstance();
	unordered_set<int> calls = pkb->getAllCalls();
	for (auto it = calls.begin(); it != calls.end(); ++it) {
		// get the procedure index that is called in the call stmt
		int p = pkb->getProcinCallStmt(*it);
		// get all var indexs that are used by p
		unordered_set<int> vars = pkb->getUsedByProcedure(p);
		for (auto itVar = vars.begin(); itVar != vars.end(); ++itVar) {
			pkb->setUses(*it, *itVar);
		}

		// update the uses relationship for *parents
		unordered_set<int> parents = pkb->getTransParentOf(*it);
		for (auto itP = parents.begin(); itP != parents.end(); ++itP) {
			unordered_set<int> vars2 = pkb->getUsedByStmt(*it);
			for (auto itVar2 = vars2.begin(); itVar2 != vars2.end(); ++itVar2) {
				pkb->setUses(*itP, *itVar2);
			}
		}
	}
	return 1;
}


