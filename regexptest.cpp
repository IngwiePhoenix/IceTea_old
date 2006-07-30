#include <stdio.h>
#include <regex.h>
#include <string>

int test( const char *str, const char *sRegExp )
{
	printf("Compiling: %s\n", sRegExp );
	regex_t r;
	if( regcomp( &r, sRegExp, REG_EXTENDED|REG_NEWLINE ) )
	{
		printf("Error compiling regular expression.\n");
		return 0;
	}

	printf("Compiled, %d sub expressions.\n", r.re_nsub );

	int nMatch = r.re_nsub+1;
	regmatch_t *match = new regmatch_t[nMatch];
	if( regexec( &r, str, nMatch, match, 0 ) )
	{
		printf("Regular expression did not match.\n");
		return 0;
	}

	printf("Match!\nSubstrings:\n");
	for( int j = 0; j < nMatch; j++ )
	{
		printf("   %d: (%d-%d) %s\n",
			j,
			match[j].rm_so, match[j].rm_eo,
			std::string(str+match[j].rm_so, match[j].rm_eo-match[j].rm_so ).c_str()
			);
	}

	delete[] match;
	regfree( &r );
}

int main( int argc, char *argv[] )
{
	printf("Regular expression test:\n\n");

	test( argv[1], argv[2] );

	return 0;
}

