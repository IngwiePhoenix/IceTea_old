%skeleton "lalr1.cc"
%define "parser_class_name" "BuildParser"
%defines
%{
# include <string>
# include "builder.h"
# include "build.tab.h"
%}

%parse-param { Builder &bld }
%lex-param { Builder &bld }

%locations
%initial-action
{
	@$.begin.filename = @$.end.filename = &bld.file;
}

%debug
%error-verbose
%union {
	char *strval;
}

%token <strval>		STRING		"string literal"

%token TOK_ADDSET				"+="
%token TOK_DEFAULT				"keyword 'default'"
%token TOK_ACTION				"keyword 'action'"
%token TOK_CREATE				"keyword 'create'"
%token TOK_FILE					"keyword 'file'"
%token TOK_FROM					"keyword 'from'"
%token TOK_FILES				"keyword 'files'"
%token TOK_IN					"keyword 'in'"
%token TOK_USING				"keyword 'using'"
%token TOK_RULE					"keyword 'rule'"
%token TOK_REQUIRES				"keyword 'requires'"
%token TOK_FOR					"keyword 'for'"
%token TOK_SET					"keyword 'set'"
%token TOK_MATCHES				"keyword 'matches'"
%token TOK_ALL					"keyword 'all'"
%token TOK_ONE					"keyword 'one'"
%token TOK_PERFORM				"keyword 'perform'"
%token TOK_PRODUCES				"keyword 'produces'"
%token TOK_COMMAND				"keyword 'command'"
%token TOK_EOL					"end of line"

%destructor { delete[] $$; } STRING

%%

input:
	 | input line
	 ;

line: stuff TOK_EOL				{ printf("\n"); }
	;

stuff:
	 | stuff token
	 ;

token: TOK_ADDSET				{ printf("+= "); }
	 | TOK_DEFAULT				{ printf("default "); }
	 | TOK_ACTION				{ printf("action "); }
	 | TOK_CREATE				{ printf("create "); }
	 | TOK_FILE					{ printf("file "); }
	 | TOK_FROM					{ printf("from "); }
	 | TOK_FILES				{ printf("files "); }
	 | TOK_IN					{ printf("in "); }
	 | TOK_USING				{ printf("using "); }
	 | TOK_RULE					{ printf("rule "); }
	 | TOK_REQUIRES				{ printf("requires "); }
	 | TOK_FOR					{ printf("for "); }
	 | TOK_SET					{ printf("set "); }
	 | TOK_MATCHES				{ printf("matches "); }
	 | TOK_ALL					{ printf("all "); }
	 | TOK_ONE					{ printf("one "); }
	 | TOK_PERFORM				{ printf("perform "); }
	 | TOK_PRODUCES				{ printf("produces "); }
	 | TOK_COMMAND				{ printf("command "); }
	 ;

%%

void yy::BuildParser::error( const yy::BuildParser::location_type &l,
						   const std::string &m )
{
	bld.error( l, m );
}

