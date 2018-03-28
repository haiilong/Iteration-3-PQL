#include "stdafx.h"
#include "PKBSimulator.h"

int PKBSimulator::insertVar(std::string varName) {
    varCommands.insert(VarCommand(varName, n));
    n++;
    return 0;
}

int PKBSimulator::insertStmt(int i, std::string type, int p) {
    stmtCommands.insert(StmtCommand(i, type, p, n));
    n++;
    return 0;
}

int PKBSimulator::insertProc(std::string name) {
    procCommands.insert(ProcCommand(name, n));
    n++;
    return 1;
}

int PKBSimulator::insertPatternExpression(int stmtNum, std::string expression) {
    patternCommands.insert(PatternCommand(stmtNum, expression, n));
    n++;
    return 0;
}

int PKBSimulator::insertConst(int c) {
    constCommands.insert(ConstCommand(1, n));
    n++;
    return 0;
}

int PKBSimulator::setControlVariable(int s, int v) {
    controlCommands.insert(ControlCommand(s, v));
    return 0;
}

int PKBSimulator::setFollows(int stmt1, int stmt2) {
    followsCommands.insert(FollowsCommand(stmt1, stmt2, n));
    n++;
    return 0;
}

int PKBSimulator::setParent(int stmt1, int stmt2) {
    parentCommands.insert(ParentCommand(stmt1, stmt2, n));
    n++;
    return 0;
}

int PKBSimulator::setModifies(int s, int vindex) {
    modifiesCommands.insert(ModifiesCommand(s, vindex, n));
    n++;
    return 0;
}

int PKBSimulator::setUses(int s, int vindex) {
    usedCommands.insert(UsedCommand(s, vindex, n));
    n++;
    return 0;
}

int PKBSimulator::setNext(int s1, int s2) {
    nextCommands.insert(NextCommand(s1, s2));
    return 0;
}

int PKBSimulator::setCalls(int s, int p1, int p2) {
    callCommands.insert(CallCommand(s, p1, p2));
    return 0;
}

int PKBSimulator::setProcModifies(int proc, int var) {
    procModifyCommands.insert(ProcModifyCommand(proc, var));
    return 0;
}

int PKBSimulator::setProcUses(int proc, int var) {
    procUseCommands.insert(ProcUseCommand(proc, var));
    return 0;
}

std::unordered_set<int> PKBSimulator::getModifiedByStmt(int s) {
    return std::unordered_set<int>();
}

std::unordered_set<int> PKBSimulator::getUsedByStmt(int s) {
    return std::unordered_set<int>();
}

PKBSimulator::PKBSimulator() {
    n = 0;
}

bool PKBSimulator::operator==(const PKBSimulator & other) const {
    return procCommands == other.procCommands &&
        varCommands == other.varCommands &&
        constCommands == other.constCommands &&
        stmtCommands == other.stmtCommands &&
        patternCommands == other.patternCommands &&
        followsCommands == other.followsCommands &&
        parentCommands == other.parentCommands &&
        nextCommands == other.nextCommands;
}

bool PKBSimulator::usesAndModifiesEqual(const PKBSimulator & other) const {
    return usedCommands == other.usedCommands &&
        modifiesCommands == other.modifiesCommands;
}