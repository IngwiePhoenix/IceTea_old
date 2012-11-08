/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the Xagasoft Build and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "viewplugger.h"

#include <sys/types.h>
#include <dirent.h>

extern Bu::PluginInfo pluginViewDefault;
extern Bu::PluginInfo pluginViewMake;
ViewPlugger::ViewPlugger()
{
    registerBuiltinPlugin( &pluginViewDefault );
    registerBuiltinPlugin( &pluginViewMake );

    DIR *dir = opendir("/usr/lib/build");
    if( !dir )
        return;
    struct dirent *de;
    while( (de = readdir( dir )) )
    {
        if( strncmp("pluginView", de->d_name, 15 ) )
            continue;

        Bu::String sFile("/usr/lib/build/");
        sFile += de->d_name;
        char *s = de->d_name;
        for(; *s && *s != '.'; s++ ) { }
        registerExternalPlugin(
            sFile,
            Bu::String( de->d_name, (ptrdiff_t)s-(ptrdiff_t)de->d_name )
            );
    }

    closedir( dir );
}

ViewPlugger::~ViewPlugger()
{
}

