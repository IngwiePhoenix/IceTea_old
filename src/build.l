%{
# include <string>
# include "builder.h"
# include "build.tab.h"
# include "stringrep.h"

std::string strbuf;
%}

%x regexp
%x strsq
%x strdq
%x comment
%option noyywrap nounput batch debug

%{
# define YY_USER_ACTION  yylloc->last_column += yyleng;
%}
%%

[,:=[\]]							return yytext[0];
"+="								return TOK_ADDSET;

"default"							return TOK_DEFAULT;
"action"							return TOK_ACTION;
"rule"								return TOK_RULE;
"requires"							return TOK_REQUIRES;
"set"								return TOK_SET;
"matches"							return TOK_MATCHES;
"perform"							return TOK_PERFORM;
"produces"							return TOK_PRODUCES;
"check"								return TOK_CHECK;
"clean"								return TOK_CLEAN;
"target"							return TOK_TARGET;

\n+									{
	yylloc->last_line += yyleng;
	yylloc->first_line = yylloc->last_line;
	yylloc->first_column = yylloc->last_column = 0;
}

[ \t\r]*							{
	yylloc->first_line = yylloc->last_line;
	yylloc->first_column = yylloc->last_column+1;
}

"/"									{
	BEGIN( regexp );
	strbuf = "";
}
<regexp>[^\n/\\]*					strbuf += yytext;
<regexp>"\\/"						strbuf += "/";
<regexp>"\\"						strbuf += "\\";
<regexp>"/"							{
	BEGIN( INITIAL );
	yylval->strval = stringdup( strbuf.c_str() );
	return REGEXP;
}

"#".*								/* single line comment */

\"									{
	BEGIN( strdq );
	strbuf = "";
}
\'									{
	BEGIN( strsq );
	strbuf = "";
}

<strdq>[^\\\n\"]+					{
	strbuf += yytext;
}

<strsq>[^\\\n\']+					{
	strbuf += yytext;
}

<strdq,strsq>\\n					strbuf += "\n";
<strdq,strsq>\\t					strbuf += "\t";
<strdq,strsq>\\r					strbuf += "\r";
<strdq,strsq>\\b					strbuf += "\b";
<strdq,strsq>\\f					strbuf += "\f";

<strdq>\"							{
	BEGIN( INITIAL );
	yylval->strval = stringdup( strbuf.c_str() );
	return STRING;
}

<strsq>\'							{
	BEGIN( INITIAL );
	yylval->strval = stringdup( strbuf.c_str() );
	return STRING;
}

%%

void Builder::scanBegin()
{
	yy_flex_debug = false;
	if( !(yyin = fopen( file.c_str(), "r" )) )
	{
		error( std::string("cannot open file: ") + file );
	}
}

void Builder::scanEnd()
{
	fclose( yyin );
}

