/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search_history.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/16 16:17:59 by jguthert          #+#    #+#             */
/*   Updated: 2016/09/19 18:53:45 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"
#include <unistd.h>

/*
static void		print_search(t_line *l, char *search)
{
	int		pos_cur;

	pos_cur = l->count;
	do_term("sc");
	ft_end(l);
	ft_putstr("\nbck-i-search: ");
	if (search != NULL)
		ft_putstr(search);
	do_term("rc");
	l->count = pos_cur;
}
*/

static int		ft_search_str(char *str, char *to_find)
{
    int i;
    int j;
    int size;

    i = 0;
    j = 0;
    size = ft_strlen(to_find);
    if (to_find == NULL)
        return (-1);
    while (str[i])
    {
        while (to_find[j] == str[i + j])
        {
            if (j == size - 1)
                return (i);
            j++;
        }
        j = 0;
        i++;
    }
    return (-1);
}

static void		find_history(t_line *l, t_ftl_root *root, char *to_search)
{
	t_ftl_node	*node;
	int			ret;

	node = (t_ftl_node *)root->prev;
	while (node != (t_ftl_node *)root)
	{
		ret = ft_search_str(((t_hist *)node)->str, to_search);
		if (ret >= 0)
		{
			l->count = ret;
			l->str = ((t_hist *)node)->str;
			l->size = ft_strlen(l->str);
			break ;
		}
		node = node->next;
	}
}

void			get_line_history(t_line *l, t_ftl_root *root)
{
	char		*oldstr;
	char		*search;

	oldstr = l->str;
	search = NULL;
	while (1)
	{
		find_history(l, root, search);
		ft_print_line(l);
		ft_bzero(l->buffer, 6);
		if (read(0, l->buffer, 6) == -1)
			return ;
		if (l->buffer[0] == 10)
			break ;
		else if (ft_strncmp(CTRL_R, l->buffer, 6) == 0)
			break ;
		else if (ft_strncmp(BACKSPACE, l->buffer, 6) == 0)
			break ;
	}
}
