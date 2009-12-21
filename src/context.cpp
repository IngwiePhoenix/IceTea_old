#include "context.h"
#include "target.h"
#include "rule.h"
#include "function.h"
#include "runner.h"
#include "action.h"
#include "profile.h"
#include "view.h"

#include "functionreplace.h"
#include "functionexists.h"
#include "functionfiles.h"
#include "functionexecute.h"
#include "functionmatches.h"
#include "functiontostring.h"
#include "functionunlink.h"
#include "functiontargets.h"
#include "functiondirs.h"
#include "functiongetmakedeps.h"
#include "functionfilename.h"
#include "functiondirname.h"

#include <bu/process.h>
#include <bu/sio.h>
using namespace Bu;

Context::Context() :
	pView( NULL )
{
	addFunction( new FunctionReplace() );
	addFunction( new FunctionExists() );
	addFunction( new FunctionFiles() );
	addFunction( new FunctionExecute() );
	addFunction( new FunctionMatches() );
	addFunction( new FunctionToString() );
	addFunction( new FunctionUnlink() );
	addFunction( new FunctionTargets() );
	addFunction( new FunctionDirs() );
	addFunction( new FunctionGetMakeDeps() );
	addFunction( new FunctionFileName() );
	addFunction( new FunctionDirName() );
	pushScope();
}

Context::~Context()
{
}

void Context::addTarget( Target *pTarget )
{
	for( StrList::const_iterator i = pTarget->getOutputList().begin(); i; i++ )
	{
		hTarget.insert( (*i).getStr(), pTarget );
	}
}

void Context::addRule( Rule *pRule )
{
	hRule.insert( pRule->getName(), pRule );
}

void Context::addFunction( Function *pFunction )
{
	pFunction->setContext( this );
	hFunction.insert( pFunction->getName(), pFunction );
}

void Context::addVariable( const Bu::FString &sName, const Variable &vValue )
{
	for( ScopeStack::iterator i = sVars.begin(); i; i++ )
	{
		if( (*i).has( sName ) )
		{
//			sio << "Replacing higher scope variable \"" << sName << "\" with value \"" << (*i).get( sName ) << "\" with new value \"" << vValue << "\"" << sio.nl;
			(*i).insert( sName, vValue );
			return;
		}
	}
	sVars.first().insert( sName, vValue );
}

void Context::addAction( Action *pAction )
{
	hAction.insert( pAction->getName(), pAction );
}

Action *Context::getAction( const Bu::FString &sName )
{
	return hAction.get( sName );
}

void Context::addTargetToTag( Target *pTarget, const Bu::FString &sTag )
{
	if( !hTag.has( sTag ) )
	{
		hTag.insert( sTag, TargetList() );
	}
	hTag.get( sTag ).append( pTarget );
}

void Context::addTargetToTags( Target *pTarget, const StrList &sTags )
{
	for( StrList::const_iterator i = sTags.begin(); i; i++ )
	{
		addTargetToTag( pTarget, *i );
	}
}

TargetList &Context::getTag( const Bu::FString &sTag )
{
	return hTag.get( sTag );
}

Variable &Context::getVariable( const Bu::FString &sName )
{
	for( ScopeStack::iterator i = sVars.begin(); i; i++ )
	{
		if( (*i).has( sName ) )
		{
			return (*i).get( sName );
		}
	}
	throw Bu::ExceptionBase("No such variable.");
}

void Context::delVariable( const Bu::FString &sName )
{
	for( ScopeStack::iterator i = sVars.begin(); i; i++ )
	{
		if( (*i).has( sName ) )
		{
			(*i).erase( sName );
		}
	}
}

void Context::pushScope()
{
	VarHash h;
	if( !sVars.isEmpty() )
		h = sVars.peek();
	sVars.push( h );
}

void Context::pushScope( const VarHash &hNewVars )
{
	VarHash h = hNewVars;
	if( !sVars.isEmpty() )
	{
		for( VarHash::iterator i = sVars.peek().begin(); i; i++ )
		{
			if( !h.has( i.getKey() ) )
				h.insert( i.getKey(), i.getValue() );
		}
	}
	sVars.push( h );
}

VarHash &Context::getScope()
{
	return sVars.first();
}

void Context::popScope()
{
	sVars.pop();
}

Variable Context::call( const Bu::FString &sName, Variable &input,
		VarList lParams )
{
	if( !hFunction.has( sName ) )
	{
		throw Bu::ExceptionBase("Unknown function called: %s", sName.getStr() );
	}
	return hFunction.get( sName )->call( input, lParams );
}

