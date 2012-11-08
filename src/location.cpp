/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the Xagasoft Build and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "location.h"
#include "build.tab.h"

Location::Location() :
    sFile("none"),
    iStartRow( -1 ),
    iStartCol( -1 ),
    iEndRow( -1 ),
    iEndCol( -1 )
{
}

Location::Location( struct YYLTYPE &loc ) :
    sFile("???"),
    iStartRow( loc.first_line ),
    iStartCol( loc.first_column ),
    iEndRow( loc.last_line ),
    iEndCol( loc.last_column )
{
}

Location::Location( const Bu::String &sFile, int iStartRow, int iStartCol,
        int iEndRow, int iEndCol ) :
    sFile( sFile ),
    iStartRow( iStartRow ),
    iStartCol( iStartCol ),
    iEndRow( iEndRow ),
    iEndCol( iEndCol )
{
}

Location::~Location()
{
}

