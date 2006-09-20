#ifndef PERFORM_H
#define PERFORM_H

#include <stdint.h>
#include <list>
#include <string>
#include <map>

typedef std::map<std::string,std::string> VarMap;
typedef std::list<std::string> StringList;

class Build;

class Perform
{
public:
	Perform();
	virtual ~Perform();

	void addParam( const char *sParam );
	virtual Perform *duplicate( Build &bld, const StringList *cont, VarMap *mExtra ) = 0;
	virtual void execute( Build &bld ) = 0;
	void copyData( Perform *pSrc, Build &bld, const StringList *cont, VarMap *mExtra );
	std::string getTarget()
	{
		return sTarget;
	}
	void setTarget( std::string sTarget )
	{
		this->sTarget = sTarget;
	}

	std::list<class Function *> &getReqFuncs()
	{
		return lReqFuncs;
	}

	void setRule( const std::string &sRule )
	{
		this->sRule = sRule;
	}

	std::string &getRule()
	{
		return sRule;
	}

protected:
	std::list<std::string> lParam;
	std::string sTarget;
	std::string sRule;
	std::list<class Function *> lReqFuncs;
};

#endif
