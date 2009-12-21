#ifndef FUNCTION_DIR_NAME_H
#define FUNCTION_DIR_NAME_H

#include "function.h"

class FunctionDirName : public Function
{
public:
	FunctionDirName();
	virtual ~FunctionDirName();

	virtual Bu::FString getName() const;
	virtual Variable call( Variable &input, VarList lParams );

};

#endif
