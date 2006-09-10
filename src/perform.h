#ifndef PERFORM_H
#define PERFORM_H

#include <stdint.h>
#include <list>
#include <string>
#include <map>

typedef std::map<std::string,std::string> VarMap;

class Build;

class Perform
{
public:
	Perform();
	virtual ~Perform();

	void addParam( const char *sParam );
	virtual Perform *duplicate( Build &bld, const std::string &cont ) = 0;
	void copyData( Perform *pSrc, Build &bld, const std::string &cont, VarMap *mExtra );
	std::string getTarget()
	{
		return sTarget;
	}
	void setTarget( std::string sTarget )
	{
		this->sTarget = sTarget;
	}

private:
	std::list<std::string> lParam;
	std::string sTarget;
};

#endif
