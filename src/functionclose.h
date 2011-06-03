#ifndef FUNCTION_CLOSE_H
#define FUNCTION_CLOSE_H

#include "function.h"

class FunctionClose : public Function
{
public:
	FunctionClose();
	virtual ~FunctionClose();

	virtual Bu::String getName() const;
	virtual Variable call( Variable &input, VarList lParams );

};

#endif
