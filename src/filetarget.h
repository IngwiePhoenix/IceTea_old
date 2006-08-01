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

	void addInputDir( const char *sDir );

	virtual void check( class Builder &bld );
	virtual void clean( class Builder &bld );
	
private:
	// start here with the file time cache

};

#endif
