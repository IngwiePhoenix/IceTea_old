#ifndef STAT_CACHE_H
#define STAT_CACHE_H

#include <bu/hash.h>
#include <bu/singleton.h>
#include <bu/fstring.h>
#include <time.h>

class StatCache : public Bu::Singleton<StatCache>
{
friend class Bu::Singleton<StatCache>;
private:
	StatCache();
	virtual ~StatCache();

public:
	time_t mtime( const Bu::FString &sFileName );

private:
	typedef Bu::Hash<Bu::FString, time_t> TimeHash;
	TimeHash hMTime;
};

#endif
