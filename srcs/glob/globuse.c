#include "21sh.h"

int ttlen(char ***t)
{
	int i;
	int a;
	int len;

	len = 0;
	a = 0;
	while (t[a])
	{
		i = 0;
		while(t[a][i])
		{
			len++;
			i++;
		}
		a++;
	}
	return (len);
}

char **fusion_tarray(char ***t)
{
	char **ret;
	int a;
	int i;
	int len;

	if (!(ret = xmalloc(sizeof(char **) * (ttlen(t) + 1))))
		return (NULL);
	a = 0;
	len = 0;
	while (t[a])
	{
		i = 0;
		while (t[a][i])
		{
			ret[len++] = x_strdup(t[a][i]);
			i++;
		}
		a++;
	}
	ret[len] = NULL;
	return (ret);
}

size_t len_depth(char *s)
{
	size_t i;
	int d;

	i = 0;
	d = 1;
	while (s[i])
	{
		if (s[i] == '{')
			d++;
		else if (s[i] == '}')
			d--;
		i++;
		if (d == 0)
			break;
	}
	return (i);
}

char	*x_strjoin2(char *s1, size_t len1, char *s2, size_t len2)
{
	int i;
	int a;
	char *s;

	a = 0;
	s = xmalloc(sizeof(char) * (len1 + len2 + 1));
	if (!s)
		return (NULL);
	i = -1;
	while (++i < (int)len1)
		s[a++] = s1[i];
	i = -1;
	while (++i < (int)len2)
		s[a++] = s2[i];
	s[a] = '\0';
	return (s);
}

char *x_strdup2(char *s, int len)
{
	int i;
	char *ns;

	i = 0;
	ns = xmalloc(sizeof(char) * (len + 1));
	if (ns == NULL)
		return (NULL);
	while (i < len)
	{
		ns[i] = s[i];
		i++;
	}
	ns[i] = '\0';
	return (ns);
}
