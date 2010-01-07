#include "functionregex.h"

#include <bu/regex.h>
#include <bu/plugger.h>
PluginInterface3( pluginFunctionRegEx, regex, FunctionRegEx, Function,
		"Mike Buland", 0, 1 );

FunctionRegEx::FunctionRegEx()
{
}

FunctionRegEx::~FunctionRegEx()
{
}

Bu::FString FunctionRegEx::getName() const
{
	return "regex";
}

Variable FunctionRegEx::call( Variable &input, VarList lParams )
{
	if( lParams.getSize() == 1 )
	{
		Bu::RegEx re( lParams.first().getString() );
		switch( input.getType() )
		{
			case Variable::typeString:
				return re.execute( input.getString() );

			case Variable::typeList:
				{
					Variable vOut( Variable::typeList );
					for( VarList::iterator i = input.begin(); i; i++ )
					{
						if( re.execute( (*i).toString() ) )
							vOut.append( *i );
					}
					return vOut;
				}
				break;

			default:
				break;
		}
	}
	else if( lParams.getSize() == 2 )
	{
		Bu::RegEx re( lParams.first().getString() );
		Bu::FString sPat = lParams.last().getString();
		switch( input.getType() )
		{
			case Variable::typeString:
				if( re.execute( input.getString() ) )
				{
					return replace( re, input.getString(), sPat );
				}
				else
				{
					return input;
				}
				break;

			case Variable::typeList:
				{
					Variable vOut( Variable::typeList );
					for( VarList::iterator i = input.begin(); i; i++ )
					{
						if( re.execute( (*i).toString() ) )
							vOut.append( replace( re, (*i).toString(), sPat ) );
						else
							vOut.append( *i );
					}
					return vOut;
				}
				break;

			default:
				break;
		}
	}
	throw Bu::ExceptionBase(
		"regex does not work on non-string or non-list types.");
}

Bu::FString FunctionRegEx::replace( Bu::RegEx &re, const Bu::FString &sSrc,
		const Bu::FString &sPat )
{
	Bu::FString sOut;

	int iStart, iEnd;
	re.getSubStringRange( 0, iStart, iEnd ); // Get the range of the full match
	
	if( iStart > 0 )
		sOut.append( sSrc, 0, iStart ); 

	for( Bu::FString::const_iterator i = sPat.begin(); i; i++ )
	{
		if( *i == '\\' )
		{
			i++;
			if( *i <= '9' && *i >= '0' )
			{
				int iInd = *i-'0';
				if( iInd < re.getNumSubStrings() )
					sOut += re.getSubString( iInd );
			}
			else
			{
				sOut += *i;
			}
		}
		else
		{
			sOut += *i;
		}
	}

	if( iEnd < sSrc.getSize() )
	{
		sOut.append( sSrc, iEnd, -1 );
	}

	return sOut;
}

