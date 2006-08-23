#ifndef PERFORM_H
#define PERFORM_H

#include <stdint.h>
#include <list>
#include <string>

class Perform
{
public:
	Perform();
	virtual ~Perform();

	void addParam( const char *sParam );

private:
	std::list<std::string> lParam;

};

#endif
