#ifndef FUNCTION_RANGE_H
#define FUNCTION_RANGE_H

#include "function.h"

class FunctionRange : public Function
{
public:
	FunctionRange();
	virtual ~FunctionRange();

	virtual Bu::String getName() const;
	virtual Variable call( Variable &input, VarList lParams );

};

#endif
