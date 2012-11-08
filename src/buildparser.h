/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the Xagasoft Build and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BUILD_PARSER_H
#define BUILD_PARSER_H

#include "build.tab.h"

#include "bu/stack.h"
#include "bu/string.h"
#include "types.h"

class BuildParser
{
public:
    BuildParser( class Ast &rAst );
    virtual ~BuildParser();

    void load( const Bu::String &sFile );

    bool isKeyword( const Bu::String &sStr );
    bool isCond( const Bu::String &sStr );
    void include( const Bu::String &sStr, void *scanner, YYLTYPE *loc );
    void endInclude( YYLTYPE *loc );

    void error( int iLine1, int iLine2, int iCol1, int iCol2,
        const Bu::String &sMsg );

    class Ast &xAst;

    void addIncludePath( const Bu::String &sPath );

private:
    Bu::Stack<Bu::String> sFilename;
    Bu::Stack<YYLTYPE> sLocation;
    StrList lIncludePaths;
    Bu::Hash<Bu::String, bool> hConds;
};

typedef void * yyscan_t;
#define YY_DECL int build_lex( YYSTYPE *yylval_param, YYLTYPE *yylloc_param, yyscan_t yyscanner, BuildParser &b )
YY_DECL;

#endif
