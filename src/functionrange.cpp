#include "functionrange.h"

#include <bu/plugger.h>
PluginInterface3( pluginFunctionRange, range, FunctionRange, Function,
        "Mike Buland", 0, 1 );

FunctionRange::FunctionRange()
{
}

FunctionRange::~FunctionRange()
{
}

Bu::String FunctionRange::getName() const
{
    return "range";
}

Variable FunctionRange::call( Variable &input, VarList lParams )
{
    Variable vRet( Variable::typeList );
    int iLow = 1;
    int iHigh = 1;
    int iStep = 1;
    if( lParams.getSize() == 1 )
    {
        iHigh = lParams.first().toInt();
    }
    else if( lParams.getSize() == 2 )
    {
        iLow = lParams.first().toInt();
        iHigh = lParams.last().toInt();
    }
    else if( lParams.getSize() == 3 )
    {
        VarList::iterator i = lParams.begin();
        iLow = (*i).toInt();
        i++;
        iHigh = (*i).toInt();
        i++;
        iStep = (*i).toInt();
    }
    if( iStep == 0 )
        throw Bu::ExceptionBase("Step cannot be zero.");

    if( iHigh < iLow )
    {
        if( iStep > 0 )
            throw Bu::ExceptionBase(
                "If start is less than end then step must be negative.");
        for( int j = iLow; j >= iHigh; j += iStep )
        {
            vRet.append( Variable( j ) );
        }
    }
    else
    {
        if( iStep < 0 )
            throw Bu::ExceptionBase(
                "If start is more than end then step must be positive.");
        for( int j = iLow; j <= iHigh; j += iStep )
        {
            vRet.append( Variable( j ) );
        }
    }


    return vRet;
}

