#include "shell.h"

void decalage_intouchable(int **t_ind, int fpoint, int dec)
{
	int i;

	i = -1;
	while ((*t_ind)[++i] != -1)
		if ((*t_ind)[i] >= fpoint)
			(*t_ind)[i] += dec;
}

void replace_alias(char **expr, int **t_ind, int i, int len)
{
	char *s;

	(void)t_ind;
	s = get_alias(cpy_a_to_b((*expr), i, i + len));
	if (s != NULL)
	{
		inject_in_string(expr, s, i, len);
		decalage_intouchable(t_ind, i, ft_strlen(s) - len);
	}
}

void replace_glob(char **expr, int **t_ind, int i, int len)
{
	char *s;

	s = ret_glob(cpy_a_to_b((*expr), i, i + len));
	if (s != NULL)
	{
		inject_in_string(expr, s, i, len);
		decalage_intouchable(t_ind, i, ft_strlen(s) - len);
	}
}

void handle_alias_glob2(char **expr, int **t_ind, int **l_ind,
	t_norm_hag *t)
{
	(t->a) = 0;
	while ((*expr)[(t->i) + (t->a)] && !(
		is_whitespace((*expr)[(t->i) + (t->a)]) &&
		!is_intouchable((t->i) + (t->a), (*t_ind), (*l_ind))))
	{
		if (is_intouchable((t->i) + (t->a), (*t_ind), (*l_ind)))
		{
			(t->continu) = TRUE;
			(t->i) += (t->a) + 1;
			(t->first_time) = FALSE;
			break;
		}
		(t->a)++;
	}
	if ((t->continu) == TRUE)
			return ;
	if ((t->first_time))
		replace_alias(expr, t_ind, (t->i), (t->a));
	replace_glob(expr, t_ind, (t->i), (t->a));
	(t->i) += (t->a) + 1;
	(t->first_time) = FALSE;
}

void handle_alias_glob(char **expr, int **t_ind, int **l_ind)
{
	t_norm_hag t;

	(t.i) = 0;
	(t.first_time) = TRUE;
	(t.len) = ft_strlen((*expr));
	while ((t.i) < (t.len))
	{
		(t.continu) = FALSE;
		if (is_whitespace((*expr)[(t.i)]) &&
		!is_intouchable((t.i), (*t_ind), (*l_ind)))
				(t.i)++;
		else
			handle_alias_glob2(expr, t_ind, l_ind, &t);
	}
}
