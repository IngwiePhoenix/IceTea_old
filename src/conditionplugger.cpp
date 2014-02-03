/*
 * Copyright (C) 2007-2014 Xagasoft, All rights reserved.
 *
 * This file is part of the Xagasoft Build and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "conditionplugger.h"

#include <sys/types.h>
#include <dirent.h>

extern Bu::PluginInfo pluginConditionAlways;
extern Bu::PluginInfo pluginConditionNever;
extern Bu::PluginInfo pluginConditionFileTime;
extern Bu::PluginInfo pluginConditionFileExists;

ConditionPlugger::ConditionPlugger()
{
    registerBuiltinPlugin( &pluginConditionAlways );
    registerBuiltinPlugin( &pluginConditionNever );
    registerBuiltinPlugin( &pluginConditionFileTime );
    registerBuiltinPlugin( &pluginConditionFileExists );

    DIR *dir = opendir("/usr/lib/build");
    if( !dir )
        return;
    struct dirent *de;
    while( (de = readdir( dir )) )
    {
        if( strncmp("pluginCondition", de->d_name, 15 ) )
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

ConditionPlugger::~ConditionPlugger()
{
}

