#ifndef FUNCTION_TO_STRING_H
#define FUNCTION_TO_STRING_H

#include "function.h"

class FunctionToString : public Function
{
public:
	FunctionToString();
	virtual ~FunctionToString();

	virtual Bu::String getName() const;
	virtual Variable call( Variable &input, VarList lParams );
};

#endif
