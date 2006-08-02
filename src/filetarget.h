#ifndef FILE_TARGET_H
#define FILE_TARGET_H

#include <stdint.h>
#include <map>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
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

	time_t getTime( class Builder &bld, std::string str );
	void updateTime( std::string str );
	
private:
	std::map<std::string, time_t> mTimes;

};

#endif
