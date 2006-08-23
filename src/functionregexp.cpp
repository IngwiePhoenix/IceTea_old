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

void FunctionRegexp::execute( const StringList &lInput, StringList &lOutput )
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
			}
		}
	}
	else
	{
	}
}

