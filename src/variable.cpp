#include "variable.h"
#include "astleaf.h"
#include "bu/sio.h"
using Bu::sio;

#include <stdlib.h>

Variable::Variable() :
	eType( typeNone )
{
	memset( &uVal, 0, sizeof(uVal) );
}

Variable::Variable( Type t ) :
	eType( t )
{
	memset( &uVal, 0, sizeof(uVal) );
	if( eType == typeString || eType == typeRef )
	{
		uVal.sVal = new Bu::String;
	}
	else if( eType == typeList )
	{
		uVal.lVal = new VarList;
	}
}

Variable::Variable( int iVal ) :
	eType( typeInt )
{
	memset( &uVal, 0, sizeof(uVal) );
	uVal.iVal = iVal;
}

Variable::Variable( double fVal ) :
	eType( typeFloat )
{
	memset( &uVal, 0, sizeof(uVal) );
	uVal.fVal = fVal;
}

Variable::Variable( bool bVal ) :
	eType( typeBool )
{
	memset( &uVal, 0, sizeof(uVal) );
	uVal.bVal = bVal;
}

Variable::Variable( const Bu::String &sVal ) :
	eType( typeString )
{
	memset( &uVal, 0, sizeof(uVal) );
	uVal.sVal = new Bu::String( sVal );
}

Variable::Variable( const char *sVal ) :
	eType( typeString )
{
	memset( &uVal, 0, sizeof(uVal) );
	uVal.sVal = new Bu::String( sVal );
}

Variable::Variable( const Variable &v ) :
	eType( v.eType )
{
	memset( &uVal, 0, sizeof(uVal) );
	if( eType == typeString || eType == typeRef )
	{
		uVal.sVal = new Bu::String( *v.uVal.sVal );
	}
	else if( eType == typeList )
	{
		uVal.lVal = new VarList( *v.uVal.lVal );
	}
	else
	{
		uVal = v.uVal;
	}
}
	
Variable::Variable( const class AstLeaf &l )
{
	switch( l.getDataType() )
	{
		case AstNode::typeDataInt:
			eType = typeInt;
			uVal.iVal = l.getIntValue();
			break;

		case AstNode::typeDataFloat:
			eType = typeFloat;
			uVal.fVal = l.getFloatValue();
			break;

		case AstNode::typeDataBool:
			eType = typeBool;
			uVal.bVal = l.getBoolValue();
			break;

		case AstNode::typeDataString:
			eType = typeString;
			uVal.sVal = new Bu::String( l.getStrValue() );
			break;

		case AstNode::typeDataNone:
			eType = typeNone;
			memset( &uVal, 0, sizeof(uVal) );
			break;

		default:
			sio << "Unhandled type <<!>>" << sio.nl << sio.nl;
			break;
	}
}

Variable::Variable( const StrList &lst )
{
	if( lst.getSize() == 1 )
	{
		eType = typeString;
		uVal.sVal = new Bu::String( lst.first() );
	}
	else
	{
		eType = typeList;
		uVal.lVal = new VarList();
		for( StrList::const_iterator i = lst.begin(); i; i++ )
		{
			uVal.lVal->append( Variable( *i ) );
		}
	}
}

Variable::Variable( const VarList &lst )
{
	eType = typeList;
	uVal.lVal = new VarList( lst );
}

Variable::~Variable()
{
	if( eType == typeString || eType == typeRef )
	{
		delete uVal.sVal;
	}
	else if( eType == typeList )
	{
		delete uVal.lVal;
	}
}

Variable Variable::mkRef( const Bu::String &sVal )
{
	Variable v( typeRef );
	(*v.uVal.sVal) = sVal;
	return v;
}

Variable::Type Variable::getType() const
{
	return eType;
}

int Variable::getInt() const
{
	if( eType != typeInt ) throw Bu::ExceptionBase("Wrong variable type.");
	return uVal.iVal;
}

double Variable::getFloat() const
{
	if( eType != typeFloat ) throw Bu::ExceptionBase("Wrong variable type.");
	return uVal.fVal;
}

bool Variable::getBool() const
{
	if( eType != typeBool ) throw Bu::ExceptionBase("Wrong variable type.");
	return uVal.bVal;
}

const Bu::String &Variable::getString() const
{
	if( eType != typeString && eType != typeRef ) throw Bu::ExceptionBase("Wrong variable type.");
	return *uVal.sVal;
}

