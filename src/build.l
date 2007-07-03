%{
# include <string>
# include <string.h>
# include "buildparser.h"
# include "build.tab.h"

char *stringdup( const char *sin )
{
	char *n = new char[strlen(sin)+1];
	strcpy( n, sin );
	return n;
}

std::string strbuf;
%}

%x strsq
%x strdq
%x cmdstr
%x comment
%option noyywrap nounput batch debug

%{
# define YY_USER_ACTION  yylloc->last_column += yyleng;
%}
%%

[,:=[\]()]							return yytext[0];
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
"input"								return TOK_INPUT;
"filter"							return TOK_FILTER;
"prefix"							return TOK_PREFIX;
"aggregate"							return TOK_AGGREGATE;
"group"								return TOK_GROUP;

\n+									{
	yylloc->last_line += yyleng;
	yylloc->first_line = yylloc->last_line;
	yylloc->first_column = yylloc->last_column = 0;
}

[ \t\r]*							{
	yylloc->first_line = yylloc->last_line;
	yylloc->first_column = yylloc->last_column+1;
}

"#".*								/* single line comment */

[a-zA-Z][a-zA-Z0-9]*						{
	{
		if( bld.isTarget( yytext ) )
		{
			yylval->strval = stringdup( yytext );
			return TARGETTYPE;
		}
		else if( bld.isPerform( yytext ) )
		{
			yylval->strval = stringdup( yytext );
			return PERFORM;
		}
		else if( bld.isFunction( yytext ) )
		{
			yylval->strval = stringdup( yytext );
			return FUNCTION;
		}
		bld.error( yylloc, "Invalid token" );
	}
}

\"									{
	BEGIN( strdq );
	strbuf = "";
}
\'									{
	BEGIN( strsq );
	strbuf = "";
}
\`									{
	BEGIN( cmdstr );
	strbuf = "";
}

<strdq>[^\\\n\"]+					{
	strbuf += yytext;
}

<strsq>[^\\\n\']+					{
	strbuf += yytext;
}

<cmdstr>[^\\\n\`]+					{
	strbuf += yytext;
}

<strdq,strsq,cmdstr>\\n				strbuf += "\n";
<strdq,strsq,cmdstr>\\t				strbuf += "\t";
<strdq,strsq,cmdstr>\\r				strbuf += "\r";
<strdq,strsq,cmdstr>\\b				strbuf += "\b";
<strdq,strsq,cmdstr>\\f				strbuf += "\f";
<strdq,strsq,cmdstr>\\\\			strbuf += "\\";
<strdq,strsq,cmdstr>\\\"			strbuf += "\"";
<strdq,strsq,cmdstr>\\\'			strbuf += "\'";
<strdq,strsq,cmdstr>\\\`			strbuf += "`";
<strdq,strsq,cmdstr>\\.			bld.error( yylloc, "Invalid escape sequence.");

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

<cmdstr>\`							{
	BEGIN( INITIAL );
	FILE *fpg = popen( strbuf.c_str(), "r" );
	strbuf = "";
	for(;;)
	{
		char buf[1024];
		int nRead = fread( buf, 1, 1024, fpg );
		if( nRead == 0 ) break;
		for( int j = 0; j < nRead; j++ )
		{
			if( buf[j] == '\n' || buf[j] == '\r' )
				buf[j] = ' ';
		}
		strbuf.append( buf, nRead );
		if( nRead < 1024 ) break;
	}
	yylval->strval = stringdup( strbuf.c_str() );
	pclose( fpg );
	return STRING;
}

.									{
	char buf[] = {"Character x is out of place"};
	buf[10] = yytext[0];
	bld.error( yylloc, buf );
}

%%

void BuildParser::scanBegin()
{
	yy_flex_debug = false;
	if( !(yyin = fopen( file.c_str(), "r" )) )
	{
		error( std::string("cannot open file: ") + file );
	}
}

void BuildParser::scanEnd()
{
	fclose( yyin );
}

