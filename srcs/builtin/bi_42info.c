/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_42info.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouchla <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/05 15:44:26 by ibouchla          #+#    #+#             */
/*   Updated: 2016/12/05 17:14:36 by ibouchla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"
#include <stdlib.h>

/*
** !!!!!!! DON'T ADD: %s in your info or any %*
*/

t_info g_infos[LEN_INFOS] = {
	{"|",
	"{green}#Definition :{eoc}\n\
	\'|\' is called  a \'pipe\'. it's a specific separator between\
	two commands,\n\
	The format is: \"command1 | command2\"\n\
	\n\
	{green}#How it works ?{eoc}\n\
	The output (stdout{red}(?){eoc}) of the command1 will be put in the stdin\
	{red}(?){eoc} of the command2.\n\
	\n\
	{green}#Examples :{eoc}\n\
	$>echo \"ok\" | cat -e <ENTER>\n\
	ok$\n\
	"},
	{"alias",
	"{green}#Definition :{eoc}\n\
	Alias system is a implementation of command\'s substitution,\n\
	The format is: \"{green}key{eoc}={red}value{eoc}\"\n\
	\n\
	{green}#How it works ?{eoc}\n\
	When you send a command to the shell,\
	if it\'s present in the alias\'s list,\n\
	it\'s search recursively the {red}value{eoc}\
	associated to the {green}key{eoc}.\n\
	When an infinite loop is detected, no substitution is made.\n\
	\n\
	{green}#Examples :{eoc}\n\
	$> alias \"{green}key1{eoc}={red}value1{eoc}\"\n\
	$> alias \"{green}value1{eoc}={red}ls -la -h{eoc}\"\n\
	$> key1\n\
	total 0\n\
	drwxr-xr-x   4 ibouchla  2015_paris  136 Dec  4 20:41 .\n\
	drwxr-xr-x  13 ibouchla  2015_paris  442 Dec  4 20:41 ..\n\
	drwxr-xr-x   2 ibouchla  2015_paris   68 Dec  4 20:41 bar\n\
	-rw-r--r--   1 ibouchla  2015_paris    0 Dec  4 20:41 foo\n"
	}
};

int	exec_42info(int ac, char **argv)
{
	int i;

	if (ac == 0)
	{
		ft_dprintf(2, "Usage: 42info \'?\'\n");
		return (0);
	}
	i = -1;
	while (++i < LEN_INFOS)
		if (ft_strcmp(g_infos[i].name, argv[1]) == 0)
		{
			ft_printf(g_infos[i].info);
			return (0);
		}
	ft_printf("Didn't find '%s'!\n", argv[1]);
	return (0);
}

int	bi_42info(t_av av, t_list **g_env, t_list **l_env)
{
	(void)g_env;
	(void)av;
	(void)l_env;
	return (exec_42info(av.argc, av.argv));
}
