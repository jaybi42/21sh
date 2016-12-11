/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shelluse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouchla <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/06 19:59:23 by ibouchla          #+#    #+#             */
/*   Updated: 2016/12/06 19:59:27 by ibouchla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int		x_strjoins(char **s1, size_t *len1, char *s2, size_t len2)
{
	int		i;
	int		a;
	char	*s;
	int		len;

	a = 0;
	if (s1 == NULL || s2 == NULL)
		return (0);
	len = (int)((*len1) + len2 + 1);
	s = xmalloc(sizeof(char) * len);
	if (!s)
		return (0);
	i = -1;
	while (++i < (int)(*len1))
		s[a++] = (*s1)[i];
	i = -1;
	while (++i < (int)len2)
		s[a++] = s2[i];
	s[a] = '\0';
	(*s1) = s;
	(*len1) = a;
	return (1);
}

void	set_retcode(unsigned char ret_code)
{
	char	*s;

	handle_var(KV_SET, "?", (s = ft_litoa(ret_code)));
	ft_strdel(&s);
}

void	clear_stack(t_executor ***stack, int *stack_index)
{
	(*stack) = xmalloc(sizeof(t_exec*) * 2);
	(*stack_index) = 0;
	(*stack[(*stack_index)]) = NULL;
}

void	update_stack(t_executor ***p_stack, int *p_stack_index,
		t_exec *current_ex, t_av *current_av)
{
	(*p_stack)[(*p_stack_index)] = xmalloc(sizeof(t_executor));
	ft_memcpy(&(*p_stack)[(*p_stack_index)]->ex, (t_exec*)current_ex,
			sizeof(t_exec));
	ft_memcpy(&(*p_stack)[(*p_stack_index)]->av,
			(t_av*)current_av, sizeof(t_av));
	(*p_stack_index) += 1;
	(*p_stack)[(*p_stack_index)] = NULL;
}

void	extend_stack(t_executor ***stack, int *stack_index, t_exec *current_ex,
		t_av *current_av)
{
	int			i;
	t_executor	**tmp;

	tmp = xmalloc(sizeof(t_executor*) * ((*stack_index) + 2));
	i = 0;
	while ((*stack)[i])
	{
		tmp[i] = (*stack)[i];
		i++;
	}
	tmp[i] = xmalloc(sizeof(t_executor));
	ft_memcpy(&tmp[i]->ex, (t_exec*)current_ex, sizeof(t_exec));
	ft_memcpy(&tmp[i]->av, (t_av*)current_av, sizeof(t_av));
	tmp[i + 1] = NULL;
	(*stack) = tmp;
	(*stack_index) = i + 1;
}
