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

	virtual Bu::FString getName() const;
	virtual Variable call( Variable &input, VarList lParams );

	Bu::FString replace( Bu::RegEx &re, const Bu::FString &sSrc,
		const Bu::FString &sPat );
};

#endif
