#ifndef STRING_PROC_H
#define STRING_PROC_H

#include <stdint.h>
#include <string>
#include <map>

class Build;

typedef std::map<std::string,std::string> VarMap;

class StringProc
{
public:
	StringProc( Build *pBld );
	virtual ~StringProc();

	virtual std::string replVars( const std::string &sSrc, const std::string &sCont, VarMap *mExtra )=0;

protected:
	Build *getBuild()
	{
		return pBld;
	}

private:
	Build *pBld;

};

#endif