const VarList &Variable::getList() const
{
	if( eType != typeList ) throw Bu::ExceptionBase("Wrong variable type.");
	return *uVal.lVal;
}

int Variable::toInt() const
{
	switch( eType )
	{
		case typeInt:
			return uVal.iVal;

		case typeFloat:
			return (int)uVal.fVal;

		case typeBool:
			return (uVal.bVal)?(1):(0);

		case typeString:
		case typeRef:
			return strtol( uVal.sVal->getStr(), NULL, 0 );

		default:
			return 0;
	}
	return 0;
}

double Variable::toFloat() const
{
	switch( eType )
	{
		case typeInt:
			return (double)uVal.iVal;

		case typeFloat:
			return uVal.fVal;

		case typeBool:
			return (uVal.bVal)?(1.0):(0.0);

		case typeString:
		case typeRef:
			return strtod( uVal.sVal->getStr(), NULL );

		default:
			return 0.0;
	}
	return 0.0;
}

bool Variable::toBool() const
{
	switch( eType )
	{
		case typeInt:
			return uVal.iVal != 0;

		case typeFloat:
			return uVal.fVal != 0.0;

		case typeBool:
			return uVal.bVal;

		case typeString:
		case typeRef:
			return (*uVal.sVal) == "true";

		case typeList:
			return !(*uVal.lVal).isEmpty();

		default:
			return false;
	}
	return false;
}

Bu::String Variable::toString() const
{
	Bu::String sRet;
	switch( eType )
	{
		case typeNone:
			// No type, no data, we return empty string
			break;

		case typeInt:
			sRet.format("%d", uVal.iVal );
			break;

		case typeFloat:
			sRet.format("%f", uVal.fVal );
			break;

		case typeBool:
			sRet = (uVal.bVal)?("true"):("false");
			break;

		case typeString:
		case typeRef:
			sRet = *uVal.sVal;
			break;

		case typeList:
			{
				for( VarList::const_iterator i = uVal.lVal->begin(); i; i++ )
				{
					if( i != uVal.lVal->begin() )
						sRet += " ";
					sRet += (*i).toString();
				}
			}
			break;

		case typeVersion:
			break;
	}

	return sRet;
}

VarList Variable::toList() const
{
	if( eType == typeList )
		return *this;
	return VarList( *this );
}

Variable Variable::toType( Type eNewType ) const
{
	switch( eNewType )
	{
		case typeNone:
			return Variable();

		case typeBool:
			return Variable( toBool() );

		case typeInt:
			return Variable( toInt() );

		case typeFloat:
			return Variable( toFloat() );

		case typeVersion:
			return Variable();

		case typeString:
			return Variable( toString() );

		case typeList:
			return Variable( toList() );
		
		case typeRef:
			return Variable::mkRef( toString() );
	}
	throw Bu::ExceptionBase("Unhandled case in Variable toType");
}

void Variable::append( const Variable &v )
{
	if( eType != typeList ) throw Bu::ExceptionBase("Wrong variable type.");

	if( v.eType == typeList )
	{
		uVal.lVal->append( *v.uVal.lVal );
	}
	else
	{
		uVal.lVal->append( v );
	}
}

VarList::iterator Variable::begin()
{
	if( eType != typeList ) throw Bu::ExceptionBase("Wrong variable type.");

	return uVal.lVal->begin();
}

VarList::const_iterator Variable::begin() const
{
	if( eType != typeList ) throw Bu::ExceptionBase("Wrong variable type.");
	
	return const_cast<const VarList *>(uVal.lVal)->begin();
}

void Variable::doNegate()
{
	switch( eType )
	{
		case typeNone:
			break;
		
		case typeBool:
			throw Bu::ExceptionBase("You cannot negate boolean values.");

		case typeInt:
			uVal.iVal = -uVal.iVal;
			break;

		case typeFloat:
			uVal.fVal = -uVal.fVal;
			break;
		
		case typeVersion:
			throw Bu::ExceptionBase("You cannot negate version values.");
		
		case typeString:
			throw Bu::ExceptionBase("You cannot negate string values.");
		
		case typeList:
			throw Bu::ExceptionBase("You cannot negate list values.");
		
		case typeRef:
			throw Bu::ExceptionBase("You cannot negate reference values.");
	}
}

void Variable::doNot()
{
	bool bVal = !toBool();
	reset( typeBool );
	uVal.bVal = bVal;
}

