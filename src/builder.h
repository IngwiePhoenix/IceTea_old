#ifndef BUILDER_H
#define BUILDER_H

#include <string>
#include <list>
#include <map>
#include "build.tab.h"
#include "exceptionbase.h"

subExceptionDecl( BuildException )

class Builder;
class Action;
class Command;

#define YY_DECL int yylex( YYSTYPE *yylval_param, YYLTYPE *yylloc_param, Builder &bld )
YY_DECL;

class Builder
{
	struct ltstr
	{
		bool operator()(const char* s1, const char* s2) const
		{
			return strcmp(s1, s2) < 0;
		}
	};

public:
	Builder();
	virtual ~Builder();

	void load( const char *sFN );

	//void error( const yy::location &l, const std::string &m );
	//void error( const std::string &m );

	std::string file;

	void add( Action *pAct );
	void add( Command *pCmd );

	bool hasDefaultAction()
	{
		return pDefaultAction != NULL;
	}

	void debug();

private:
	void scanBegin();
	void scanEnd();

	Action *pDefaultAction;
	Action *pLastAddedAction;
	std::map<const char *, Action *, ltstr> mAction;
};

#endif
