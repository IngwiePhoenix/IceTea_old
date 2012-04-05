#ifndef FUNCTION_UNIQUE_H
#define FUNCTION_UNIQUE_H

#include "function.h"

class FunctionUnique : public Function
{
public:
	FunctionUnique();
	virtual ~FunctionUnique();

	virtual Bu::String getName() const;
	virtual Variable call( Variable &input, VarList lParams );
};

#endif
