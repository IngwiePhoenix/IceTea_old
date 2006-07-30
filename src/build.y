%defines
%{
# include <string>
# include "builder.h"
# include "action.h"
# include "command.h"
# include "build.tab.h"
void yyerror( YYLTYPE *locp, Builder &bld, char const *msg );
%}

%parse-param { Builder &bld }
%lex-param { Builder &bld }
%pure-parser

%locations
%initial-action
{
	//@$.begin.filename = @$.end.filename = &bld.file;
}

%debug
%error-verbose
%union {
	char *strval;
}

%token <strval>		STRING		"string literal"
%token <strval>		REGEXP		"regular expression"

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
%token TOK_CHECK				"keyword 'check'"
%token TOK_EOL					"end of line"
%token ',' ':' '='

%destructor { delete[] $$; } STRING

%%

input:
	 | input fullline
	 ;

fullline: TOK_EOL
		| line TOK_EOL
		;

line: TOK_DEFAULT TOK_ACTION ':'
	  {
		  bld.add( new Action() );
	  }
	  actionlst
	| STRING TOK_ACTION ':'
	  {
		  bld.add( new Action( $1 ) );
	  }
	  actionlst
	| TOK_CREATE createwhat TOK_FROM createfrom TOK_USING createusing
	;

createwhat: TOK_FILE STRING { printf("target: %s\n", $2 ); }
		  ;

createfrom: TOK_FILES TOK_IN createfromdirlst
		  ;

createfromdirlst: createfromdir
				| createfromdirlst ',' createfromdir
				;

createfromdir: STRING { printf("  srcdir: %s\n", $1 ); }
			 ;

createusing: TOK_RULE STRING { printf("  rule: %s\n", $2 ); }
		   ;

actionlst: action
		 | actionlst ',' action
		 ;

action: TOK_CHECK STRING
	    {
			bld.add( new Command( Command::cmdCheck, $2 ) );
		}
	  ;

%%

void yyerror( YYLTYPE *locp, Builder &bld, char const *msg )
{
	fprintf( stderr, "%s:%d-%d:%d-%d: %s\n",
		bld.file.c_str(),
		locp->first_line, locp->last_line,
		locp->first_column, locp->last_column,
		msg
		);
}
