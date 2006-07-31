#include "regexp.h"
#include "builder.h" // For BuildException
#include "staticstring.h"

RegExp::RegExp() :
	bCompiled( false ),
	aSubStr( NULL )
{
}

RegExp::RegExp( const char *sSrc ) :
	bCompiled( false ),
	aSubStr( NULL )
{
	compile( sSrc );
}

RegExp::~RegExp()
{
	regfree( &re );
	delete[] aSubStr;
}

void RegExp::compile( const char *sSrc )
{
	if( bCompiled )
		throw BuildException("Already compiled.");

	int nErr = regcomp( &re, sSrc, REG_EXTENDED|REG_NEWLINE );
	if( nErr )
	{
		size_t length = regerror( nErr, &re, NULL, 0 );
		char *buffer = new char[length];
		(void) regerror( nErr, &re, buffer, length );
		StaticString s( buffer );
		delete[] buffer;
		throw BuildException( s.getString() );
	}
	bCompiled = true;
	this->sSrc = sSrc;

	nSubStr = re.re_nsub+1;
	aSubStr = new regmatch_t[nSubStr];
}

int RegExp::getNumSubStrings()
{
	return nSubStr;
}

bool RegExp::execute( const char *sSrc )
{
	sTest = sSrc;
	if( regexec( &re, sSrc, nSubStr, aSubStr, 0 ) )
		return false;
	return true;
}

std::pair<int,int> RegExp::getSubStringRange( int nIndex )
{
	return std::pair<int,int>( aSubStr[nIndex].rm_so, aSubStr[nIndex].rm_eo );
}

std::string RegExp::getSubString( int nIndex )
{
	return std::string(
		sTest.getString()+aSubStr[nIndex].rm_so,
		aSubStr[nIndex].rm_eo - aSubStr[nIndex].rm_so
		);
}

