#include "cache.h"
#include "serializer.h"
#include "staticstring.h"

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
		int sCache, sData, sIndex;

		ar >> sIndex;
		StaticString *Index = new StaticString[sIndex];
		for( int i = 0; i < sIndex; i++ )
		{
			ar >> Index[i];
		}

		ar >> sCache;
		int nTmp;
		for( int i = 0; i < sCache; i++ )
		{
			Entry *e = new Entry;
			ar >> e->tCreated;
			ar >> sData;
			std::list<std::string> &lData = e->lData;
			for( int j = 0; j < sData; j++ )
			{
				ar >> nTmp;
				lData.push_back( Index[nTmp].getString() );
			}
			ar >> nTmp;
			mCache[Index[nTmp].getString()] = e;
		}
		/*
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
		*/
	}
	else
	{
		std::map<std::string, int> mIndex;
		for( std::map<std::string, Entry *>::iterator i = mCache.begin();
			 i != mCache.end(); i++ )
		{
			mIndex[(*i).first] = 0;
			std::list<std::string> &lData = (*i).second->lData;
			for( std::list<std::string>::iterator j = lData.begin();
				 j != lData.end(); j++ )
			{
				mIndex[(*j)] = 0;
			}
		}

		ar << mIndex.size();
		int cnt = 0;
		for( std::map<std::string, int>::iterator i = mIndex.begin();
			 i != mIndex.end(); i++ )
		{
			(*i).second = cnt;
			cnt++;
			std::string s = ((*i).first);
			ar << s;
		}

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
				ar << mIndex[(*j)];
			}

			ar << mIndex[(*i).first];
		}


		/*
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
		*/
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

