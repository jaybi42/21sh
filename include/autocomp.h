/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autocomp.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mseinic <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/14 17:06:38 by mseinic           #+#    #+#             */
/*   Updated: 2016/12/05 20:15:05 by mseinic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AUTOCOMP_H
# define AUTOCOMP_H
# include <dirent.h>
# include <stdlib.h>
# include <limits.h>
# include <unistd.h>
# include "21sh.h"

typedef struct s_ft_info
{
	int				count;
	char			*complete;
	int				size;
	int				complete_size;
	char			*aux;
	char			**tab_ret;
	int				function;
	int				index;
	char			*copy;
	int				k;
	char			*copy2;
	int				globing;
}					t_ft_info;


typedef struct		s_aut_info
{
	DIR				*dirp;
	struct dirent	*dp;
	size_t			len;
	size_t			size;
	char			*tmp;
	int				i;
	char			**tab_ret;
}					t_aut_info;

void				ft_swap_print(t_line *l);
char				**ret_match(char *str);
char				**command_fnc(char *str);
int					count_files(char *path, char *str);
char				**ret_tab(char *tmp, char *path);
void				del_tab(char **tab_ret);
int					tab_size(char **tab_ret);
char				**tab_dup(char **tab_ret);
char				**append_found(char **t, char *str, char *path);
int					cmp_buf(int *value, char *buf);
void				insert_ontop_string(t_ft_info *info, t_line *l, char *data);
void				insert_in_string(t_ft_info *info, t_line *l, char *data);
int					test_if_fnc(int i, char *str, int *glob);
void				init(t_ft_info *info, t_line *l);
char				**ret_match(char *str);
char				**command_fnc(char *str);
char				**ret_globing(char *tmp, char *path);
char				**ret_tab(char *tmp, char *path);
int					count_files(char *path, char *str);
int					auto_my_cmp(char *d_name, char *tmp, char *str);
void				get_slashes(char **t);
char				**del_return_null(char ***t);
void				init_var(int *n, char ***t, char ***tab2, char ***ret);
int					auto_my_cmp(char *d_name, char *tmp, char *str);
int					is_dir_file(const char *path);
void				add_slash(t_aut_info *info);
int					verify_if_dir(t_aut_info *info, char *path);
char				*ret_glob(char *str);
#endif
