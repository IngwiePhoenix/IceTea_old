#ifndef BUILD_H
#define BUILD_H

#include <stdint.h>

#include <map>
#include <string>

#include "exceptions.h"
#include "rule.h"
#include "target.h"
#include "action.h"
#include "stringproc.h"
#include "cache.h"

subExceptionDecl( BuildException );
typedef std::map<std::string, std::string> VarMap;
typedef std::map<std::string, Target *> TargetMap;
typedef std::list<std::string> StringList;
typedef std::map<std::string, StringList> ReqMap;
typedef std::map<std::string, VarMap> ContextMap;
typedef std::map<std::string, Rule *> RuleMap;
typedef std::list<Rule *> RuleList;
typedef std::map<std::string, Action *> ActionMap;
typedef std::list<Target *> TargetList;
typedef std::map<std::string, TargetList> GroupMap;

class Viewer;

class Build
{
public:
	Build();
	virtual ~Build();

	void execAction( const std::string &sWhat );

	/**
	 * Adds a target to the build.  If the target already exists, this will
	 * attempt to merge them as best it can.  If there are any conflicts, it
	 * will throw an exception.
	 *@param pTarget A pointer to a Target object that Build takes ownership of.
	 */
	void addTarget( Target *pTarget );
	void addRequires( const std::string &who, const std::string &what );
	void addRule( Rule *pRule );
	void addAction( Action *pAction );
	bool hasAction( const std::string &str );
	void addToGroup( const std::string &sGroup, Target *pTarget );

	void set( const std::string &cont, const std::string &var, const std::string &val );
	void setAdd( const std::string &cont, const std::string &var, const std::string &val );
	void copyContext( const std::string &src, const std::string &dest );
	std::string getVar( const StringList *cont, const std::string &var, VarMap *mExtra );

	Rule *getRule( const std::string &name );

	void debugDump();

	void setStringProc( StringProc *pStrProc );
	std::string replVars( const std::string &sSrc, const StringList *pCont, VarMap *mExtra );
	RuleList findChainRules( Rule *pHead );
	StringList &getRequires( std::string sName );

	Viewer *getView()
	{
		return pView;
	}

	TargetMap &getTargetMap()
	{
		return mTarget;
	}

	void setView( const std::string &sView );

	void setCache( const std::string &sFileName );
	bool getCached( const std::string &sID, int nTime, StringList &lOut );
	void updateCache( const std::string &sID, FunctionList &lFunc, StringList &lOut );

	void chainTarget( const std::string &sName );

	void printInfo();

	void setMode( Action::eAction nAct );

private:
	TargetMap mTarget;
	ReqMap mRequires;
	VarMap mVars;
	ContextMap mContVars;
	RuleMap mRule;
	ActionMap mAction;
	StringProc *pStrProc;
	Viewer *pView;
	Cache cRequires;
	bool bCacheUpdated;
	std::string sCacheName;
	GroupMap mGroup;

	//std::map<std::string, Rule *> mRule;
	//Action *pActDefault;
	//std::map<std::string, Action *> mAction;

};

#endif
