#ifndef TARGET_FILE_H
#define TARGET_FILE_H

#include <stdint.h>

#include "target.h"

class TargetFile : public Target
{
public:
	TargetFile();
	virtual ~TargetFile();

	virtual void check( Build &bld );
	virtual void clean( Build &bld );

private:

};

#endif
