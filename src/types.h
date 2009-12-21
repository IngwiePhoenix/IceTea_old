#ifndef TYPES_H
#define TYPES_H

#include "bu/fstring.h"
#include "bu/list.h"
#include "bu/hash.h"

typedef Bu::List<Bu::FString> StrList;

class Variable;
typedef Bu::List<Variable> VarList;
typedef Bu::Hash<Bu::FString, Variable> VarHash;

class Condition;

class Target;
typedef Bu::List<Target *> TargetList;
class Profile;
typedef Bu::Hash<Bu::FString, Profile *> ProfileHash;

class AstNode;
class AstBranch;
class AstLeaf;
typedef Bu::List<const AstBranch *> AstBranchList;

#endif
