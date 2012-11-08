#include "functionmatches.h"

#include <unistd.h>

#include <bu/plugger.h>
PluginInterface3( pluginFunctionMatches, matches, FunctionMatches, Function,
        "Mike Buland", 0, 1 );

FunctionMatches::FunctionMatches()
{
}

FunctionMatches::~FunctionMatches()
{
}

Bu::String FunctionMatches::getName() const
{
    return "matches";
}

bool FunctionMatches::globcmp( const Bu::String &sTxt, const Bu::String &sMatches )
{
    Bu::String::const_iterator t, g;
    t = sTxt.begin();
    g = sMatches.begin();

    while( g && t )
    {
        switch( *g )
        {
            case '*':
                // First, if the * is at the end, then we do match, it doesn't
                // matter what is in sTxt
                if( !(g+1) )
                    return true;
                // Now attempt to scan for the remainder as a matched set
                {
                    Bu::String::const_iterator tn = t+1, gn = g+1, gi=g+1;
                    bool bFoundMatch = false;
                    while( tn && gn )
                    {
                        if( *gn == '*' )
                        {
                            g = gn;
                            t = tn;
                            break;
                        }
                        if( *tn == *gn )
                        {
                            g = gn;
                            t = tn;
                            tn++;
                            gn++;
                            bFoundMatch = true;
                        }
                        else
                        {
                            gn = gi;
                            tn++;
                            bFoundMatch = false;
                        }
                    }
                    if( bFoundMatch == false )
                        return false;
                    if( !tn && !gn && bFoundMatch )
                        return true;
                }
                break;

            case '?':
                // Don't bother checking.
                t++;
                g++;
                break;

            default:
                if( *t != *g )
                    return false;
                t++;
                g++;
                break;
        }
    }
    if( t || (g && *g != '*') )
        return false;
    return true;
}

bool FunctionMatches::matchlist( const Bu::String &sTxt, VarList &lParams )
{
    for( VarList::iterator i = lParams.begin(); i; i++ )
    {
        if( (*i).getType() == Variable::typeList )
        {
            for( VarList::iterator j = (*i).begin(); j; j++ )
            {
                if( globcmp( sTxt, (*j).toString() ) )
                    return true;
            }
        }
        else
        {
            if( globcmp( sTxt, (*i).toString() ) )
                return true;
        }
    }
    return false;
}

Variable FunctionMatches::call( Variable &input, VarList lParams )
{
    switch( input.getType() )
    {
        case Variable::typeString:
            {
                Bu::String sTxt = input.getString();
                return Variable( matchlist( sTxt, lParams ) );
            }
            break;

        case Variable::typeList:
            {
                Variable vRet( Variable::typeList );
                for( VarList::iterator i = input.begin(); i; i++ )
                {
                    if( (*i).getType() != Variable::typeString )
                        continue;
                    Bu::String sTxt = (*i).getString();
                    if( matchlist( sTxt, lParams ) )
                        vRet.append( *i );
                }
                return vRet;
            }
            break;

        default:
            throw Bu::ExceptionBase("You can only use a string or list as the "
                    "input to matches.");
            break;
    }

    return Variable();
}

