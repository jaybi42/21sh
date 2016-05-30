/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logic_red.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/26 13:17:55 by jguthert          #+#    #+#             */
/*   Updated: 2016/05/29 17:30:41 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

static t_red const	g_red[8] =
{
	{red_stofile, ">"};
	{red_dtofile, ">>"};
	{red_stoin, "<"};
	{red_dtoin, "<<"};
	{red_pipe, "|"};
	{red_and, "&&"};
	{red_or, "||"};
	{NULL, NULL};
};

int			logic_red(char *in, char *out, char *value)
{
	int		i;

	i = 0;
	while (g_red[i].value != NULL)
	{
		if (ft_strcmp(g_red[i].value, value) == 0)
			return (g_red[i].func(in, out));
		i++;
	}
	return (1);
}
