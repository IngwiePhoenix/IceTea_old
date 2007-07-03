#include "functionregexp.h"
#include "bu/plugger.h"
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
		if( !bld )
		{
			throw BuildException("You apparently can't use regexp with two params here.  Isn't that odd?");
		}

		RegExp re( lParams.front().c_str() );
		lParams.pop_front();
		std::string p2 = lParams.front();

		for( StringList::const_iterator i = lInput.begin();
			 i != lInput.end(); i++ )
		{
			if( re.execute( (*i).c_str() ) )
			{
				VarMap ext;
				int jmax = re.getNumSubStrings();
				for( int j = 0; j < jmax; j++ )
				{
					char buf[30];
					sprintf( buf, "re:%d", j );
					ext[buf] = re.getSubString( j );
				}

				std::string sNew = bld->replVars( p2, NULL, &ext );
				lOutput.push_back( sNew );

				for( int j = 0; j < jmax; j++ )
				{
					char buf[30];
					sprintf( buf, "re:%d", j );
					bld->set( sNew, buf, re.getSubString( j ) );
				}
				bld->copyContext( *i, sNew );
			}
		}
	}
}

Function *FunctionRegexp::duplicate( Build &bld, const StringList *cont, VarMap *mExtra )
{
	Function *pRet = new FunctionRegexp();
	pRet->copyData( this, bld, cont, mExtra );
	return pRet;
}

