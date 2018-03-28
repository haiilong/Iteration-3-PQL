#pragma once
#include "pkb.h"
#include "Utility.h"

class DesignExtractor
{
public:

    // populate additional tables in PKB: Follows*, Parent*, Calls*, Modifies (call), Uses (call)
    // note these are all relationships that cannot be easily populated in one parse
    static void setPostParsingRelations();

//private: 
	static int populateFollowsStarTable();
	static int populateParentStarTable();
	static int populateCallsStarTable();

	// set proc modifies relation for p1 when Calls(p1, p2) is true
	static int setProcCallModifies(int p1, int p2);
	// set proc uses relation for p1 when Calls(p1, p2) is true
	static int setProcCallUses(int p1, int p2);
	
	// set modifies relation for all call stmts and update it's *parents
	static int setCallStmtModifies();
	// set uses relation for all call stmts and update it's *parents
	static int setCallStmtUses();

    // set the trans call relation for callee
    static void setTransCalls(int callee, int current);
    // set the reverse trans call relation for called
    static void setTransReverseCalls(int called, int current);

};

