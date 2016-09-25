/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_history.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/06 16:58:08 by jguthert          #+#    #+#             */
/*   Updated: 2016/09/25 18:52:05 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

/*
** This file parse the char *str, in struct l
** if a "!foo" is found we replace it
*/


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

	if (ABS(pos) > (int)hist->size || pos == 0 || (t_ftl_root *)hist->next == hist)
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

static char		*get_event(char *str, t_ftl_root *hist)
{
	char		*new_entry;
	char		*tofind;

	tofind = ft_first_word(str);
	if (tofind == NULL)
		return (NULL);
	if (ft_isdigit(*(tofind + 1)) == 1)
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

static int		transform_str(t_line *l, char *new_entry, int i)
{
	char	*temp;
	int		len;

	if (i != 0)
	{
		temp = ft_strndup(l->str, i - 1);
		if (temp == NULL)
			return (1);
		new_entry = ft_strjoin(temp, new_entry);
		ft_strdel(&temp);
		if (new_entry == NULL)
			return (1);
	}
	len = 0;
	while (l->str[len] != '\0' && ft_isblank(l->str[len]) == 0)
		len++;
	temp = ft_strjoin(new_entry, l->str + len);
	if (temp == NULL)
		return (1);
	ft_strdel(&(l->str));
	l->str = temp;
	return (0);
}

void			parse_history(t_line *l, t_ftl_root *hist)
{
	char	*new_entry;
	int		i;

	i = 0;
	while (l->str[i] != '\0')
	{
		if (l->str[i] == '!' && (i == 0 || l->str[i - 1] != '\\'))
		{
			new_entry = get_event(l->str, hist);
			if (new_entry != NULL)
			{
				transform_str(l, new_entry, i);
				ft_strdel(&new_entry);
			}
		}
		i++;
	}
}
