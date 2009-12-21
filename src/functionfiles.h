#ifndef FUNCTION_FILES_H
#define FUNCTION_FILES_H

#include "function.h"

class FunctionFiles : public Function
{
public:
	FunctionFiles();
	virtual ~FunctionFiles();

	virtual Bu::FString getName() const;
	virtual Variable call( Variable &input, VarList lParams );

};

#endif
