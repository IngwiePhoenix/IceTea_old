#ifndef PERFORM_COMMAND_H
#define PERFORM_COMMAND_H

#include <stdint.h>

#include "perform.h"

class PerformCommand : public Perform
{
public:
	PerformCommand();
	virtual ~PerformCommand();

	virtual Perform *duplicate( Build &bld, const std::string &cont );

private:

};

#endif
