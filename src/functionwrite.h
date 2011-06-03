#ifndef FUNCTION_WRITE_H
#define FUNCTION_WRITE_H

#include "function.h"

class FunctionWrite : public Function
{
public:
	FunctionWrite();
	virtual ~FunctionWrite();

	virtual Bu::String getName() const;
	virtual Variable call( Variable &input, VarList lParams );

};

#endif
