#ifndef TYPES_H
#define TYPES_H

#include "bu/string.h"
#include "bu/list.h"
#include "bu/hash.h"

typedef Bu::List<Bu::String> StrList;

class Variable;
typedef Bu::List<Variable> VarList;
typedef Bu::Hash<Bu::String, Variable> VarHash;

class Condition;

class Target;
typedef Bu::List<Target *> TargetList;
class Profile;
typedef Bu::Hash<Bu::String, Profile *> ProfileHash;

class AstNode;
class AstBranch;
class AstLeaf;
typedef Bu::List<const AstBranch *> AstBranchList;

#endif
