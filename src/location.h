#ifndef LOCATION_H
#define LOCATION_H

#include <bu/string.h>

class Location
{
public:
    Location();
    Location( struct YYLTYPE &loc );
    Location( const Bu::String &sFile, int iStartRow, int iStartCol,
            int iEndRow, int iEndCol );
    virtual ~Location();

private:
    const Bu::String sFile;
    int iStartRow;
    int iStartCol;
    int iEndRow;
    int iEndCol;
};

#endif
