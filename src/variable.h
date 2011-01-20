#ifndef VARIABLE_H
#define VARIABLE_H

#include "types.h"
#include <bu/archivebase.h>

/**
 * A build variable, which is basically a flexible, limited type range variant.
 */
class Variable
{
friend Bu::ArchiveBase &operator<<( Bu::ArchiveBase &ar, const Variable &v );
friend Bu::ArchiveBase &operator>>( Bu::ArchiveBase &ar, Variable &v );
public:
	enum Type
	{
		typeNone,
		typeBool,
		typeInt,
		typeFloat,
		typeVersion,
		typeString,
		typeList,
		typeRef			/**< Reference by name, it's just a string. */
	};

public:
	Variable();
	Variable( Type t );
	Variable( int iVal );
	Variable( double fVal );
	Variable( bool bVal );
	Variable( const Bu::String &sVal );
	Variable( const char *sVal );
	Variable( const Variable &v );
	Variable( const class AstLeaf &l );
	/**
	 * This special case function turns the variable into a string if there is
	 * only one string in the list, or a list of strings if there is more or
	 * less than one.
	 */
	Variable( const StrList &lst );
	Variable( const VarList &lst );
	virtual ~Variable();

	static Variable mkRef( const Bu::String &sVal );

	Type getType() const;

	// Raw aquisition functions, if the type isn't right,
	// they throw an exception
	int getInt() const;
	double getFloat() const;
	bool getBool() const;
	const Bu::String &getString() const;
	const VarList &getList() const;

	// Conversion functions, they'll return the requested type, maybe an error
	// if the source data is really bad
	int toInt() const;
	double toFloat() const;
	bool toBool() const;
	Bu::String toString() const;
	VarList toList() const;

	Variable toType( Type eNewType ) const;

	void append( const Variable &v );
	VarList::iterator begin();
	VarList::const_iterator begin() const;

	void doNegate();
	void doNot();

	const Variable &operator=( const Variable &rhs );
	const Variable &operator=( const int &rhs );
	const Variable &operator=( const double &rhs );
	const Variable &operator=( const bool &rhs );
	const Variable &operator=( const Bu::String &rhs );

	const Variable &operator+=( const Variable &rhs );
	const Variable &operator<<( const Variable &rhs );

	bool operator==( const Variable &rhs ) const;
	bool operator!=( const Variable &rhs ) const;
	bool operator<( const Variable &rhs ) const;
	bool operator>( const Variable &rhs ) const;
	bool operator<=( const Variable &rhs ) const;
	bool operator>=( const Variable &rhs ) const;

	Variable operator+( const Variable &rhs ) const;
	Variable operator-( const Variable &rhs ) const;
	Variable operator*( const Variable &rhs ) const;
	Variable operator/( const Variable &rhs ) const;

private:
	Type eType;
	union
	{
		int iVal;
		double fVal;
		bool bVal;
		Bu::String *sVal;
		VarList *lVal;
	} uVal;

	void reset( Type eType );
};

namespace Bu
{
	class Formatter;
}

Bu::Formatter &operator<<( Bu::Formatter &f, const Variable::Type &t );
Bu::Formatter &operator<<( Bu::Formatter &f, const Variable &v );

Bu::ArchiveBase &operator<<( Bu::ArchiveBase &ar, const Variable &v );
Bu::ArchiveBase &operator>>( Bu::ArchiveBase &ar, Variable &v );

#endif
