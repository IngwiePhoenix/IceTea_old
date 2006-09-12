#ifndef TARGET_FILE_H
#define TARGET_FILE_H

#include <stdint.h>
#include <map>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "target.h"

class TargetFile : public Target
{
public:
	TargetFile();
	virtual ~TargetFile();

	virtual void check( Build &bld );
	virtual void clean( Build &bld );

	time_t getTime( class Build &bld, std::string str );
	void updateTime( std::string str );

private:
	std::map<std::string, time_t> mTimes;
};

#endif
