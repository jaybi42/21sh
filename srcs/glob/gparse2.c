#include "21sh.h"

int ret_init(t_arr *r, char *s)
{
	(r->i) = 0;
	(r->len) = ft_strlen(s);
	if (!((r->t_s) = xmalloc(sizeof(char *) * ((r->len) + 1))))
		return (FALSE);
	if (!((r->t_a) = xmalloc(sizeof(int) * ((r->len) + 1))))
		return (FALSE);
	(r->t_i) = 0;
	(r->t_a)[(r->t_i)] = 0;
	if (!((r->t_s)[(r->t_i)++] = xmalloc(sizeof(char) * ((r->len) + 1))))
		return (FALSE);
	(r->t_s)[(r->t_i)] = NULL;
	(r->t_s)[(r->t_i) - 1][0] = '\0';
	(r->tog) = 1;
	return (TRUE);
}

int	ret_o_b_place(t_arr *r, char **a_tmp, int i_tmp)
{
	char *old_tmp;

	if (!(old_tmp = x_strdup2((r->t_s)[(r->t_i) - 1], (r->t_a)[(r->t_i) - 1])))
			return (FALSE);
	i_tmp = 0;
	while (a_tmp[i_tmp] != NULL)
	{
		if ((r->t_s)[(r->t_i) - 1 + i_tmp] != NULL)
		{
			(r->t_s)[(r->t_i) - 1 + i_tmp] = x_strjoin2((r->t_s)[(r->t_i) - 1 + i_tmp]
			, (r->t_a)[(r->t_i) - 1 + i_tmp],
					a_tmp[i_tmp],ft_strlen(a_tmp[i_tmp]));
			(r->t_a)[(r->t_i) - 1 + i_tmp] += ft_strlen(a_tmp[i_tmp]);
		}
		else
		{
			(r->t_s)[(r->t_i) - 1 + i_tmp] = x_strjoin2(old_tmp, ft_strlen(old_tmp),
					a_tmp[i_tmp], ft_strlen(a_tmp[i_tmp]));
			(r->t_a)[(r->t_i) - 1 + i_tmp] = ft_strlen(old_tmp) +
			ft_strlen(a_tmp[i_tmp]);
		}
		i_tmp++;
	}
	return (TRUE);
}

int	 ret_open_bracket(t_arr *r, char *s)
{
	char **a_tmp;
	int i_tmp;

	a_tmp = ret_arr(s + (r->i) + 1);
	if (a_tmp == NULL)
		return (FALSE);
	i_tmp = 0;
	ret_o_b_place(r, a_tmp, i_tmp);
	(r->t_i) += i_tmp - 1;
	(r->tog) += i_tmp - 1;
	int xx;
	(r->i) += (xx = len_depth(s + (r->i) + 1));
	return (TRUE);
}

int	ret_virgule(t_arr *r)
{
	if (!((r->t_s)[(r->t_i)] = xmalloc(sizeof(char) * ((r->len) + 1))))
		return (FALSE);
	(r->t_s)[(r->t_i)][0] = '\0';
	(r->t_a)[(r->t_i)] = 0;
	(r->tog) = 1;
	(r->t_s)[++(r->t_i)] = NULL;
	return (TRUE);
}

int	ret_other(t_arr *r, char *s)
{
	int i_tmp;

	i_tmp = 0;
	while (i_tmp < (r->tog))
	{
		(r->t_s)[(r->t_i) - 1 - i_tmp][(r->t_a)[(r->t_i) - 1 - i_tmp]++] = s[(r->i)];
		(r->t_s)[(r->t_i) - 1 - i_tmp][(r->t_a)[(r->t_i) - 1 - i_tmp]] = '\0';
		i_tmp++;
	}
	return (TRUE);
}

char **ret_arr(char *s)
{
	t_arr r;

	if (!ret_init(&r, s))
		return (NULL);
	while (s[(r.i)])
	{
		if (s[(r.i)] == '}')
			break;
		else if (s[(r.i)] == ',')
		{
			if (!ret_virgule(&r))
				return (NULL);
		}
		else if (s[(r.i)] == '{')
		{
			if (!ret_open_bracket(&r, s))
				return (NULL);
		}
		else
			ret_other(&r, s);
		(r.i)++;
	}
	return (((r.i) == (r.len)) ? NULL : (r.t_s));
}
