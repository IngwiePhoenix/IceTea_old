#ifndef STRING_PROC_H
#define STRING_PROC_H

#include <stdint.h>
#include <string>

class Build;

class StringProc
{
public:
	StringProc( Build *pBld );
	virtual ~StringProc();

	virtual std::string replVars( const std::string &sSrc, const std::string &sCont )=0;

protected:
	Build *getBuild()
	{
		return pBld;
	}

private:
	Build *pBld;

};

#endif
