/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/08 17:56:10 by jguthert          #+#    #+#             */
/*   Updated: 2016/09/25 17:22:24 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_21SH_H
# define FT_21SH_H

/*
** TODO: Suppr stdio
*/

# include "libft.h"
# include "edit_line.h"

# define INIT_ALL(CMD,AV1,AV2) (char *[]){CMD, AV1, AV2, NULL}
# define INIT_ARG(AV1,AV2) (char *[]){AV1, AV2, NULL}
# define INIT_AV(A,B,C,D) ((t_av){A, INIT_ARG(B, C), D, INIT_ALL(A, B, C)})
# define STAT	struct stat
# define PW_T	struct passwd

# define STDIN	0
# define STDOUT	1

typedef void	(*t_sig) (int);
typedef int		(*t_red_func)();

typedef struct	s_red
{
	t_red_func	func;
	char		*value;
}				t_red;

typedef struct	s_env
{
	char		*name;
	char		*value;
	char		*str;
}				t_env;

typedef struct	s_prompt
{
	int			rand;
	t_list		*g_env;
	t_list		*l_env;
	bool		son;
	t_line		*l;
}				t_prompt;

typedef struct	s_error
{
	bool		cmd;
	char		*str;
	bool		arg;
}				t_error;

typedef struct	s_hist
{
	t_ftl_node	node;
	char		*str;
}				t_hist;

typedef struct	s_av
{
	char		*cmd;
	char		**arg;
	int			argc;
	char		**all;
}				t_av;

typedef int		(*t_bi_fptr)();

typedef struct	s_builtin
{
	char		*key;
	t_bi_fptr	value;
}				t_builtin;

/*
**Name: Parsing
**File: read.c get_env.c
**Desc: Parse read and env
*/

int				read_init(t_list **av_list, t_line *l, t_ftl_root *hist);
int				init_env(t_list **g_env, t_list **l_env);

/*
**Name: Shell
**File: shell.c print_prompt.c do_exec.c
**Desc: Do shell functions
*/

int				shell(t_list *av_list, t_list **g_env, t_list **l_env, t_ftl_root *hist);
int				do_fork(char *bin, char **all, char **env);
int				check_bin(t_av av, t_list *g_env, t_list *l_env);
void			print_prompt(int rand, t_list *g_env, t_list *l_env, t_line *l);
void			catch_signal(t_prompt prompt);

/*
**Name: history
**File: history.c parse_history.c
**Desc: Function to use history builtin
*/

void			history(void);
int				put_history(t_ftl_root *root);
int				get_history(t_ftl_root *root);
int				add_history(char *str, t_ftl_root *root);

/*
**Name: builtin
**File: builtin.c bi_*.c
**Desc: Do the builtin if it exist
*/

int				builtin(t_av av, t_list **g_env, t_list **l_env,
						t_ftl_root *hist);
int				bi_cd(t_av av, t_list **g_env, t_list **l_env);
int				bi_env(t_av av, t_list **g_env, t_list **l_env);
int				bi_unsetenv(t_av av, t_list **g_env, t_list **l_env);
int				bi_setenv(t_av av, t_list **g_env, t_list **l_env);
int				bi_exit(t_av av, t_list **g_env, t_list **l_env);
int				bi_getenv(t_av av, t_list **g_env, t_list **l_env);
int				bi_echo(t_av av, t_list **g_env, t_list **l_env);
int				bi_history(t_av av, t_ftl_root *hist);

/*
**Name: Free list
**File: free_list.c
**Desc: free list functions to work with ft_lstdel() && ft_lstdelone()
*/

void			free_av(void *content, size_t size);
void			free_env(void *content, size_t size);

/*
**Name: Print error
**File: print_error.c
**Desc: print error from builtins
*/

int				print_error(t_av av, int error);

#endif
