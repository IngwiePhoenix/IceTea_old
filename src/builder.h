#ifndef BUILDER_H
#define BUILDER_H

#include <string>
#include <list>
#include <map>
#include "build.tab.h"
#include "exceptionbase.h"
#include "staticstring.h"
#include "regexp.h"

subExceptionDecl( BuildException )

class Builder;
class Action;
class Command;
class Rule;
class Target;
class Viewer;

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
	Builder( Viewer &rView );
	virtual ~Builder();

	void load( const char *sFN );
	void build( const char *sAct=NULL );
	void execute( Action *pAct );

	void error( YYLTYPE *locp, const std::string &m );
	void error( const std::string &m );

	std::string file;

	Viewer &view()
	{
		return rView;
	}
	void add( Action *pAct );
	void add( Command *pCmd );
	void add( Rule *pRule );
	void add( Target *pTarg );
	void varSet( const char *sName, const char *sValue );
	void varAddSet( const char *sName, const char *sValue );
	Rule *getRule( const char *sName );
	std::list<Rule *> findRuleChain( Rule *pRule );
	void processRequires( std::list<std::string> &lInput );
	void requires( const char *sBase, const char *sReq );
	void requiresFromCommand( const char *sBase, const char *sReq );
	void genRequiresFor( const char *sName );
	void requiresRegexp( bool on )
	{
		bReqRegexp = on;
	}
	bool isRequiresRegexp()
	{
		return bReqRegexp;
	}
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

	Target *getTarget( const char *sName )
	{
		if( mTarget.find( sName ) == mTarget.end() )
			throw BuildException("Target %s not found.", sName );

		return mTarget[sName];
	}

	std::list<std::string> *getRequires( const char *sReq )
	{
		if( mRequires.find(sReq) == mRequires.end() )
			return NULL;
		return mRequires[sReq];
	}

	typedef std::map<std::string, std::string> varmap;
	varmap *regexVars( RegExp *re );
	std::string varRepl( const char *sSrc, const char *cont, varmap *mExtra );

private:
	void requiresNormal( const char *sBase, const char *sReq );
	void requiresRegexp( const char *sBase, const char *sReq );
	void checkVar( const char *cont, const char *sName );
	void scanBegin();
	void scanEnd();

	bool hasVar( varmap *pMap, std::string &var );

	Action *pDefaultAction;
	Action *pLastAddedAction;
	std::map<const char *, Action *, ltstr> mAction;

	Rule *pLastAddedRule;
	std::map<const char *, Rule *, ltstr> mRule;

	Target *pLastAddedTarget;
	std::map<const char *, Target *, ltstr> mTarget;

	varmap mVar;

	std::map<std::string, std::list<std::string> *> mRequires;

	typedef std::list<std::pair<RegExp *, std::string> > regreqlist;
	regreqlist lRequiresRegexp;
	regreqlist lRequiresRegexpCommand;

	std::map<std::string, varmap> mContVar;
	StaticString sContext;

	StaticString sTmp;

	bool bReqRegexp;

	Viewer &rView;
};

void cleanList( std::list<std::string> &lst );

#endif
