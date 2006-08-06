%defines
%{
# include <string>
# include "builder.h"
# include "action.h"
# include "command.h"
# include "rule.h"
# include "filetarget.h"
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
}

%token <strval>		STRING		"string literal"
%token <strval>		REGEXP		"regular expression"

%token TOK_ADDSET				"+="
%token TOK_DEFAULT				"default"
%token TOK_ACTION				"action"
%token TOK_CREATE				"create"
%token TOK_FILE					"file"
%token TOK_FROM					"from"
%token TOK_FILES				"files"
%token TOK_IN					"in"
%token TOK_USING				"using"
%token TOK_RULE					"rule"
%token TOK_REQUIRES				"requires"
%token TOK_FOR					"for"
%token TOK_SET					"set"
%token TOK_MATCHES				"matches"
%token TOK_MATCHING				"matching"
%token TOK_ALL					"all"
%token TOK_ONE					"one"
%token TOK_PERFORM				"perform"
%token TOK_PRODUCES				"produces"
%token TOK_COMMAND				"command"
%token TOK_CHECK				"check"
%token TOK_CLEAN				"clean"
%token TOK_DIRECTORIES			"directories"
%token TOK_TARGETS				"targets"
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
	| TOK_FOR fortypes TOK_MATCHING REGEXP
	  {
		  bld.setFilter( $4 );
	  }
	  augments
	  {
		  bld.endList();
	  }
	  listcmds
	| STRING TOK_REQUIRES 
	  {
		  bld.setTmp( $1 );
		  bld.requiresRegexp( false );
	  }
	  reqcompletion
	| REGEXP TOK_REQUIRES
	  {
		  bld.setTmp( $1 );
		  bld.requiresRegexp( true );
	  }
	  reqcompletion
	| listcmds
	| TOK_FOR STRING
	  {
		  bld.startList( STRING );
		  bld.addListItem( $2 );
	  }
	  listcmds
	  {
		  
	  }
	| rule
	;

fortypes: TOK_DIRECTORIES
		{
			bld.startList( TOK_DIRECTORIES );
		}
		| TOK_TARGETS
		{
			bld.startList( TOK_TARGETS );
		}
		| TOK_FILES
		{
			bld.startList( TOK_FILES );
		}
		;

augments:
		| TOK_IN STRING
		{
			bld.augmentList( $2 );
		}
		;

reqcompletion: reqlst
			 | TOK_FROM TOK_COMMAND STRING
			   {
				   bld.requiresFromCommand( bld.getTmp(), $3 );
			   }
			 ;

reqlst: STRING
	    {
			bld.requires( bld.getTmp(), $1 );
		}
	  | reqlst ',' STRING
	    {
			bld.requires( bld.getTmp(), $3 );
		}
	  ;

listcmds: TOK_SET setexpr
		| TOK_CREATE createwhat TOK_FROM createfrom TOK_USING createusing
		{
			bld.endTarget();
		}
		;

createwhat: TOK_FILE STRING
		    {
				bld.addTarget( TOK_FILE, $2 );
			}
		  ;

createfrom: TOK_FILES TOK_IN
		    {
				bld.setTargetInputType( TOK_FILES );
			}
			createfromdirlst
		  ;

createfromdirlst: createfromdir
				| createfromdirlst ',' createfromdir
				;

createfromdir: STRING
			   {
				   bld.addTargetInput( $1 );
			   }
			 ;

createusing: TOK_RULE STRING
		     {
				 bld.setTargetRule( $2 );
			 }
		   ;

actionlst: action
		 | actionlst ',' action
		 ;

action: TOK_CHECK STRING
	    {
			bld.add( new Command( Command::cmdCheck, $2 ) );
		}
	  | TOK_CHECK REGEXP
	    {
			bld.addRegexCommand( Command::cmdCheck, $2 );
		}
	  | TOK_CLEAN STRING
	    {
			bld.add( new Command( Command::cmdClean, $2 ) );
		}
	  | TOK_CLEAN REGEXP
	    {
			bld.addRegexCommand( Command::cmdClean, $2 );
		}
	  ;

setexpr: STRING '=' STRING
	     {
			 bld.varSet( $1, $3 );
		 }
	   | STRING TOK_ADDSET STRING
	     {
			 bld.varAddSet( $1, $3 );
		 }
	   ;

rule: TOK_RULE STRING
	  {
		  bld.add( new Rule( $2 ) );
	  }
	  rulesublst TOK_PERFORM rulecompletion
	;

rulesublst: rulesub
		  | rulesublst rulesub
		  ;

rulesub: TOK_MATCHES rulematches
	   | TOK_PRODUCES produceslst
	   ;

produceslst: STRING
		   {
			   bld.lastRule()->addProduces( $1 );
		   }
		   | produceslst ',' STRING
		   {
			   bld.lastRule()->addProduces( $3 );
		   }
		   ;

rulematches: TOK_ALL REGEXP
		     {
				 try
				 {
					 bld.lastRule()->setMatches( Rule::matchAll, $2 );
				 }
				 catch( BuildException &e )
				 {
					 std::string s("RegExp compile error: ");
					 s += e.what();
					 yyerror( &yyloc, bld, s.c_str() );
					 return 1;
				 }
			 }
		   | TOK_ONE REGEXP
		     {
				 try
				 {
					 bld.lastRule()->setMatches( Rule::matchOne, $2 );
				 }
				 catch( BuildException &e )
				 {
					 std::string s("RegExp compile error: ");
					 s += e.what();
					 yyerror( &yyloc, bld, s.c_str() );
					 return 1;
				 }
			 }
		   ;

rulecompletion: TOK_COMMAND STRING
			    {
					bld.lastRule()->setPerforms( Rule::perfCommand, $2 );
				}
			  ;

%%

void yyerror( YYLTYPE *locp, Builder &bld, char const *msg )
{
	bld.error( locp, msg );
}
