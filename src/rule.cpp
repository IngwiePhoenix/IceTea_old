#include "rule.h"
#include "target.h"
#include "astbranch.h"
#include "astleaf.h"
#include "runner.h"
#include "variable.h"
#include "context.h"
#include "condition.h"
#include "profile.h"

#include <bu/sio.h>
using namespace Bu;

Rule::Rule( const Bu::FString &sName ) :
	sName( sName ),
	pInput( NULL )
{
}

Rule::~Rule()
{
}

const Bu::FString &Rule::getName() const
{
	return sName;
}

void Rule::setInput( const AstBranch *pNewInput )
{
	pInput = pNewInput;
}

const AstBranch *Rule::getInput() const
{
	return pInput;
}

bool Rule::hasOutputs() const
{
	return !lOutput.isEmpty();
}

void Rule::addOutput( const AstBranch *pNewOutput )
{
	lOutput.append( pNewOutput );
}

void Rule::addProfile( const AstBranch *pProfRoot )
{
	Profile *pProf = new Profile( pProfRoot );
	hProfiles.insert( pProf->getName(), pProf );
	/*
	hProfiles.insert(
		dynamic_cast<const AstLeaf *>(
			(*pProfile->getBranchBegin()).first()
			)->getStrValue(),
		pProfile
		);
		*/
}

void Rule::prepTarget( class Target *pTarget )
{
	pTarget->setDisplay( getDisplay() );
	for( ProfileHash::iterator i = hProfiles.begin(); i; i++ )
	{
		pTarget->addProfile( *i );
	}
	for( AstBranchList::iterator i = lRequires.begin(); i; i++ )
	{
		pTarget->addRequires( *i );
	}
}

Target *Rule::createTarget( class Runner &r, const Bu::FString &sInput,
		Target *pParent )
{
	r.getContext().pushScope( pParent->getVars() );
	r.getContext().addVariable("INPUT", sInput );
	Target *pTrg = new Target( false );
	for( AstBranchList::iterator i = lOutput.begin(); i; i++ )
	{
		Variable vOut = r.execExpr(
			(*(*i)->getBranchBegin()).begin(),
			Variable( sInput )
			);
		if( vOut.getType() == Variable::typeString )
		{
			pTrg->addOutput( vOut.getString() );
		}
		else if( vOut.getType() == Variable::typeList )
		{
			for( VarList::iterator j = vOut.begin(); j; j++ )
			{
				pTrg->addOutput( (*j).getString() );
			}
		}
	}
	r.getContext().addVariable("OUTPUT", pTrg->getOutputList() );
	pTrg->addInput( sInput );
	pTrg->setDisplay( getDisplay() );
	for( ProfileHash::iterator i = hProfiles.begin(); i; i++ )
	{
		pTrg->addProfile( *i );
	}
	for( AstBranchList::iterator i = lRequires.begin(); i; i++ )
	{
		pTrg->addRequires( *i );
	}
	pTrg->setVars( r.getContext().getScope() );
	r.getContext().popScope();

	return pTrg;
}

bool Rule::ruleMatches( Runner &r, const Bu::FString &sInput )
{
	r.getContext().pushScope();
	r.getContext().addVariable("INPUT", sInput );
	Variable vInput( sInput );
	Variable vOut = r.execExpr(
		(*pInput->getBranchBegin()).begin(),
		vInput
		);
	r.getContext().popScope();
	if( vOut.getType() == Variable::typeBool )
		return vOut.getBool();
	else if( vOut.getType() == Variable::typeList )
		return vOut.begin();
	return false;
}

void Rule::addTag( const Bu::FString &sTag )
{
	lsTags.append( sTag );
}

const StrList &Rule::getTagList() const
{
	return lsTags;
}

void Rule::setDisplay( const Bu::FString &sStr )
{
	sDisplay = sStr;
}

const Bu::FString &Rule::getDisplay() const
{
	return ((bool)sDisplay)?(sDisplay):(sName);
}

void Rule::addRequires( const AstBranch *pBr )
{
	lRequires.append( pBr );
}

Bu::Formatter &operator<<( Bu::Formatter &f, const Rule &/*t*/ )
{
	return f << "rule";
}

template<> Bu::Formatter &Bu::operator<< <Rule>( Bu::Formatter &f, const Rule *t )
{
	return f << (*t);
}