#include <bu/sio.h>
using namespace Bu;
Bu::FString Context::expand( const Bu::FString &sInS )
{
	Bu::FString sRet;
	Bu::FString sIn = sInS;

	for( int iPass = 0; iPass < 2; iPass++ )
	{
		Bu::FString::const_iterator b = sIn.begin();
		sRet.clear();
		for(;;)
		{
			Bu::FString::const_iterator e = b.find('$');
			if( !e )
			{
				sRet.append( b );
				break;
			}
			sRet.append( b, e );
			b = e+1;
			if( !b )
			{
				sRet.append('$');
			}
			else if( *b == '{' )
			{
				b++;
				e = b.find('}');
				Bu::FString sVar( b, e );
				try
				{
					sRet.append( getVariable( sVar ).toString() );
				} catch(...)
				{
					// TODO: This may be handy debugging later...
					//sio << "No variable named " << sVar << sio.nl;
					//sio << "Vars: " << sVars << sio.nl << sio.nl;
				}
				b = e+1;
			}
			else if( *b == '(' && iPass == 1 )
			{
				b++;
				e = b.find(')');
				Bu::FString sCmd( b, e );
				Bu::FString sBuf;
				try
				{
					//sio << "Executing command:  >>>" << sCmd << "<<<" << sio.nl;
					Process p( Process::StdOut, "/bin/bash", "/bin/bash", "-c", sCmd.getStr(), NULL );
					while( p.isRunning() )
					{
						char buf[4096];
						sBuf.append( buf, p.read( buf, 4096 ) );
					}
					sBuf = sBuf.replace("\n", " ").replace("\r", " ");
					sBuf.trimBack(' ');
					sRet.append( sBuf );
				} catch(...)
				{
					// TODO: This may be handy debugging later...
					//sio << "No variable named " << sVar << sio.nl;
					//sio << "Vars: " << sVars << sio.nl << sio.nl;
				}
				b = e+1;
			}
			else
			{
				// Not a match, uh, just output the $ for now...
				sRet.append('$');
			}
		}

		sIn = sRet;
	}
	return sRet;
}

Target *Context::getTarget( const Bu::FString &sOutput )
{
	return hTarget.get( sOutput );
}

TargetList Context::getExplicitTargets()
{
	TargetList lRet;
	for( TargetHash::iterator i = hTarget.begin(); i; i++ )
	{
		if( (*i)->isExplicit() )
			lRet.append( *i );
	}
	return lRet;
}

void Context::buildTargetTree( Runner &r )
{
	TargetList lTargets = hTarget.getValues();

	for( TargetList::iterator i = lTargets.begin(); i; i++ )
	{
		// I believe we only want to autogenerate targets for explicit targets
		// that have rules defined.
		if( !(*i)->isExplicit() || !(*i)->hasRule() )
			continue;

		StrList lNewIns; // The new "changed" inputs for this target

		Rule *pMaster = hRule.get( (*i)->getRule() );

		for( StrList::const_iterator j = (*i)->getInputList().begin(); j; j++ )
		{
			if( pMaster->ruleMatches( r, *j ) )
			{
				lNewIns.append( *j );
			}

			if( hTarget.has( *j ) )
			{
				// Find the existing dependancy
				lNewIns.append( *j );
			}
			//else
			//{
				buildTargetTree( r, *i, *j, pMaster, lNewIns );
			//}
		}

		pMaster->prepTarget( *i );
		(*i)->resetInputList( lNewIns );
	}
	//sio << "Building dependancies: " << Fmt(3) << 0 << "%\r" << sio.flush;
	//int iSize = hTarget.getSize(), iCur = 0;
	for( TargetHash::iterator i = hTarget.begin(); i; i++ )
	{
		// Before we can take a look at the requirements, we need to build
		// them...
	//	(*i)->buildRequires( r );

		// For every target we have to examine both it's inputs and it's
		// additional requirements.  Inputs first
		StrList lDeps( (*i)->getInputList() );
		lDeps += (*i)->getRequiresList();
		for( StrList::const_iterator j = lDeps.begin(); j; j++ )
		{
			try
			{
				(*i)->addDep( hTarget.get( *j ) );
			}
			catch(...)
			{
			}
		}
		//iCur++;
	//	sio << "Building dependancies: " << Fmt(3) << (iCur*100/iSize) << "%\r" << sio.flush;
		(*i)->collapseDeps();
	}
//	sio << sio.nl;

	for( TargetHash::iterator i = hTarget.begin(); i; i++ )
	{
		if( !(*i)->isExplicit() )
			continue;
		(*i)->setDepCount();
		(*i)->resetRun( false );
	}
}

