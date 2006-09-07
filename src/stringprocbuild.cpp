#include "stringprocbuild.h"
#include "build.h"

StringProcBuild::StringProcBuild( Build *pBld ) :
	StringProc( pBld )
{
}

StringProcBuild::~StringProcBuild()
{
}

std::string StringProcBuild::replVars( const std::string &sSrc, const std::string &sCont )
{
	std::string sDes, sBuf;
	int nMode = 0;

	int nLen = sSrc.size();
	for( int j = 0; j < nLen; j++ )
	{
		if( sSrc[j] == '{' )
		{
			sBuf = "";
			nMode = 1;
		}
		else if( nMode == 0 )
		{
			sDes += sSrc[j];
		}
		else if( nMode == 1 )
		{
			if( sSrc[j] == '}' )
			{
				sDes += getBuild()->getVar( sCont, sBuf );
				nMode = 0;
			}
			else
			{
				sBuf += sSrc[j];
			}
		}
	}

	if( nMode == 1 )
	{
		throw BuildException(
			"Unterminated variable replacement found: \"%s\"",
			sSrc.c_str()
			);
	}

	return sDes;
}