const Variable &Variable::operator=( const Variable &rhs )
{
	reset( rhs.eType );
	if( rhs.eType == typeString || rhs.eType == typeRef )
	{
		uVal.sVal = new Bu::String( *rhs.uVal.sVal );
	}
	else if( rhs.eType == typeList )
	{
		uVal.lVal = new VarList( *rhs.uVal.lVal );
	}
	else
	{
		uVal = rhs.uVal;
	}
	
	return *this;
}

const Variable &Variable::operator=( const int &rhs )
{
	reset( typeInt );
	uVal.iVal = rhs;

	return *this;
}

const Variable &Variable::operator=( const double &rhs )
{
	reset( typeFloat );
	uVal.fVal = rhs;

	return *this;
}

const Variable &Variable::operator=( const bool &rhs )
{
	reset( typeBool );
	uVal.bVal = rhs;

	return *this;
}

const Variable &Variable::operator=( const Bu::String &rhs )
{
	reset( typeString );
	uVal.sVal = new Bu::String( rhs );

	return *this;
}

const Variable &Variable::operator+=( const Variable &rhs )
{
	switch( eType )
	{
		case typeNone:
			reset( rhs.eType );
			if( eType == typeString || eType == typeRef )
			{
				uVal.sVal = new Bu::String( *rhs.uVal.sVal );
			}
			else if( eType == typeList )
			{
				uVal.lVal = new VarList( *rhs.uVal.lVal );
			}
			else
			{
				uVal = rhs.uVal;
			}
			break;

		case typeInt:
			uVal.iVal += rhs.getInt();
			break;

		case typeFloat:
			uVal.fVal += rhs.getFloat();
			break;

		case typeBool:
			throw Bu::ExceptionBase("Can't += with a boolean...");
			break;

		case typeString:
			uVal.sVal->append(" ");
			uVal.sVal->append( rhs.getString() );
			break;

		case typeList:
			uVal.lVal->append( rhs.getList() );
			break;

		case typeVersion:
			break;

		default:
			break;
	}
	return *this;
}

const Variable &Variable::operator<<( const Variable &rhs )
{
	switch( eType )
	{
		case typeNone:
			reset( rhs.eType );
			if( eType == typeString )
			{
				uVal.sVal = new Bu::String( *rhs.uVal.sVal );
			}
			else if( eType == typeList )
			{
				uVal.lVal = new VarList( *rhs.uVal.lVal );
			}
			else
			{
				uVal = rhs.uVal;
			}
			break;

		case typeString:
			uVal.sVal->append( rhs.getString() );
			break;

		case typeList:
			uVal.lVal->append( rhs.getList() );
			break;

		default:
			throw Bu::ExceptionBase("Can't << with non-string or non-list.");
			break;
	}
	return *this;
}

bool Variable::operator==( const Variable &rhs ) const
{
	if( eType != rhs.eType )
		return false;
	switch( eType )
	{
		case typeNone:
			return true;

		case typeInt:
			return uVal.iVal == rhs.uVal.iVal;

		case typeFloat:
			return uVal.fVal == rhs.uVal.fVal;

		case typeBool:
			return uVal.bVal == rhs.uVal.bVal;

		case typeString:
		case typeRef:
			return *uVal.sVal == *rhs.uVal.sVal;

		case typeList:
			return *uVal.lVal == *rhs.uVal.lVal;

		case typeVersion:
			return false;
	}

	return false;
}

bool Variable::operator!=( const Variable &rhs ) const
{
	return !(*this == rhs);
}

bool Variable::operator<( const Variable &rhs ) const
{
	Type eTop = Bu::max( eType, rhs.eType );
	switch( eTop )
	{
		case typeNone:
			return false;
		
		case typeBool:
			throw Bu::ExceptionBase("You cannot < compare boolean values.");

		case typeInt:
			return toInt() < rhs.toInt();

		case typeFloat:
			return toFloat() < rhs.toFloat();
		
		case typeVersion:
			return true;
		
		case typeString:
			return toString() < rhs.toString();
		
		case typeList:
			throw Bu::ExceptionBase("You cannot < compare list values.");
		
		case typeRef:
			throw Bu::ExceptionBase("You cannot < compare reference values.");
	}
	throw Bu::ExceptionBase("Unhandled case in Variable < compare");
}

