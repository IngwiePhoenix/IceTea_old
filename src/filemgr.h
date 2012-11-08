/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the Xagasoft Build and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef FILE_MGR_H
#define FILE_MGR_H

#include <bu/singleton.h>
#include <bu/hash.h>
#include <bu/file.h>

class FileMgr : public Bu::Singleton<FileMgr>
{
friend class Bu::Singleton<FileMgr>;
private:
    FileMgr();
    virtual ~FileMgr();

public:
    int open( const Bu::String &sPath, int iMode );
    Bu::File &get( int iId );
    void close( int iId );

private:
    typedef Bu::Hash<int, Bu::File *> FileHash;
    FileHash hFile;
    int iNextId;
};

#endif
