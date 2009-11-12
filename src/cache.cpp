#include "cache.h"
#include "bu/fstring.h"
#include "bu/archive.h"

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

void Cache::archive( class Bu::ArchiveBase &ar )
{
	if( ar.isLoading() )
	{
		uint32_t sCache, sData, sIndex;

		ar >> sIndex;
		Bu::FString *Index = new Bu::FString[sIndex];
		for( uint32_t i = 0; i < sIndex; i++ )
		{
			ar >> Index[i];
		}

		ar >> sCache;
		uint32_t nTmp;
		for( uint32_t i = 0; i < sCache; i++ )
		{
			Entry *e = new Entry;
			ar >> e->tCreated;
			ar >> sData;
			std::list<std::string> &lData = e->lData;
			for( uint32_t j = 0; j < sData; j++ )
			{
				ar >> nTmp;
				lData.push_back( Index[nTmp].getStr() );
			}
			ar >> nTmp;
			mCache[Index[nTmp].getStr()] = e;
		}
		/*
		uint32_t sCache, sData;
		ar >> sCache;
		std::string sTmp;

		for( uint32_t i = 0; i < sCache; i++ )
		{
			Entry *e = new Entry;
			ar >> e->tCreated;
			ar >> sData;
			std::list<std::string> &lData = e->lData;
			for( uint32_t j = 0; j < sData; j++ )
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
		std::map<std::string, uint32_t> mIndex;
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

		ar << (uint32_t)mIndex.size();
		uint32_t cnt = 0;
		for( std::map<std::string, uint32_t>::iterator i = mIndex.begin();
			 i != mIndex.end(); i++ )
		{
			(*i).second = cnt;
			cnt++;
			Bu::FString s( ((*i).first).c_str(), ((*i).first).size() );
			ar << s;
		}

		ar << (uint32_t)mCache.size();
		for( std::map<std::string, Entry *>::iterator i = mCache.begin();
			 i != mCache.end(); i++ )
		{
			ar << (*i).second->tCreated;
			std::list<std::string> &lData = (*i).second->lData;
			ar << (uint32_t)lData.size();
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

