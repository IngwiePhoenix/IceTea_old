#ifndef FUNCTION_MATCHES_H
#define FUNCTION_MATCHES_H

#include "function.h"

class FunctionMatches : public Function
{
public:
	FunctionMatches();
	virtual ~FunctionMatches();

	/**
	 * Really basic globbing function, it doesn't handle character classes,
	 * just * and ?.  We can expand on it later, it may be handy.
	 */
	bool globcmp( const Bu::FString &sTxt, const Bu::FString &sMatches );
	bool matchlist( const Bu::FString &sTxt, VarList &lParams );
	virtual Bu::FString getName() const;
	virtual Variable call( Variable &input, VarList lParams );

};

#endif
