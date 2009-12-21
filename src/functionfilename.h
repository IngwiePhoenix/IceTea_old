#ifndef FUNCTION_FILE_NAME_H
#define FUNCTION_FILE_NAME_H

#include "function.h"

class FunctionFileName : public Function
{
public:
	FunctionFileName();
	virtual ~FunctionFileName();

	virtual Bu::FString getName() const;
	virtual Variable call( Variable &input, VarList lParams );

};

#endif
