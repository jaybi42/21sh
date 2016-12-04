#include "21sh.h"

void		handle_delimiter2(char *expr, int *i, t_parse *p, t_delimiter *d, int l)
{
	int a;

	a = 0;
	while (a < l)
	{
		if (ft_strncmp(expr + (*i), d[a].name,
					ft_strlen(d[a].name)) == 0)
		{
			if (p->current != EMPTY && d[p->current].wait_another == TRUE
					&& p->current == a)
			{
				//that's mean we find for example the second " of ""
				p->end[p->nb] = (*i);
				reset_current(p, (*i));
			}
			else if (p->current != EMPTY && d[p->current].do_recursivity == TRUE
					&& p->current == a)
			{
				//that's mean we find for example another
				update_new(p, (*i), a);
			}
			else if (p->current == EMPTY)
			{
				update_new(p, (*i), a);
			}
			(*i) += 1;
			return ;
		}
		a++;
	}
	if (p->current != EMPTY && d[p->current].is_redirection == TRUE
			&& p->one_arg == FALSE
			&& char_is_whitespace(expr[(*i)]) == FALSE)
		p->one_arg = TRUE;
	else if (p->current != EMPTY && d[p->current].is_redirection == TRUE
			&& p->one_arg == TRUE 		&& char_is_whitespace(expr[(*i)]) == TRUE)
	{
		p->end[p->nb] = (*i);
		reset_current(p, (*i));
	}
	(*i) += 1;
}

t_parse		*parse_it2(char *expr, int len, t_delimiter *d, int l)
{
	int i;
	t_parse *p;

	if (!(p = xmalloc(sizeof(t_parse))) ||
			!(p->begin = xmalloc(sizeof(int) * ft_strlen(expr))) ||
			!(p->end = xmalloc(sizeof(int) * ft_strlen(expr))) ||
			!(p->type = xmalloc(sizeof(int) * ft_strlen(expr))))
		exit(0);
	i = 0;
	p->nb = 0;
	p->current = -1;
	p->begin[0] = 0;
	p->type[0] = -1;
	while (i < len && expr[i] != '\0')
		handle_delimiter2(expr, &i, p, d, l);
	if (p->current != EMPTY && d[p->current].wait_another == TRUE)
		ft_printf("[!] we were waiting another |%s| adding one for u\n", d[p->current].name);
	p->end[p->nb] = i;
	p->nb++;
	return (p);
}
