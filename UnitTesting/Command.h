#pragma once

#include <tuple>
#include <string>

class ProcUseCommand {
public:
    ProcUseCommand(int p, int v);
    bool operator==(ProcUseCommand const & other) const;
    int p;
    int v;
};

class ProcModifyCommand {
public:
    ProcModifyCommand(int p, int v);
    bool operator==(ProcModifyCommand const & other) const;
    int p;
    int v;
};

class NextCommand {
public:
    NextCommand(int n1, int n2);
    bool operator==(NextCommand const & other) const;
    int n1;
    int n2;
};

class CallCommand {
public:
    CallCommand(int s, int p1, int p2);
    bool operator==(CallCommand const & other) const;
    int s;
    int p1;
    int p2;
};

class ControlCommand {
public:
    ControlCommand(int s, int v);
    bool operator==(ControlCommand const & other) const;
    int stmt;
    int v;
};

class VarCommand {
public:
    VarCommand(std::string varName, int seqNumber);
    bool operator==(VarCommand const & other) const;
    std::string varName;
    int seqNumber;
};

class ConstCommand {
public:
    ConstCommand(int varName, int seqNumber);
    bool operator==(ConstCommand const & other) const;
    int constant;
    int seqNumber;
};

class ProcCommand {
public:
    ProcCommand(std::string procName, int seqNumber);
    bool operator==(ProcCommand const & other) const;
    std::string procName;
    int seqNumber;
};

class StmtCommand {
public:
    StmtCommand(int n, std::string stmtType, int procedure, int seqNumber);
    bool operator==(StmtCommand const & other) const;
    std::tuple<int, std::string> stmt;
    int procedure;
    int seqNumber;
};

class PatternCommand {
public:
    PatternCommand(int lineNumber, std::string pattern, int seqNumber);
    bool operator==(PatternCommand const & other) const;
    std::string pattern;
    int seqNumber;
    int lineNumber;
};

class FollowsCommand {
public:
    FollowsCommand(int followee, int follower, int seqNumber);
    bool operator==(FollowsCommand const & other) const;
    std::tuple<int, int> followee_follower;
    int seqNumber;
};

class ParentCommand {
public:
    ParentCommand(int parent, int child, int seqNumber);
    bool operator==(ParentCommand const & other) const;
    std::tuple<int, int> parent_child;
    int seqNumber;
};

class ModifiesCommand {
public:
    ModifiesCommand(int modifier, int modified, int seqNumber);
    bool operator==(ModifiesCommand const & other) const;
    std::tuple<int, int> modifier_modified;
    int seqNumber;
};

class UsedCommand {
public:
    UsedCommand(int user, int usedVariable, int seqNumber);
    bool operator==(UsedCommand const & other) const;
    std::tuple<int, int> user_used;
    int seqNumber;
};


namespace std {

    template <>
    struct hash<VarCommand> {
        std::size_t operator()(const VarCommand& k) const {
            using std::size_t;
            using std::hash;
            using std::string;

            // Compute individual hash values for first,
            // second and third and combine them using XOR
            // and bit shifting:

            return (hash<string>()(k.varName));
        }
    };

    template <>
    struct hash<ConstCommand> {
        std::size_t operator()(const ConstCommand& k) const {
            using std::size_t;
            using std::hash;
            using std::string;

            // Compute individual hash values for first,
            // second and third and combine them using XOR
            // and bit shifting:

            return 0;
        }
    };

    template <>
    struct hash<ProcCommand> {
        std::size_t operator()(const ProcCommand& k) const {
            using std::size_t;
            using std::hash;
            using std::string;

            // Compute individual hash values for first,
            // second and third and combine them using XOR
            // and bit shifting:

            return (hash<string>()(k.procName));
        }
    };

    template <>
    struct hash<StmtCommand> {
        std::size_t operator()(const StmtCommand& k) const {
            using std::size_t;
            using std::hash;
            using std::string;

            // Compute individual hash values for first,
            // second and third and combine them using XOR
            // and bit shifting:

            return 0;
        }
    };

    template <>
    struct hash<PatternCommand> {
        std::size_t operator()(const PatternCommand& k) const {
            using std::size_t;
            using std::hash;
            using std::string;

            // Compute individual hash values for first,
            // second and third and combine them using XOR
            // and bit shifting:

            return 0;
        }
    };

    template <>
    struct hash<ParentCommand> {
        std::size_t operator()(const ParentCommand& k) const {
            using std::size_t;
            using std::hash;
            using std::string;

            // Compute individual hash values for first,
            // second and third and combine them using XOR
            // and bit shifting:

            return 0;
        }
    };

    template <>
    struct hash<FollowsCommand> {
        std::size_t operator()(const FollowsCommand& k) const {
            using std::size_t;
            using std::hash;
            using std::string;

            // Compute individual hash values for first,
            // second and third and combine them using XOR
            // and bit shifting:

            return 0;
        }
    };


    template <>
    struct hash<ModifiesCommand> {
        std::size_t operator()(const ModifiesCommand& k) const {
            using std::size_t;
            using std::hash;
            using std::string;

            // Compute individual hash values for first,
            // second and third and combine them using XOR
            // and bit shifting:

            return 0;
        }
    };


    template <>
    struct hash<UsedCommand> {
        std::size_t operator()(const UsedCommand& k) const {
            using std::size_t;
            using std::hash;
            using std::string;

            // Compute individual hash values for first,
            // second and third and combine them using XOR
            // and bit shifting:

            return 0;
        }
    };

    template <>
    struct hash<ControlCommand> {
        std::size_t operator()(const ControlCommand& k) const {
            using std::size_t;
            using std::hash;
            using std::string;

            // Compute individual hash values for first,
            // second and third and combine them using XOR
            // and bit shifting:

            return 0;
        }
    };
    template <>
    struct hash<CallCommand> {
        std::size_t operator()(const CallCommand& k) const {
            using std::size_t;
            using std::hash;
            using std::string;

            // Compute individual hash values for first,
            // second and third and combine them using XOR
            // and bit shifting:

            return 0;
        }
    };
    template <>
    struct hash<ProcModifyCommand> {
        std::size_t operator()(const ProcModifyCommand& k) const {
            using std::size_t;
            using std::hash;
            using std::string;

            // Compute individual hash values for first,
            // second and third and combine them using XOR
            // and bit shifting:

            return 0;
        }
    };
    template <>
    struct hash<ProcUseCommand> {
        std::size_t operator()(const ProcUseCommand& k) const {
            using std::size_t;
            using std::hash;
            using std::string;

            // Compute individual hash values for first,
            // second and third and combine them using XOR
            // and bit shifting:

            return 0;
        }
    };
    template <>
    struct hash<NextCommand> {
        std::size_t operator()(const NextCommand& k) const {
            using std::size_t;
            using std::hash;
            using std::string;

            // Compute individual hash values for first,
            // second and third and combine them using XOR
            // and bit shifting:

            return 0;
        }
    };
}