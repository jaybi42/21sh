/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_event.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/25 19:08:36 by jguthert          #+#    #+#             */
/*   Updated: 2016/09/25 19:13:51 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

/*
** is_match return true if the first word of str == tofind
*/

static int		is_match(char *str, char *tofind)
{
	if (str == NULL || tofind == NULL)
		return (0);
	while (*str != '\0' && ft_isblank(*str) == 1)
		str++;
	while (*str == *tofind)
	{
		if (*tofind == '\0')
			return (1);
		else if (*str == '\0')
			break ;
		str++;
		tofind++;
	}
	return (0);
}

/*
** get_line return the event corresponding to the number of the event
** if it exists
*/

static char		*get_event_byid(int pos, t_ftl_root *hist)
{
	t_ftl_node	*node;

	if (ABS(pos) > (int)hist->size || pos == 0 ||
		(t_ftl_root *)hist->next == hist)
		return (NULL);
	if (pos < 0)
		pos = hist->size + pos + 1;
	node = (t_ftl_node *)hist->next;
	while (--pos > 0)
		node = node->next;
	ft_putendl_fd(((t_hist *)node)->str, 2);
	return (((t_hist *)node)->str);
}

static char		*get_event_bystr(char *str, t_ftl_root *hist)
{
	t_ftl_node	*node;

	node = (t_ftl_node *)hist->next;
	while (node != (t_ftl_node *)hist)
	{
		if (is_match(((t_hist *)node)->str, str) == 1)
			return (((t_hist *)node)->str);
		node = node->next;
	}
	ft_putstr(str);
	ft_putendl(" : Event not found");
	return (NULL);
}

/*
** when a "!foo" is found
** foo is a number == the number of the event
** foo is '!' == last event
** foo is a string == the event starting with the same word
*/

char			*get_event(char *str, t_ftl_root *hist)
{
	char		*new_entry;
	char		*tofind;

	tofind = ft_first_word(str);
	if (tofind == NULL)
		return (NULL);
	else if (ft_isdigit(*(tofind + 1)) == 1)
		new_entry = get_event_byid(ft_atoi(tofind + 1), hist);
	else if (*(tofind + 1) == '!')
		new_entry = get_event_byid(-1, hist);
	else
		new_entry = get_event_bystr(tofind + 1, hist);
	if (new_entry == NULL)
	{
		ft_putstr(tofind);
		ft_putendl(" : Event not found");
	}
	ft_strdel(&tofind);
	return (new_entry);
}