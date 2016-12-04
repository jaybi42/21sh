#include "21sh.h"

char **check_var(char *s, char **env)
{
	int i;
	char **tmp;
	char *tmpe;

	tmpe = handle_var(KV_GET, s, NULL);
	if (tmpe != NULL)
	{
		char **ret;

		ret = xmalloc(sizeof(char *) * 3);
		if (!ret)
			return (NULL);
		ret[0] = s;
		ret[1] = tmpe;
		ret[2] = NULL;
		return (ret);
	}

	i = 0;
	while (env[i])
	{
		tmp = x_strsplit(env[i], '=');
		if (tmp != NULL && tmp[0] != NULL && tmp[1] != NULL)
			if (ft_strncmp(s, tmp[0], ft_strlen(tmp[0])) == 0)
			{
				char **ret;
				ret = xmalloc(sizeof(char *) * 3);
				if (!ret)
					return (NULL);
				ret[0] = tmp[0];
				ret[1] = tmp[1];
				ret[2] = NULL;
				return (ret);
			}
		i++;
	}
	return (NULL);
}

char *apply_var(char *s, int do_extra)
{
	int i;
	char *ns;
	char **tmp;
	size_t len;
	char **env;

	env = convert_env(g_env, l_env);
	if (do_extra == TRUE)
		s = handle_tilde(s, env);
	i = -1;
	len = 0;
	while (s[++i])
	{
		if (s[i] == '$' && (tmp = check_var(s + i + 1, env)) != NULL)
			len += ft_strlen(tmp[1]);
		else
			len += 1;
	}
	if (!(ns = xmalloc(sizeof(char) * (len + 1))))
		return (NULL);
	i = -1;
	len = 0;
	while (s[++i])
	{
		if (s[i] == '$' && (tmp = check_var(s + i + 1, env)) != NULL)
		{
				x_strjoins(&ns, &len, tmp[1], ft_strlen(tmp[1]));
				ns[len] = '\0';
				i += (ft_strlen(tmp[1]) > 0) ? ft_strlen(tmp[1]) - 1 : 1;
		}
		else
			ns[len++] = s[i];
	}
	ns[len] = '\0';
	return (ns);
}
