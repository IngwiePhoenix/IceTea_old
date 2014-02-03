/*
 * Copyright (C) 2007-2014 Xagasoft, All rights reserved.
 *
 * This file is part of the Xagasoft Build and is released under the
 * terms of the license contained in the file LICENSE.
 */
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

void Cache::bind( const Bu::String &sCacheFile )
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

StrList Cache::getRequires( const Bu::String &sOutput )
{
    return hRequires.get( sOutput );
}

void Cache::setRequires( const Bu::String &sOutput, StrList lReqs )
{
    hRequires.insert( sOutput, lReqs );
    bCacheChanged = true;
}

