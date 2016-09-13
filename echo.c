/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/13 13:23:03 by jguthert          #+#    #+#             */
/*   Updated: 2016/09/13 14:49:47 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

void	check_char(char c, int *option_n)
{
	if (c == 'a')
		ft_putchar('\a');
	else if (c == 'b')
		ft_putchar('\b');
	else if (c == 'c')
		*option_n = 1;
	else if (c == 'f')
		ft_putchar('\f');
	else if (c == 'n')
		ft_putchar('\n');
	else if (c == 'r')
		ft_putchar('\r');
	else if (c == 't')
		ft_putchar('\t');
	else if (c == 'v')
		ft_putchar('\v');
	else if (c == '\\')
		ft_putchar('\\');
	else
	{
		ft_putchar('\\');
		ft_putchar(c);
	}
}

int		check_options(char *s, int *option_n, int *option_control)
{
	int	i;

	i = 1;
	if (s[0] == '-')
	{
		while ((s[i] == 'e' || s[i] == 'E' || s[i] == 'n') && s[i] != '\0')
			++i;
		if (s[i] != '\0')
			return (1);
		i = 0;
		while (s[++i] != '\0')
		{
			if (s[i] == 'n')
				*option_n = 1;
			else if (s[i] == 'e')
				*option_control = 1;
			else if (s[i] == 'E')
				*option_control = 0;
		}
		return (0);
	}
	return (1);
}

int		bi_echo(t_av av, t_list **g_env, t_list **l_env)
{
	int	*options;
	int	end_option;
	int	i = -1;
	int	x = 1;

	end_option = 0;
	if ((options = (int *)ft_memalloc(sizeof(int) * 2)) == NULL)
		return (1);
	while(argv[x] != NULL)
	{
		if (end_option == 0)
			end_option = check_options(argv[x], &(options[0]), &(options[1]));
		while(argv[x][++i] != '\0' && end_option == 1)
		{
			if (argv[x][i] == '\\' && options[1] == 1)
			{
				++i;
				if (argv[x][i] != '\0')
					check_char(argv[x][i], &(options[0]));
			}
			else
				ft_putchar(argv[x][i]);
		}
		i = (-1);
		++x;
		if (argv[x] != NULL && end_option == 1)
			ft_putchar(' ');
	}
	if (options[0] == 0)
		ft_putchar('\n');
	return (0);
}
