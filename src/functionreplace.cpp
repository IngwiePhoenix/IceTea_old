#include "functionreplace.h"

FunctionReplace::FunctionReplace()
{
}

FunctionReplace::~FunctionReplace()
{
}

Bu::FString FunctionReplace::getName() const
{
	return "replace";
}

Variable FunctionReplace::call( Variable &input, VarList lParams )
{
	Bu::FString sA, sB;
	sA = lParams.first().getString();
	sB = lParams.last().getString();
	switch( input.getType() )
	{
		case Variable::typeString:
			{
				Variable vOut( input.getString().replace( sA, sB ) );
				return vOut;
			}
			break;

		case Variable::typeList:
			{
				Variable vOut( Variable::typeList );
				for( VarList::iterator i = input.begin(); i; i++ )
				{
					vOut.append( (*i).getString().replace( sA, sB ) );
				}
				return vOut;
			}
			break;

		default:
			break;
	}
	throw Bu::ExceptionBase(
		"replace does not work on non-string or non-list types.");
}

