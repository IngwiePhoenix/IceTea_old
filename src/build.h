#ifndef BUILD_H
#define BUILD_H

#include <stdint.h>

#include <map>
#include <string>

#include "exceptions.h"
#include "rule.h"
#include "target.h"
#include "action.h"

subExceptionDecl( BuildException );

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

	void set( const std::string &cont, const std::string &var, const std::string &val );
	void setAdd( const std::string &cont, const std::string &var, const std::string &val );
	std::string getVar( const std::string &cont, const std::string &var );

	void debugDump();

private:
	typedef std::map<std::string, Target *> TargetMap;
	typedef std::list<std::string> StringList;
	typedef std::map<std::string, StringList> ReqMap;
	typedef std::map<std::string, std::string> VarMap;
	typedef std::map<std::string, VarMap> ContextMap;
	typedef std::map<std::string, Rule *> RuleMap;
	typedef std::map<std::string, Action *> ActionMap;

	TargetMap mTarget;
	ReqMap mRequires;
	VarMap mVars;
	ContextMap mContVars;
	RuleMap mRule;
	ActionMap mAction;

	//std::map<std::string, Rule *> mRule;
	//Action *pActDefault;
	//std::map<std::string, Action *> mAction;

};

#endif
