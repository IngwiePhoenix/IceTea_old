#ifndef CACHE_H
#define CACHE_H

#include <bu/singleton.h>
#include <bu/fstring.h>
#include <bu/hash.h>
#include <bu/list.h>

#include "variable.h"

class Cache : public Bu::Singleton<Cache>
{
friend class Bu::Singleton<Cache>;
private:
	Cache();
	virtual ~Cache();

public:
	void bind( const Bu::FString &sCacheFile );

	void load();
	void save();



private:
	Bu::FString sCacheFile;
	bool bIsLoaded;
	typedef Bu::Hash<Bu::FString, Bu::List<Bu::FString> > ReqHash;
	ReqHash hRequires;
	typedef Bu::Hash<Bu::FString, Variable> VarHash;
	VarHash hVariables;
};

#endif
