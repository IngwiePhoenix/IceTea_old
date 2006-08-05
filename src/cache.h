#ifndef CACHE_H
#define CACHE_H

#include <stdint.h>
#include <time.h>
#include "serializable.h"
#include <list>
#include <map>
#include <string>

class Cache : public Serializable
{
public:
	Cache();
	virtual ~Cache();

	virtual void serialize( class Serializer &ar );

	class Entry
	{
	public:
		int tCreated;
		std::list<std::string> lData;
	};

	Entry *get( const std::string &id );
	void put( const std::string &id, Entry *data );

private:
	std::map<std::string, Entry *> mCache;
};

#endif
