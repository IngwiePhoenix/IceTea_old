/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the Xagasoft Build and is released under the
 * terms of the license contained in the file LICENSE.
 */

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
