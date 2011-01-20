#ifndef FUNCTION_REPLACE_H
#define FUNCTION_REPLACE_H

#include "function.h"

class FunctionReplace : public Function
{
public:
	FunctionReplace();
	virtual ~FunctionReplace();

	virtual Bu::String getName() const;
	virtual Variable call( Variable &input, VarList lParams );
};

#endif
