#include "cache.h"
#include "serializer.h"

Cache::Cache()
{
}

Cache::~Cache()
{
	for( std::map<std::string, Entry *>::iterator i = mCache.begin();
		 i != mCache.end(); i++ )
	{
		delete (*i).second;
	}
}

void Cache::serialize( class Serializer &ar )
{
	if( ar.isLoading() )
	{
		int sCache, sData;
		ar >> sCache;
		std::string sTmp;

		for( int i = 0; i < sCache; i++ )
		{
			Entry *e = new Entry;
			ar >> e->tCreated;
			ar >> sData;
			std::list<std::string> &lData = e->lData;
			for( int j = 0; j < sData; j++ )
			{
				ar >> sTmp;
				lData.push_back( sTmp );
			}
			ar >> sTmp;
			mCache[sTmp] = e;
		}
	}
	else
	{
		ar << mCache.size();
		for( std::map<std::string, Entry *>::iterator i = mCache.begin();
			 i != mCache.end(); i++ )
		{
			ar << (*i).second->tCreated;
			std::list<std::string> &lData = (*i).second->lData;
			ar << lData.size();
			for( std::list<std::string>::iterator j = lData.begin();
				 j != lData.end(); j++ )
			{
				ar << (*j);
			}

			std::string str = (*i).first;
			ar << str;
		}
	}
}

Cache::Entry *Cache::get( const std::string &id )
{
	std::map<std::string, Entry *>::iterator i = mCache.find( id );
	if( i != mCache.end() )
		return (*i).second;

	return NULL;
}

void Cache::put( const std::string &id, Entry *data )
{
	std::map<std::string, Entry *>::iterator i = mCache.find( id );
	if( i != mCache.end() )
		delete (*i).second;

	mCache[id] = data;
}

