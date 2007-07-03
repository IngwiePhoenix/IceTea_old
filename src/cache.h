#ifndef CACHE_H
#define CACHE_H

#include <stdint.h>
#include <time.h>
#include "bu/archival.h"
#include <list>
#include <map>
#include <string>

class Cache : public Bu::Archival
{
public:
	Cache();
	virtual ~Cache();

	virtual void archive( class Bu::Archive &ar );

	class Entry
	{
	public:
		uint32_t tCreated;
		std::list<std::string> lData;
	};

	Entry *get( const std::string &id );
	void put( const std::string &id, Entry *data );

private:
	std::map<std::string, Entry *> mCache;
};

#endif
