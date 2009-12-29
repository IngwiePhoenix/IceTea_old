#include "cache.h"
#include <bu/file.h>
#include <bu/archive.h>

Cache::Cache() :
	bIsLoaded( false )
{
}

Cache::~Cache()
{
}

void Cache::bind( const Bu::FString &sCacheFile )
{
	this->sCacheFile = sCacheFile;
	load();
}

void Cache::load()
{
	if( bIsLoaded )
		return;
	Bu::File fIn( sCacheFile, Bu::File::Read );
	Bu::Archive ar( fIn, Bu::Archive::load );

	ar >> hRequires >> hVariables;

	bIsLoaded = true;
}

void Cache::save()
{
	Bu::File fIn( sCacheFile, Bu::File::WriteNew );
	Bu::Archive ar( fIn, Bu::Archive::save );

	ar << hRequires << hVariables;
}

