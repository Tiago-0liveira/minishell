#include "minishell.h"


#include <stdio.h>

int main()
{
	char *s;

	s = readline(PROMPT);
	printf("|%s|",s);

	return 0;
}