bool Variable::operator>( const Variable &rhs ) const
{
	Type eTop = Bu::max( eType, rhs.eType );
	switch( eTop )
	{
		case typeNone:
			return false;
		
		case typeBool:
			throw Bu::ExceptionBase("You cannot > compare boolean values.");

		case typeInt:
			return toInt() > rhs.toInt();

		case typeFloat:
			return toFloat() > rhs.toFloat();
		
		case typeVersion:
			return true;
		
		case typeString:
			return toString() > rhs.toString();
		
		case typeList:
			throw Bu::ExceptionBase("You cannot > compare list values.");
		
		case typeRef:
			throw Bu::ExceptionBase("You cannot > compare reference values.");
	}
	throw Bu::ExceptionBase("Unhandled case in Variable > compare");
}

bool Variable::operator<=( const Variable &rhs ) const
{
	Type eTop = Bu::max( eType, rhs.eType );
	switch( eTop )
	{
		case typeNone:
			return false;
		
		case typeBool:
			throw Bu::ExceptionBase("You cannot <= compare boolean values.");

		case typeInt:
			return toInt() <= rhs.toInt();

		case typeFloat:
			return toFloat() <= rhs.toFloat();
		
		case typeVersion:
			return true;
		
		case typeString:
			return toString() <= rhs.toString();
		
		case typeList:
			throw Bu::ExceptionBase("You cannot <= compare list values.");
		
		case typeRef:
			throw Bu::ExceptionBase("You cannot <= compare reference values.");
	}
	throw Bu::ExceptionBase("Unhandled case in Variable <= compare");
}

bool Variable::operator>=( const Variable &rhs ) const
{
	Type eTop = Bu::max( eType, rhs.eType );
	switch( eTop )
	{
		case typeNone:
			return false;
		
		case typeBool:
			throw Bu::ExceptionBase("You cannot >= compare boolean values.");

		case typeInt:
			return toInt() >= rhs.toInt();

		case typeFloat:
			return toFloat() >= rhs.toFloat();
		
		case typeVersion:
			return true;
		
		case typeString:
			return toString() >= rhs.toString();
		
		case typeList:
			throw Bu::ExceptionBase("You cannot >= compare list values.");
		
		case typeRef:
			throw Bu::ExceptionBase("You cannot >= compare reference values.");
	}
	throw Bu::ExceptionBase("Unhandled case in Variable >= compare");
}

Variable Variable::operator+( const Variable &rhs ) const
{
	Type eTop = Bu::max( eType, rhs.eType );
	switch( eTop )
	{
		case typeNone:
			return Variable();
		
		case typeBool:
			throw Bu::ExceptionBase("You cannot add boolean values.");

		case typeInt:
			return Variable( toInt() + rhs.toInt() );

		case typeFloat:
			return Variable( toFloat() + rhs.toFloat() );
		
		case typeVersion:
			throw Bu::ExceptionBase("You cannot add version values.");
		
		case typeString:
			return Variable( toString() + rhs.toString() );
		
		case typeList:
			return Variable( toList() + rhs.toList() );
		
		case typeRef:
			throw Bu::ExceptionBase("You cannot add reference values.");
	}
	throw Bu::ExceptionBase("Unhandled case in Variable add");
}

Variable Variable::operator-( const Variable &rhs ) const
{
	Type eTop = Bu::max( eType, rhs.eType );
	switch( eTop )
	{
		case typeNone:
			return Variable();
		
		case typeBool:
			throw Bu::ExceptionBase("You cannot subtract boolean values.");

		case typeInt:
			return Variable( toInt() - rhs.toInt() );

		case typeFloat:
			return Variable( toFloat() - rhs.toFloat() );
		
		case typeVersion:
			throw Bu::ExceptionBase("You cannot subtract version values.");
		
		case typeString:
			throw Bu::ExceptionBase("You cannot subtract string values.");
		
		case typeList:
			throw Bu::ExceptionBase("You cannot subtract list values.");
		
		case typeRef:
			throw Bu::ExceptionBase("You cannot subtract reference values.");
	}
	throw Bu::ExceptionBase("Unhandled case in Variable subtract");
}

Variable Variable::operator*( const Variable &rhs ) const
{
	Type eTop = Bu::max( eType, rhs.eType );
	switch( eTop )
	{
		case typeNone:
			return Variable();
		
		case typeBool:
			throw Bu::ExceptionBase("You cannot multiply boolean values.");

		case typeInt:
			return Variable( toInt() * rhs.toInt() );

		case typeFloat:
			return Variable( toFloat() * rhs.toFloat() );
		
		case typeVersion:
			throw Bu::ExceptionBase("You cannot multiply version values.");
		
		case typeString:
			throw Bu::ExceptionBase("You cannot multiply string values.");
		
		case typeList:
			throw Bu::ExceptionBase("You cannot multiply list values.");
		
		case typeRef:
			throw Bu::ExceptionBase("You cannot multiply reference values.");
	}
	throw Bu::ExceptionBase("Unhandled case in Variable multiply");
}

