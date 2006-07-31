#ifndef BUILDER_H
#define BUILDER_H

#include <string>
#include <list>
#include <map>
#include "build.tab.h"
#include "exceptionbase.h"
#include "staticstring.h"

subExceptionDecl( BuildException )

class Builder;
class Action;
class Command;
class Rule;
class Target;

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
	void add( Rule *pRule );
	void add( Target *pTarg );
	void varSet( const char *sName, const char *sValue );
	void varAddSet( const char *sName, const char *sValue );
	void requires( const char *sBase, const char *sReq );
	void setContext( const char *sCont );
	void setContext();

	bool hasDefaultAction()
	{
		return pDefaultAction != NULL;
	}

	void debug();

	Rule *lastRule()
	{
		return pLastAddedRule;
	}

	Target *lastTarget()
	{
		return pLastAddedTarget;
	}

	void setTmp( const char *s )
	{
		sTmp = s;
	}

	const char *getTmp()
	{
		return sTmp;
	}

private:
	void checkVar( const char *cont, const char *sName );
	void scanBegin();
	void scanEnd();

	Action *pDefaultAction;
	Action *pLastAddedAction;
	std::map<const char *, Action *, ltstr> mAction;

	Rule *pLastAddedRule;
	std::map<const char *, Rule *, ltstr> mRule;

	Target *pLastAddedTarget;
	std::map<const char *, Target *, ltstr> mTarget;

	typedef std::map<std::string, std::string> varmap;
	varmap mVar;
	std::map<std::string, std::list<std::string> *> mRequires;

	std::map<std::string, varmap> mContVar;
	StaticString sContext;

	StaticString sTmp;
};

#endif
