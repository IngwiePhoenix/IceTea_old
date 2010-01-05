#include "statcache.h"

#include <sys/stat.h>

#include <bu/sio.h>

StatCache::StatCache()
{
}

StatCache::~StatCache()
{
}

time_t StatCache::mtime( const Bu::FString &sFileName )
{
	try
	{
		return hMTime.get( sFileName );
	} catch( ... )
	{
		struct stat s;
		stat( sFileName.getStr(), &s );
		hMTime.insert( sFileName, s.st_mtime );
		return s.st_mtime;
	}
}

