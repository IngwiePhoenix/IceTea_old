#ifndef PARSER_H
#define PARSER_H

#include <stdint.h>
#include <string>

class Build;

#if ! defined (YYLTYPE) && ! defined (YYLTYPE_IS_DECLARED)
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif

class Parser
{
public:
	Parser();
	virtual ~Parser();

	virtual void error( YYLTYPE *locp, const char *msg );
	virtual void error( const std::string &msg );

	virtual Build *load( const std::string &sFile ) = 0;

protected:
	std::string file;

private:

};

#endif
