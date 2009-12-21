#ifndef BUILD_PARSER_H
#define BUILD_PARSER_H

#include "build.tab.h"

#include "bu/stack.h"
#include "bu/fstring.h"
#include "types.h"

class BuildParser
{
public:
	BuildParser( class Ast &rAst );
	virtual ~BuildParser();

	void load( const Bu::FString &sFile );

	bool isKeyword( const Bu::FString &sStr );
	bool isCond( const Bu::FString &sStr );
	void include( const Bu::FString &sStr, void *scanner, YYLTYPE *loc );
	void endInclude( YYLTYPE *loc );

	void error( int iLine1, int iLine2, int iCol1, int iCol2,
		const Bu::FString &sMsg );

	class Ast &xAst;

	void addIncludePath( const Bu::FString &sPath );

private:
	Bu::Stack<Bu::FString> sFilename;
	Bu::Stack<YYLTYPE> sLocation;
	StrList lIncludePaths;
};

typedef void * yyscan_t;
#define YY_DECL int build_lex( YYSTYPE *yylval_param, YYLTYPE *yylloc_param, yyscan_t yyscanner, BuildParser &b )
YY_DECL;

#endif
