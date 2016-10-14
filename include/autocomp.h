/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autocomp.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mseinic <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/14 17:06:38 by mseinic           #+#    #+#             */
/*   Updated: 2016/10/14 20:28:51 by mseinic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AUTOCOMP_H
# define AUTOCOMP_H
# include <dirent.h>
# include <stdlib.h>
# include <limits.h>
# include <unistd.h>
# include "21sh.h"

typedef struct  s_aut_info
{
	DIR				*dirp;
	struct dirent	*dp;
	size_t			len;
	size_t			size;
	char			*tmp;
	int				i;
	char			**tab;
}					t_aut_info;

char            **ret_match(char *str);
char            **command_fnc(char *str);
int             count_files(char *path, char *str);
char    **ret_tab(char *tmp, char *path);
void        del_tab(char **tab);
int         tab_size(char **tab);
char        **tab_dup(char **tab);
char            **append_found(char **tab, char *str, char *path);
int cmp_buf(int *value, char *buf);

#endif
