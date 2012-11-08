/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the Xagasoft Build and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef CACHE_H
#define CACHE_H

#include <bu/singleton.h>
#include <bu/string.h>
#include <bu/hash.h>
#include <bu/list.h>

#include "variable.h"
#include "types.h"

class Cache : public Bu::Singleton<Cache>
{
friend class Bu::Singleton<Cache>;
private:
    Cache();
    virtual ~Cache();

public:
    void bind( const Bu::String &sCacheFile );

    void load();
    void save();

    StrList getRequires( const Bu::String &sOutput );
    void setRequires( const Bu::String &sOutput, StrList lReqs );

private:
    bool bCacheChanged;
    Bu::String sCacheFile;
    bool bIsLoaded;
    typedef Bu::Hash<Bu::String, StrList> ReqHash;
    ReqHash hRequires;
    typedef Bu::Hash<Bu::String, Variable> VarHash;
    VarHash hVariables;
};

#endif
