#include "cache.h"
#include <bu/file.h>
#include <bu/archive.h>
#include <bu/sio.h>
using namespace Bu;

Cache::Cache() :
	bCacheChanged( false ),
	bIsLoaded( false )
{
}

Cache::~Cache()
{
	save();
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

	try
	{
		Bu::File fIn( sCacheFile, Bu::File::Read );
		Bu::Archive ar( fIn, Bu::Archive::load );

		ar >> hRequires >> hVariables;
	}
	catch(...) { }

	bIsLoaded = true;
}

void Cache::save()
{
	if( !bIsLoaded )
		return;
	if( bCacheChanged == false )
		return;

	Bu::File fIn( sCacheFile, Bu::File::WriteNew );
	Bu::Archive ar( fIn, Bu::Archive::save );

	ar << hRequires << hVariables;
}

StrList Cache::getRequires( const Bu::FString &sOutput )
{
	return hRequires.get( sOutput );
}

void Cache::setRequires( const Bu::FString &sOutput, StrList lReqs )
{
	hRequires.insert( sOutput, lReqs );
	bCacheChanged = true;
}

