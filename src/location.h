#ifndef LOCATION_H
#define LOCATION_H

#include <bu/fstring.h>

class Location
{
public:
	Location();
	Location( struct YYLTYPE &loc );
	Location( const Bu::FString &sFile, int iStartRow, int iStartCol,
			int iEndRow, int iEndCol );
	virtual ~Location();

private:
	const Bu::FString sFile;
	int iStartRow;
	int iStartCol;
	int iEndRow;
	int iEndCol;
};

#endif
