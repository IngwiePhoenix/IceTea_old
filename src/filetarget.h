#ifndef FILE_TARGET_H
#define FILE_TARGET_H

#include <stdint.h>
#include "target.h"

class FileTarget : public Target
{
public:
	FileTarget( const char *sName );
	virtual ~FileTarget();

	virtual void debug();
	
private:

};

#endif
