#ifndef BUILDER_H
#define BUILDER_H

#include <string>

union YYSTYPE;

namespace yy
{
	class location;
	class BuildParser;
}
class Builder;

#define YY_DECL int yylex( YYSTYPE *yylval_param, yy::location *yylloc, Builder &bld )
YY_DECL;

class Builder
{
public:
	Builder();
	virtual ~Builder();

	void load( const char *sFN );

	void error( const yy::location &l, const std::string &m );
	void error( const std::string &m );

	std::string file;

private:
	void scanBegin();
	void scanEnd();
};

#endif
