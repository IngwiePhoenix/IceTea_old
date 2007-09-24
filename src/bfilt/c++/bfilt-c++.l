%{
# include <string>

int nBC = 0;
%}

%s hasT
%x inWith
%x inT
%option noyywrap nounput batch

%%

"operator<<"						{ ECHO; }
"operator>>"						{ ECHO; }
"operator".							{ ECHO; }

"<<"								{ ECHO; }
">>"								{ ECHO; }
" <<"								{ ECHO; }
" >>"								{ ECHO; }
"'<'"								{ ECHO; }
"'>'"								{ ECHO; }

"<anonymous>"						{ ECHO; }

\n+									{
	BEGIN( INITIAL );
	nBC = false;
	ECHO;
}

" <"								{ ECHO; }

"<"									{
	BEGIN( inT );
	printf("<...>");
	nBC++;
}

<inT>"<"							{
	nBC++;

}
<inT>[^<>]*							{ }
<inT>">"							{
	nBC--;
	if( nBC == 0 )
		BEGIN( hasT );
}

<hasT>" [with"[^\]]*"]"					{ }

%%

int main()
{
	yyin = stdin;

	yylex();

	return 0;
}

