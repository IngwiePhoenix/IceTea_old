/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the Xagasoft Build and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "buildparser.h"
#include "context.h"
#include "ast.h"
#include "runner.h"
#include "target.h"

#include "viewplugger.h"
#include "functionplugger.h"
#include "conditionplugger.h"

#include "cache.h"

#include <bu/optparser.h>
#include <bu/sio.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>

extern char **environ;

using namespace Bu;

class Options : public Bu::OptParser
{
public:
    Options( int argc, char *argv[] ) :
        sView("default"),
        sAction("default"),
        sConfig("default.bld"),
        sCacheFile(".build_cache"),
        bDot( false ),
        bDebug( false ),
        bAutoInclude( true ),
        bAstDump( false ),
        bEnviron( true ),
        bCache( true ),
        iInfoLevel( 0 )
    {
        bool bClean = false;
        addHelpBanner("build mark 3\n");

        Bu::String sViews("Select a view from: ");
        StrList lViews = ViewPlugger::getInstance().getPluginList();
        for( StrList::iterator i = lViews.begin(); i; i++ )
        {
            if( i != lViews.begin() )
                sViews += ", ";
            sViews += *i;
        }

        addHelpBanner("The following options do things other than build:");
        addOption( iInfoLevel, 'i', "info", "Display some basic info about the "
            "loaded build config, including available targets.");
        addOption( slot( this, &Options::onListPlugins), "list-plugins",
            "List all available plugins.");
        addHelpOption();

        addHelpBanner("\nThe following options control general execution:");
        addOption( sView, 'v', "view", sViews );
        addOption( sConfig, 'f', "file", "Select a different config file." );
        addOption( bClean, 'c', "Shorthand for running action 'clean'.  If an "
            "action is specified, this will modify it to run 'clean-action'.");
        addOption( slot(this, &Options::onChdir), 'C', "chdir",
            "Change to directory before doing anything else.");
        addOption( sCacheFile, "cache", "Select a different cache file.");
        addOption( bCache, "no-cache", "Disable using the cache.");

        addHelpBanner("\nThe following options control debugging:");
        addOption( bEnviron, "no-env", "Do not import environment variables.");
        addOption( bDot, "dot", "Generate a dot chart after execution." );
        addOption( bDebug, "debug",
            "Dump massive amounts of hard to read debugging data." );
        addOption( bAstDump, "debug-ast",
            "Display the raw AST that is computed from parsing the input.  "
            "You should probably never ever use this, it'll scare you."
            );

        setHelpDefault( "view", "=[view]" );
        setHelpDefault( "file", "=[file]" );
        setHelpDefault( "cache", "=[file]" );
        setHelpDefault( "chdir", "=[dir]" );

        setOverride( "no-env", "false" );
        setOverride( "dot", "true" );
        setOverride( "debug", "true" );
        setOverride( "debug-ast", "true" );
        setOverride( "info", "1" );
        setOverride( 'c', "true" );
        setOverride( "no-cache", "false" );

        setNonOption( slot( this, &Options::onNonOption ) );

        parse( argc, argv );

        if( bClean )
        {
            if( sAction == "default" )
                sAction = "clean";
            else
                sAction.prepend("clean-");
        }
    }

    virtual ~Options()
    {
    }

    int onChdir( StrArray sParams )
    {
        if( sParams.getSize() == 0 )
        {
            sio << "You must specify a directory name!" << sio.nl << sio.nl;
            exit(2);
        }
        chdir( sParams[1].getStr() );
        return 1;
    }

    int onNonOption( StrArray sParams )
    {
        sAction = sParams[0];
        return 0;
    }

