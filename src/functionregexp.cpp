#include "functionregexp.h"
#include "plugger.h"
#include "regexp.h"

PluginInterface2(regexp, FunctionRegexp, Function, "Mike Buland", 0, 1 );

FunctionRegexp::FunctionRegexp()
{
}

FunctionRegexp::~FunctionRegexp()
{
}

void FunctionRegexp::execute( Build *bld, const StringList &lInput, StringList &lOutput )
{
	if( lParams.size() == 1 )
	{
		RegExp re( lParams.front().c_str() );

		for( StringList::const_iterator i = lInput.begin();
			 i != lInput.end(); i++ )
		{
			if( re.execute( (*i).c_str() ) )
			{
				lOutput.push_back( *i );
				if( bld )
				{
					int jmax = re.getNumSubStrings();
					for( int j = 0; j < jmax; j++ )
					{
						char buf[30];
						sprintf( buf, "re:%d", j );
						bld->set( *i, buf, re.getSubString( j ) );
					}
				}
			}
		}
	}
	else
	{
	}
}

Function *FunctionRegexp::duplicate( Build &bld, const StringList *cont, VarMap *mExtra )
{
	Function *pRet = new FunctionRegexp();
	pRet->copyData( this, bld, cont, mExtra );
	return pRet;
}