void Context::buildTargetTree( class Runner &r, class Target * /*pTarget*/, const Bu::FString &sInput, Rule *pMaster, StrList &lNewIns )
{
	Target *pNewTarget = NULL;
	for( RuleHash::iterator i = hRule.begin(); i; i++ )
	{
		if( (*i)->hasOutputs() && (*i)->ruleMatches( r, sInput ) )
		{
			pNewTarget = (*i)->createTarget( r, sInput );

			Bu::Hash<ptrdiff_t, bool> hDone;
			for( StrList::const_iterator oi =
				 pNewTarget->getOutputList().begin(); oi; oi++ )
			{
				try
				{
					Target *pOver = hTarget.get( *oi );
					if( hDone.has( (ptrdiff_t)pOver ) )
						continue;
					hDone.insert( (ptrdiff_t)pOver, true );
					if( !pOver->isExplicit() )
					{
						delete pNewTarget;
						pNewTarget = pOver;
						break;
					}
					pOver->mergeUnder( pNewTarget );
					delete pNewTarget;
//					sio << "Delete: " << Fmt::ptr() << (ptrdiff_t)pNewTarget << sio.nl;
					pNewTarget = pOver;
					break;
				}
				catch(...)
				{
				}
			}

			// We actually want to add this either way, if the merge added new
			// outputs, then we need to take them into account.
			addTarget( pNewTarget );
			addTargetToTags( pNewTarget, (*i)->getTagList() ); 

			// We have created a new target (or not, either way, we need to
			// check if it matches.)
			for( StrList::const_iterator m =
				 pNewTarget->getOutputList().begin(); m; m++ )
			{
				// Does the new output match the master rule?
				if( pMaster->ruleMatches( r, (*m) ) )
				{
					lNewIns.append( (*m) );

//					sio << "What?" << sio.nl;
					// These relationships are difficult to pick up on except
					// that one target was created by the other, I suppose.
					// Anyway, that means that we need to add this while we
					// can.
//					pTarget->addDep( pNewTarget );
				}
	//			else
	//			{
					buildTargetTree( r, pNewTarget, *m, pMaster, lNewIns );
	//			}
			}

			return;
		}
	}
	if( !pNewTarget )
	{
		//sio << "Incomplete tree created, trying to find purpose for \""
		//	<< sInput << "\"." << sio.nl;
		return;
	}
}

void Context::attachDefaults()
{
	for( TargetHash::iterator i = hTarget.begin(); i; i++ )
	{
		if( !(*i)->hasProfile("clean") )
		{
			(*i)->addProfile( Profile::genDefaultClean() );
		}
	}
}

void Context::genDefaultActions()
{
	if( !hAction.has("all") )
	{
		addAction( Action::genDefaultAll() );
	}
	if( !hAction.has("clean") )
	{
		addAction( Action::genDefaultClean() );
	}
	if( !hAction.has("clean-all") )
	{
		addAction( Action::genDefaultCleanAll() );
	}
	if( !hAction.has("default") )
	{
		addAction( Action::genDefaultDefault() );
	}
}

void Context::writeTargetDot()
{
	Bu::Hash<ptrdiff_t, bool> hDone;
	sio << "digraph {" << sio.nl
		<< "\trankdir=LR;" << sio.nl;
	for( TargetHash::iterator i = hTarget.begin(); i; i++ )
	{
		if( hDone.has( (ptrdiff_t)*i ) )
			continue;
		hDone.insert( (ptrdiff_t)*i, true );
		for( StrList::const_iterator l = (*i)->getOutputList().begin();
			 l; l++ )
		{
			for( StrList::const_iterator k = (*i)->getInputList().begin();
				 k; k++ )
			{
				sio << "\t\"" << *k << "\" -> \""
					<< *l << "\";" << sio.nl;
			}
			for( StrList::const_iterator k = (*i)->getRequiresList().begin();
				 k; k++ )
			{
				sio << "\t\"" << *k << "\" -> \""
					<< *l << "\" [color=red];" << sio.nl;
			}
		}

	}
	sio << "}" << sio.nl;
}

void Context::setView( View *pNewView )
{
	delete pView;
	pView = pNewView;
}

View *Context::getView()
{
	return pView;
}

Bu::Formatter &operator<<( Bu::Formatter &f, const Context &c )
{
	f << "Variables:  " << c.sVars << f.nl;
	f << "Targets: " << c.hTarget << f.nl;
	f << "Rules: " << c.hRule << f.nl;

	return f;
}

void Context::printBasicInfo()
{
	sio << "Available actions:" << sio.nl << "\t";
	for( ActionHash::iterator i = hAction.begin(); i; i++ )
	{
		if( i != hAction.begin() )
			sio << ", ";
		sio << i.getKey();
	}
	
	TargetList lTargets = getExplicitTargets();
	sio << sio.nl << sio.nl << "Available targets:" << sio.nl << "\t";
	for( TargetList::iterator i = lTargets.begin(); i; i++ )
	{
		if( i != lTargets.begin() )
			sio << ", ";
		for( StrList::const_iterator j = (*i)->getOutputList().begin(); j; j++ )
		{
			if( j != (*i)->getOutputList().begin() )
				sio << ", ";
			sio << (*j);
		}
	}

	sio << sio.nl << sio.nl;
}

