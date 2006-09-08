#include "perform.h"
#include "build.h"

Perform::Perform()
{
}

Perform::~Perform()
{
}

void Perform::addParam( const char *sParam )
{
	lParam.push_back( sParam );
}

void Perform::copyData( Perform *pSrc, Build &bld, const std::string &cont )
{
	lParam.clear();
	for( std::list<std::string>::iterator i = pSrc->lParam.begin();
		 i != pSrc->lParam.end(); i++ )
	{
		lParam.push_back( bld.replVars( *i, cont ) );
	}
}