Variable Variable::operator/( const Variable &rhs ) const
{
	Type eTop = Bu::max( eType, rhs.eType );
	switch( eTop )
	{
		case typeNone:
			return Variable();
		
		case typeBool:
			throw Bu::ExceptionBase("You cannot divide boolean values.");

		case typeInt:
			return Variable( toInt() / rhs.toInt() );

		case typeFloat:
			return Variable( toFloat() / rhs.toFloat() );
		
		case typeVersion:
			throw Bu::ExceptionBase("You cannot divide version values.");
		
		case typeString:
			throw Bu::ExceptionBase("You cannot divide string values.");
		
		case typeList:
			throw Bu::ExceptionBase("You cannot divide list values.");
		
		case typeRef:
			throw Bu::ExceptionBase("You cannot divide reference values.");
	}
	throw Bu::ExceptionBase("Unhandled case in Variable divide");
}

void Variable::reset( Type eNewType )
{
	if( eType == typeString || eType == typeRef )
	{
		delete uVal.sVal;
	}
	else if( eType == typeList )
	{
		delete uVal.lVal;
	}
	memset( &uVal, 0, sizeof(uVal) );

	eType = eNewType;
}

Bu::Formatter &operator<<( Bu::Formatter &f, const Variable::Type &t )
{
	switch( t )
	{
		case Variable::typeNone:	f << "*typeless*";	break;
		case Variable::typeInt:		f << "int";			break;
		case Variable::typeFloat:	f << "double";		break;
		case Variable::typeBool:	f << "bool";		break;
		case Variable::typeString:	f << "string";		break;
		case Variable::typeList:	f << "list";		break;
		case Variable::typeVersion:	f << "version";		break;
		case Variable::typeRef:		f << "ref";			break;
	}
	return f;
}

Bu::Formatter &operator<<( Bu::Formatter &f, const Variable &v )
{
	f << "Variable(" << v.getType() << ") = ";
	switch( v.getType() )
	{
		case Variable::typeNone:							break;
		case Variable::typeInt:		f << v.getInt();		break;
		case Variable::typeFloat:	f << v.getFloat();		break;
		case Variable::typeBool:	f << v.getBool();		break;
		case Variable::typeString:	f << v.getString();		break;
		case Variable::typeList:	f << v.getList();		break;
		case Variable::typeVersion:/*f << v.getVersion();*/	break;
		case Variable::typeRef:		f << v.getString();		break;
	}

	return f;
}

Bu::ArchiveBase &operator<<( Bu::ArchiveBase &ar, const Variable &v )
{
	ar << (int8_t)v.eType;
	switch( v.eType )
	{
		case Variable::typeNone:
			break;

		case Variable::typeBool:
			ar << v.uVal.bVal;
			break;

		case Variable::typeInt:
			ar << v.uVal.iVal;
			break;

		case Variable::typeFloat:
			ar << v.uVal.fVal;
			break;

		case Variable::typeVersion:
			break;

		case Variable::typeString:
			ar << *v.uVal.sVal;
			break;

		case Variable::typeList:
			ar << *v.uVal.lVal;
			break;

		case Variable::typeRef:
			ar << *v.uVal.sVal;
			break;
	}

	return ar;
}

Bu::ArchiveBase &operator>>( Bu::ArchiveBase &ar, Variable &v )
{
	int8_t t;
	ar >> t;
	v.eType = (Variable::Type)t;
	v.reset( v.eType );
	switch( v.eType )
	{
		case Variable::typeNone:
			break;

		case Variable::typeBool:
			ar >> v.uVal.bVal;
			break;

		case Variable::typeInt:
			ar >> v.uVal.iVal;
			break;

		case Variable::typeFloat:
			ar >> v.uVal.fVal;
			break;

		case Variable::typeVersion:
			break;

		case Variable::typeString:
			ar >> *v.uVal.sVal;
			break;

		case Variable::typeList:
			ar >> *v.uVal.lVal;
			break;

		case Variable::typeRef:
			ar >> *v.uVal.sVal;
			break;
	}
	
	return ar;
}

