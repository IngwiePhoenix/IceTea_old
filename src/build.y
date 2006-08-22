%defines
%{
# include <string>
# include "builder.h"
# include "build.tab.h"
void yyerror( YYLTYPE *locp, Builder &bld, char const *msg );
%}

%parse-param { Builder &bld }
%lex-param { Builder &bld }
%pure-parser

%locations

%debug
%error-verbose
%union {
	char *strval;
	int tval;
}

%token <strval>		STRING		"string literal"
%token <tval>		TARGETTYPE	"target type"
%token <strval>		FUNCTION	"function name"
%token <strval>		PERFORM		"perform name"

%token TOK_ADDSET				"+="
%token TOK_DEFAULT				"default"
%token TOK_ACTION				"action"
%token TOK_CHECK				"check"
%token TOK_CLEAN				"clean"
%token TOK_RULE					"rule"
%token TOK_TARGET				"target"
%token TOK_SET					"set"
%token TOK_INPUT				"input"
%token TOK_FILTER				"filter"
%token TOK_PREFIX				"prefix"
%token TOK_REQUIRES				"requires"
%token TOK_MATCHES				"matches"
%token TOK_PERFORM				"perform"
%token TOK_PRODUCES				"produces"

%token ',' ':' '=' '(' ')'

%destructor { delete[] $$; } STRING FUNCTION

%%

// General input format
input:
	 | input rule
	 | input action
	 | input target
	 ;

// Rule interpretation
rule: TOK_RULE STRING {printf("Rule %s:\n", $2 ); } ':' rulecmds
	;

rulecmds: rulecmd
		| rulecmds ',' rulecmd
		;

rulecmd: TOK_MATCHES { printf("    Matches: " ); } func
	   | TOK_PRODUCES STRING { printf("    Produces: %s\n", $2 ); }
	   | TOK_REQUIRES { printf("    Requires:\n"); } list {printf("\n");}
	   | TOK_INPUT TOK_FILTER { printf("    Input Filter: "); } func {printf("\n");}
	   | TOK_PERFORM { printf("    Perform: "); } perf {printf("\n");}
	   ;

// Action interpretation
action: TOK_DEFAULT TOK_ACTION ':' { printf("Default action:\n"); } actioncmds
	  | STRING TOK_ACTION ':' { printf("\"%s\" action:\n", $1 ); } actioncmds
	  ;

actioncmds: actioncmd
		  | actioncmds ',' actioncmd
		  ;

actioncmd: { printf("    "); } actioncmdtype list {printf("\n");}
		 ;

actioncmdtype: TOK_CHECK { printf("check "); }
			 | TOK_CLEAN { printf("clean "); }
			 ;

// Target interpretation
target: list ':' { printf(" are targets:\n"); } targetcmds
	  ;

targetcmds: targetcmd
		  | targetcmds ',' targetcmd
		  ;

targetcmd: TOK_RULE STRING { printf("    Rule %s\n", $2 ); }
		 | TOK_TARGET TOK_PREFIX STRING { printf("    Target prefix: %s\n", $3 ); }
		 | TOK_TARGET TARGETTYPE { printf("    Target Type: %d\n", $2 ); }
		 | TOK_INPUT { printf("    Input: "); } list { printf("\n"); }
		 | TOK_INPUT TOK_FILTER { printf("    Input filter: "); } func
		 | TOK_REQUIRES { printf("    Requires: "); } list { printf("\n"); }
		 | TOK_SET { printf("    Set: "); } targetset
		 ;

targetset: STRING '=' STRING { printf("%s = %s\n", $1, $3 ); }
		 | STRING TOK_ADDSET STRING { printf("%s += %s\n", $1, $3 ); }
		 ;

// list goo
list: listitem listfilter
	| '[' { printf("["); } listitems ']' { printf("]"); } listfilter
	;

listfilter:
		  | TOK_FILTER { printf(" filtered by "); } func
		  ;

listitems: listitem
		 | listitems ',' { printf(", "); } listitem
		 ;

listitem: STRING { printf("%s", $1 ); }
		| func
		;

// Function
func: FUNCTION { printf("%s(", $1 ); } '(' funcparams ')' { printf(")"); }
	;

funcparams:
		  | STRING { printf("%s", $1 ); }
		  | funcparams ',' STRING { printf(", %s", $3 ); }
		  ;

// Perform
perf: PERFORM { printf("%s(", $1 ); } '(' perfparams ')' { printf(")"); }
	;

perfparams:
		  | STRING { printf("%s", $1 ); }
		  | perfparams ',' STRING { printf(", %s", $3 ); }
		  ;
%%

void yyerror( YYLTYPE *locp, Builder &bld, char const *msg )
{
	bld.error( locp, msg );
}

