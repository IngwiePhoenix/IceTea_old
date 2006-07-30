#include <iostream>

#include "builder.h"
#include "build.tab.h"

Builder::Builder()
{
}

Builder::~Builder()
{
}

void Builder::load( const char *sFN )
{
	file = sFN;

	scanBegin();
	yy::BuildParser parser( *this );
	parser.set_debug_level( false );
	parser.parse();
	scanEnd();
}

void Builder::error( const yy::location &l, const std::string &m )
{
	std::cerr << l << ": " << m << std::endl;
}

void Builder::error( const std::string &m )
{
	std::cerr << m << std::endl;
}

