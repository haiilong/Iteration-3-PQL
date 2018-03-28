#include "stdafx.h"
#include "Command.h"

VarCommand::VarCommand(std::string varName, int seqNumber) {
    this->varName = varName;
    this->seqNumber = seqNumber;
}

bool VarCommand::operator==(VarCommand const & other) const {
    return varName == other.varName;
}

ProcCommand::ProcCommand(std::string procName, int seqNumber) {
    this->procName = procName;
    this->seqNumber = seqNumber;
}

bool ProcCommand::operator==(ProcCommand const & other) const {
    return procName == other.procName;
}

StmtCommand::StmtCommand(int n, std::string stmtType, int procedure, int seqNumber) {
    this->stmt = std::tuple<int, std::string>(n, stmtType);
    this->seqNumber = seqNumber;
    this->procedure = procedure;
}

bool StmtCommand::operator==(StmtCommand const & other) const {
    return stmt == other.stmt && procedure == other.procedure;
}

PatternCommand::PatternCommand(int lineNumber, std::string pattern, int seqNumber) {
    this->pattern = pattern;
    this->seqNumber = seqNumber;
    this->lineNumber = lineNumber;
}

bool PatternCommand::operator==(PatternCommand const & other) const {
    return pattern == other.pattern && lineNumber == other.lineNumber;
}

FollowsCommand::FollowsCommand(int followee, int follower, int seqNumber) {
    this->followee_follower = std::tuple<int, int>(followee, follower);
    this->seqNumber = seqNumber;
}

bool FollowsCommand::operator==(FollowsCommand const & other) const {
    return followee_follower == other.followee_follower;
}

ParentCommand::ParentCommand(int parent, int child, int seqNumber) {
    this->parent_child = std::tuple<int, int>(parent, child);
    this->seqNumber = seqNumber;
}

bool ParentCommand::operator==(ParentCommand const & other) const {
    return parent_child == other.parent_child;
}

ConstCommand::ConstCommand(int constant, int seqNumber) {
    this->constant = constant;
    this->seqNumber = seqNumber;
}

bool ConstCommand::operator==(ConstCommand const & other) const {
    return constant == other.constant;
}

ModifiesCommand::ModifiesCommand(int modifier, int modified, int seqNumber) {
    this->modifier_modified = std::tuple<int, int>(modifier, modified);
    this->seqNumber = seqNumber;
}

bool ModifiesCommand::operator==(ModifiesCommand const & other) const {
    return modifier_modified == other.modifier_modified;
}

UsedCommand::UsedCommand(int user, int usedVariable, int seqNumber) {
    this->user_used = std::tuple<int, int>(user, usedVariable);
    this->seqNumber = seqNumber;
}

bool UsedCommand::operator==(UsedCommand const & other) const {
    return user_used == other.user_used;
}

ControlCommand::ControlCommand(int s, int v) {
    this->stmt = s;
    this->v = v;
}

bool ControlCommand::operator==(ControlCommand const & other) const {
    return (stmt == other.stmt) && (v == other.v);
}

CallCommand::CallCommand(int s, int p1, int p2) {
    this->s = s;
    this->p1 = p1;
    this->p2 = p2;
}

bool CallCommand::operator==(CallCommand const & other) const {
    return s == other.s && p1 == other.p1 && p2 == other.p2;
}

NextCommand::NextCommand(int n1, int n2) {
    this->n1 = n1;
    this->n2 = n2;
}

bool NextCommand::operator==(NextCommand const & other) const {
    return n1 == other.n1 && n2 == other.n2;
}

ProcModifyCommand::ProcModifyCommand(int p, int v) {
    this->p = p;
    this->v = v;
}

bool ProcModifyCommand::operator==(ProcModifyCommand const & other) const {
    return p == other.p && v == other.v;
}

ProcUseCommand::ProcUseCommand(int p, int v) {
    this->p = p;
    this->v = v;
}

bool ProcUseCommand::operator==(ProcUseCommand const & other) const {
    return p == other.p && v == other.v;
}
