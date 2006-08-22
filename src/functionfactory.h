#ifndef FUNCTION_FACTORY_H
#define FUNCTION_FACTORY_H

#include <stdint.h>

#include "plugger.h"
#include "singleton.h"
#include "function.h"

class FunctionFactory : public Plugger<Function>, public Singleton<FunctionFactory>
{
public:
	FunctionFactory();
	virtual ~FunctionFactory();

private:

};

#endif
