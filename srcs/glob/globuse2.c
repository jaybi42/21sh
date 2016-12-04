#include "21sh.h"

char		*cpy_a_to_b2(char *str, int a, int b)
{
	char *new_str;
	int len;
	int i;

	len = (b - a < 0) ? 0: b - a;
	if (!(new_str = xmalloc(sizeof(char) * (len + 1))))
		return (NULL);
	i = 0;
	while (a + i < b)
	{
		new_str[i] = str[a + i];
		i++;
	}
	new_str[i] = '\0';
	return (new_str);
}

int tlen(char **t)
{
	int i;

	i = -1;
	while(t[++i]);
	return (i);
}


int accolade_isit(char c)
{
	if (c == ',')
		return (1);
	else
		return (0);
}
