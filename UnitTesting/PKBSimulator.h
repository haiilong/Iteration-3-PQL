#pragma once
#include "IPKB.h"
#include "Command.h"

#include <unordered_set>

class PKBSimulator :
    public IPKB {
public:
    // set the synonyms
    int insertVar(std::string varName);
    int insertStmt(int stmtNum, std::string type, int p); //stmt type refers to: ASSIGN, IF, WHILE, CALL
    int insertProc(std::string name);
    int insertPatternExpression(int stmtNum, std::string expression);
    int insertConst(int c);
    int setControlVariable(int s, int v);

    // set the relationships
    int setFollows(int stmt1, int stmt2);
    int setParent(int stmt1, int stmt2);
    int setModifies(int s, int vindex);
    int setUses(int s, int vindex);
    int setNext(int s1, int s2);
    int setCalls(int s, int p1, int p2); // stmt s in p1 calls p2
    int setProcModifies(int proc, int var);
    int setProcUses(int proc, int var);

    std::unordered_set<int> getModifiedByStmt(int s);
    std::unordered_set<int> getUsedByStmt(int s);

    // PKBSimuator
    PKBSimulator();
    bool operator==(const PKBSimulator& other) const;

    bool usesAndModifiesEqual(const PKBSimulator & other) const;

    std::unordered_set<ProcModifyCommand> procModifyCommands;
    std::unordered_set<ProcUseCommand> procUseCommands;
    std::unordered_set<NextCommand> nextCommands;
    std::unordered_set<CallCommand> callCommands;
    std::unordered_set<ControlCommand> controlCommands;
    std::unordered_set<VarCommand> varCommands;
    std::unordered_set<ConstCommand> constCommands;
    std::unordered_set<ProcCommand> procCommands;
    std::unordered_set<StmtCommand> stmtCommands;
    std::unordered_set<PatternCommand> patternCommands;
    std::unordered_set<FollowsCommand> followsCommands;
    std::unordered_set<ParentCommand> parentCommands;
    std::unordered_set<ModifiesCommand> modifiesCommands;
    std::unordered_set<UsedCommand> usedCommands;
    int n = 0;
};

