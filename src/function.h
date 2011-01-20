#ifndef FUNCTION_H
#define FUNCTION_H

#include "bu/string.h"
#include "variable.h"

class Function
{
public:
	Function();
	virtual ~Function();

	virtual Bu::String getName() const=0;

	virtual Variable call( Variable &input, VarList lParams )=0;

	void setContext( class Context *p );

protected:
	class Context *pContext;
};

#endif
