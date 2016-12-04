#include "21sh.h"

int			char_is_whitespace(char c)
{
	if (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r')
		return (TRUE);
	else
		return (FALSE);
}

char		*cpy_a_to_b(char *str, int a, int b)
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

char *find_home(char **env)
{
	int i;
	char **t;

	i = 0;
	while (env[i])
	{
			if (ft_strncmp(env[i], "HOME=", 5) == 0)
			{
					if (!(t = ft_strsplit(env[i], '=')))
						return (NULL);
					if (t[0] != NULL && t[1] != NULL)
						return (t[1]);
					else
						return (NULL);
			}
			i++;
	}
	return (NULL);
}

char *join_string_array(char **a, int *marked_ind, int **t_ind, int **l_ind)
{
	int i;
	size_t len;
	char *ns;
	int len_ind;

	len = 0;
	len_ind = 0;
	i = -1;
	while (a[++i])
	{
		len += ft_strlen(a[i]) + 1;
		if (marked_ind[i] == 1)
			len_ind++;
	}
	ns = xmalloc(sizeof(char) * (len + 1));
	(*t_ind) = xmalloc(sizeof(int) * (len_ind + 1));
	(*l_ind) = xmalloc(sizeof(int) * (len_ind + 1));
	i = 0;
	len = 0;
	len_ind = 0;
	while (a[i])
	{
		if (marked_ind[i] == 1)
		{
			(*t_ind)[len_ind] = len ;
			(*l_ind)[len_ind++] = ft_strlen(a[i])-1;
		}
		x_strjoins(&ns, &len, a[i], ft_strlen(a[i]));
		/*if (a[i + 1])
			x_strjoins(&ns, &len, " ", 1);*/
		i++;
	}
	ns[len] = '\0';
	(*t_ind)[len_ind] = -1;
	(*l_ind)[len_ind] = -1;
	return (ns);
}
