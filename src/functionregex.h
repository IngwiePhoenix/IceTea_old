#ifndef FUNCTION_REG_EX_H
#define FUNCTION_REG_EX_H

#include "function.h"

namespace Bu
{
	class RegEx;
}

class FunctionRegEx : public Function
{
public:
	FunctionRegEx();
	virtual ~FunctionRegEx();

	virtual Bu::String getName() const;
	virtual Variable call( Variable &input, VarList lParams );

	Bu::String replace( Bu::RegEx &re, const Bu::String &sSrc,
		const Bu::String &sPat );
};

#endif
