#ifndef FUNCTION_GET_MAKE_DEPS_H
#define FUNCTION_GET_MAKE_DEPS_H

#include "function.h"

class FunctionGetMakeDeps : public Function
{
public:
	FunctionGetMakeDeps();
	virtual ~FunctionGetMakeDeps();

	virtual Bu::FString getName() const;
	virtual Variable call( Variable &input, VarList lParams );
};

#endif
