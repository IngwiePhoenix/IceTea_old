#ifndef STRING_PROC_H
#define STRING_PROC_H

#include <stdint.h>
#include <string>
#include <map>
#include <list>

class Build;

typedef std::map<std::string,std::string> VarMap;
typedef std::list<std::string> StringList;

class StringProc
{
public:
	StringProc( Build *pBld );
	virtual ~StringProc();

	virtual std::string replVars( const std::string &sSrc, const StringList *pCont, VarMap *mExtra )=0;

protected:
	Build *getBuild()
	{
		return pBld;
	}

private:
	Build *pBld;

};

#endif
