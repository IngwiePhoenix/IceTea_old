#ifndef FUNCTION_TARGETS_H
#define FUNCTION_TARGETS_H

#include "function.h"

class FunctionTargets : public Function
{
public:
	FunctionTargets();
	virtual ~FunctionTargets();

	virtual Bu::String getName() const;
	virtual Variable call( Variable &input, VarList lParams );
};

#endif
