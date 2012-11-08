/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the Xagasoft Build and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "filemgr.h"

FileMgr::FileMgr() :
    iNextId( 1 )
{
}

FileMgr::~FileMgr()
{
    for( FileHash::iterator i = hFile.begin(); i; i++ )
    {
        delete *i;
    }
}

int FileMgr::open( const Bu::String &sPath, int iMode )
{
    hFile.insert( iNextId, new Bu::File( sPath, iMode ) );
    return iNextId++;
}

Bu::File &FileMgr::get( int iId )
{
    try
    {
        return *hFile.get( iId );
    }
    catch(...)
    {
        throw Bu::ExceptionBase("Invalid file handle accessed.");
    }
}

void FileMgr::close( int iId )
{
    try
    {
        delete hFile.get( iId );
        hFile.erase( iId );
    }
    catch(...)
    {
        throw Bu::ExceptionBase("Invalid file handle accessed.");
    }
}

