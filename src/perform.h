#ifndef PERFORM_H
#define PERFORM_H

#include <stdint.h>

class Perform
{
public:
	Perform();
	virtual ~Perform();

	virtual void execute( class Builder &bld ) = 0;

private:

};

#endif
