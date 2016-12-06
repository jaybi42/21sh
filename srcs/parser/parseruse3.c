#include "21sh.h"

void reset_current(t_parse *p, int index)
{
	p->nb++;
	p->current = EMPTY;
	p->begin[p->nb] = index + 1;
	p->end[p->nb] = 0;
	p->type[p->nb] = EMPTY;
	p->one_arg = FALSE;
	p->quote_activate = FALSE;
}

void update_new(t_parse *p, int index, int delimiter_index)
{
	p->end[p->nb++] = index;
	p->current = delimiter_index;
	p->type[p->nb] = delimiter_index;
	p->begin[p->nb] = index + ft_strlen(g_delimiter[delimiter_index].name);
	if (g_delimiter[delimiter_index].is_redirection == TRUE)
		p->begin[p->nb] = index;
	p->one_arg = FALSE;
	p->quote_activate = FALSE;
}