    int onListPlugins( StrArray /*sParams*/ )
    {
        StrList lViews = ViewPlugger::getInstance().getPluginList();
        sio << "Available view plugins:" << sio.nl << "\t";
        for( StrList::iterator i = lViews.begin(); i; i++ )
        {
            if( i != lViews.begin() )
                sio << ", ";
            sio << *i;
        }

        StrList lFuncs = FunctionPlugger::getInstance().getPluginList();
        sio << sio.nl << sio.nl << "Available function plugins:"
            << sio.nl << "\t";
        for( StrList::iterator i = lFuncs.begin(); i; i++ )
        {
            if( i != lFuncs.begin() )
                sio << ", ";
            sio << *i;
        }
        
        StrList lConds = ConditionPlugger::getInstance().getPluginList();
        sio << sio.nl << sio.nl << "Available condition plugins:"
            << sio.nl << "\t";
        for( StrList::iterator i = lConds.begin(); i; i++ )
        {
            if( i != lConds.begin() )
                sio << ", ";
            sio << *i;
        }

        sio << sio.nl << sio.nl;

        return 0;
    }

    Bu::String sView;
    Bu::String sAction;
    Bu::String sConfig;
    Bu::String sCacheFile;
    bool bDot;
    bool bDebug;
    bool bAutoInclude;
    bool bAstDump;
    bool bEnviron;
    bool bCache;
    int iInfoLevel;
};

int main( int argc, char *argv[] )
{
    typedef Bu::List<Bu::String> StrList;
    StrList lShareList;
    lShareList.append("/usr/share/build/").append("./share/");
    Ast ast;
    Context cnt;
    BuildParser bp( ast );

    for( StrList::iterator i = lShareList.begin(); i; i++ )
    {
        bp.addIncludePath( *i + "include");
    }

    Options opts( argc, argv );

    try
    {
        cnt.setView( ViewPlugger::getInstance().instantiate( opts.sView ) );
    }
    catch( Bu::HashException &e )
    {
        sio << "Error:  Invalid view specified, please choose from the "
            "following choices:" << sio.nl << sio.nl << "\t";

        StrList lViews = ViewPlugger::getInstance().getPluginList();
        for( StrList::iterator i = lViews.begin(); i; i++ )
        {
            if( i != lViews.begin() )
                sio << ", ";
            sio << *i;
        }
        sio << sio.nl << sio.nl;
        return 1;
    }

    if( opts.bCache )
    {
        Cache::getInstance().bind( opts.sCacheFile );
    }

    // Load up the environment as vars.
    if( opts.bEnviron )
    {
        for( char **env = environ; *env; env++ )
        {
            int iSplit;
            for( iSplit = 0; (*env)[iSplit] != '='; iSplit++ ) { }
            cnt.addVariable(
                String( *env, iSplit ),
                String( *env+iSplit+1 )
                );
        }
    }

    if( opts.bAutoInclude )
    {
        DIR *d;
        Bu::String sAutoDir;
        for( StrList::iterator i = lShareList.begin(); i; i++ )
        {
            sAutoDir = *i + "autoinclude";
            d = opendir( sAutoDir.getStr() );
            if( d )
                break;
        }
        if( !d )
        {
            cnt.getView()->sysWarning(
                "Could not find an autoinclude directory."
                );
        }
        else
        {
            struct dirent *de;
            while( (de = readdir( d )) )
            {
                if( de->d_name[0] == '.' || (de->d_type != DT_REG) )
                    continue;
                //sio << "Auto-including: " << de->d_name << sio.nl;
                bp.load( sAutoDir + "/" + de->d_name );
            }
        }
    }

    bp.load( opts.sConfig );

    if( opts.bAstDump )
    {
        sio << ast << sio.nl << sio.nl;
        return 0;
    }

//  sio << ast << sio.nl;

    Runner r( ast, cnt );
    r.initialize();

    r.run();

    switch( opts.iInfoLevel )
    {
        case 0:
            // Do nothing
            break;

        case 1:
            cnt.printBasicInfo();
            return 0;
    }

    try
    {
        r.execAction( opts.sAction );
    }
    catch( std::exception &e )
    {
        cnt.getView()->sysError(e.what());
    }
    catch( ... )
    {
        cnt.getView()->sysError(
            "Unknown error occured, this is probably bad..."
            );
    }

    if( opts.bDot )
    {
        cnt.writeTargetDot();
    }

    if( opts.bDebug )
    {
        sio << "Final context:" << sio.nl << cnt << sio.nl << sio.nl;
    }

    return 0;
}

