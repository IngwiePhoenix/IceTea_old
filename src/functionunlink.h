#ifndef FUNCTION_UNLINK_H
#define FUNCTION_UNLINK_H

#include "function.h"

class FunctionUnlink : public Function
{
public:
	FunctionUnlink();
	virtual ~FunctionUnlink();

	virtual Bu::String getName() const;
	virtual Variable call( Variable &input, VarList lParams );
};

#endif
