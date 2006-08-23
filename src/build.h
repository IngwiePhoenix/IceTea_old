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

	/**
	 * Adds a target to the build.  If the target already exists, this will
	 * attempt to merge them as best it can.  If there are any conflicts, it
	 * will throw an exception.
	 *@param pTarget A pointer to a Target object that Build takes ownership of.
	 */
	void addTarget( Target *pTarget );
	void addRequires( const std::string &who, const std::string &what );

	void debugDump();

private:
	typedef std::map<std::string, Target *> TargetMap;
	typedef std::list<std::string> StringList;
	typedef std::map<std::string, StringList> ReqMap;

	TargetMap mTarget;
	ReqMap mRequires;

	//std::map<std::string, Rule *> mRule;
	//Action *pActDefault;
	//std::map<std::string, Action *> mAction;

};

#endif
