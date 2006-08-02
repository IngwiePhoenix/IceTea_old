#include "viewerpercent.h"
#include "perform.h"

ViewerPercent::ViewerPercent() :
	nWidth( 25 )
{
}

ViewerPercent::~ViewerPercent()
{
}
	
void ViewerPercent::beginTarget( const char *sName, const char *sType, const char *sOperation, int nPerforms )
{
	printf("--- %s ---\n", sName );
	nMax = nPerforms;
	nCount = 0;
	nLastLen = 0;
}

void ViewerPercent::endTarget()
{
	printf("\n\n");
}

void ViewerPercent::beginPerform( Perform *pPerf )
{
	sTarget = pPerf->getTarget();
}

void ViewerPercent::beginExecute()
{
}

void ViewerPercent::endPerform()
{
	nCount++;

	int nPer = (nCount*nWidth)/nMax;
	fputc( '[', stdout );
	for( int j = 0; j < nPer; j++ )
		fputc('=', stdout );
	for( int j = nPer; j < nWidth; j++ )
		fputc(' ', stdout );
	//fputc(']', stdout );

	printf("] %s", sTarget.getString() );

	int diff = nLastLen-sTarget;
	for( int j = 0; j < diff; j++ )
		fputc(' ', stdout );

	nLastLen = sTarget;

	fputc('\r', stdout );
	fflush( stdout );
}

