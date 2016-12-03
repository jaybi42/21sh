/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/08 17:56:10 by jguthert          #+#    #+#             */
/*   Updated: 2016/11/29 14:34:52 by malaine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_21SH_H
# define FT_21SH_H

/*
** TODO: Suppr stdio
*/

#define NAME "21sh"

#define KV_SET 0
#define KV_GET 1
#define KV_DEL 2

# include "libft.h"
# include "edit_line.h"

# define INIT_ALL(CMD,AV1,AV2) (char *[]){CMD, AV1, AV2, NULL}
# define INIT_ARG(AV1,AV2) (char *[]){AV1, AV2, NULL}
# define INIT_AV(A,B,C,D) ((t_av){A, INIT_ARG(B, C), D, INIT_ALL(A, B, C), NULL, NULL, NULL, 0})
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

typedef struct		s_alias
{
	char		*key;
	char		*value;
	struct s_alias	*next;
}			t_alias;

typedef struct	s_prompt
{
	int			rand;
	t_list		*g_env;
	t_list		*l_env;
	bool		son;
	t_line		*l;
}				t_prompt;

typedef struct s_file
{
	int			len_str;
	int			len_buff;
	char		*str;
	char		buffer[4096 + 1];
}				t_file;

/*
---- added by a
*/

#define READER 0
#define WRITER 1

/*
** packets
*/

#define WRITING 1024

typedef struct s_sf
{
  char  s[WRITING];
  int len;
  struct s_sf *next;
}              t_sf;

t_sf *create_packet(char *b, int len);


#define BUILTIN 1
#define BIN 0
#define GET 0
#define SET 1

typedef struct          s_redirect
{
                int     type; //0 for redirect basic or 1 for <
                int     fd_in;
                int     fd_out; //if it's -1 we look for path
								char *path;
								int open_flag;
								int fd; // in "cat -e < file.txt", fd = open("file.txt")
}                       t_redirect;

typedef struct	s_exec
{
	int type; //-1 = error, 0 = bin, 1 = builtin
	char *path;
	char **argv;
	t_redirect **r;
	int		(*fnct)();
}				t_exec;

typedef struct          s_command
{
                int     type;
                int     argc;
                char    **argv;
                int     *bitcode;
}						t_command;

/*
----
*/


char *handle_var(int state, char *key, char *value);

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
	char		**argv;
	int		**argv_auth;
	int			bg;
	struct s_redirect **redirect;
    int     *bitcode;
	int		type;
}							t_av;

typedef int		(*t_bi_fptr)();

typedef struct	s_builtin
{
	char		*key;
	t_bi_fptr	value;
}				t_builtin;

void		clean_exit(int ret);


void print_err(char *err, char *what);

char *get_path(t_list *g_env, t_list *l_env);
char **convert_env(t_list *g_env, t_list *l_env);
char **get_allpath(char *cmd, char *path);

/*
**
**	Garbage collector like
**
*/

//same behaviour as malloc
void	*xmalloc(size_t size);
//same behaviour as ft_memdel
void	xmemdel(void **p);
//special function: free al the pointers you have previously malloced with xmalloc
void	xmasterfree(void);
//print the mem you have create with xmalloc and currently didn't free
void	xprintmem(void);

/*
** parser
*/
char		*cpy_a_to_b(char *str, int a, int b);
char            **fstrsplit(char *str, int len, int (*is_whatever)(char));
char **join_array(char ***t);
char	**copy_array_begin(size_t b, char **array);
/*
**Name: Parsing
**File: read.c get_env.c
**Desc: Parse read and env
*/

t_av**			read_init(t_line *l, t_ftl_root *hist);
int				init_env(t_list **g_env, t_list **l_env);
void			storage_env(t_list **e, char *new_var);
int		x_strjoins(char **s1, size_t *len1, char *s2, size_t len2);
char			*get_input(char *prompt);

/*
**Name: Shell
**File: shell.c print_prompt.c do_exec.c
**Desc: Do shell functions
*/

t_output	shell(t_av **av, int ret);
int				do_fork(char *bin, char **all, char **env);
void			get_alias(t_av **av);
int				array_key_exists(t_alias *alias, char *key);
char			*get_alias_elem_by_id(t_alias *alias, int id);
int				check_bin(t_list *g_env, t_list *l_env, t_av av);
void			print_prompt(int rand, t_list *g_env, t_list *l_env, t_line *l);
void			catch_signal(void);

/*
**Name: history
**File: history.c parse_history.c
**Desc: Function to use history builtin
*/

void			history(void);
int				put_history(t_ftl_root *root);
int				get_history(t_ftl_root *root);
int                             add_history(char *str, t_ftl_root *root);

/*
**Name: builtin
**File: builtin.c bi_*.c
**Desc: Do the builtin if it exist
*/
int				builtin(t_list **g_env, t_list **l_env, t_ftl_root *hist,t_av *av);
//int				builtin(t_av av, t_list **g_env, t_list **l_env,t_ftl_root *hist);
int				bi_cd(t_av av, t_list **g_env, t_list **l_env);
int				bi_env(t_av av, t_list **g_env, t_list **l_env);
int				bi_unsetenv(t_av av, t_list **g_env, t_list **l_env);
int				bi_setenv(t_av av, t_list **g_env, t_list **l_env);
int				bi_exit(t_av av, t_list **g_env, t_list **l_env);
int				bi_getenv(t_av av, t_list **g_env, t_list **l_env);
int				bi_echo(t_av av, t_list **g_env, t_list **l_env);
int				bi_clear(t_av av, t_list **g_env, t_list **l_env);
int				bi_history(t_av av, t_list **g_env, t_list **l_env);
int                             bi_export(t_av av, t_list **g_env, t_list **l_env);

int			bi_42info(t_av av, t_list **g_env, t_list **l_env);
int			bi_glob(t_av av, t_list **g_env, t_list **l_env);


int				bi_alias(t_av av, t_list **g_env, t_list **l_env);
/*
**Name: Free list
**File: free_list.c
**Desc: free list functions to work with ft_lstdel() && ft_lstdelone()
*/

void			free_av(void *content, size_t size);
void			free_env(void *content, size_t size);
void			free_key(void *conent, size_t size);

/*
**Name: Print error
**File: print_error.c
**Desc: print error from builtins
*/

int				print_error(t_av av, int error);

/*
** added by a
*/
#define TO_EXEC_IN_BG -10
#define TYPE_OTHER 0
#define TYPE_PIPE 7
#define TYPE_OR 4
#define TYPE_AND 6


t_av **parse_commands(char *expr);
int				ret_exit(int state, int value);
int			fd_get_binary(int fd, char **str, int *len);
int			char_is_whitespace(char c);
char            **fstrsplit(char *str, int len, int (*is_whatever)(char));
char	**ft_globing(char *expr, char **words);
t_output shell_exec(char *expr);
char				**x_strsplit(char const *s, char c);
char	*x_strjoin(char const *s1, char const *s2);
char	*x_strdup(char const *src);
char	*x_strnew(size_t size);

int			x_fd_get_binary(int fd, char **str, int *len);
int			fd_get_binary(int fd, char **str, int *len);
int			file_get_binary(char *filename, char **str, int *len);
void			create_or_update_key(t_alias **addr, char **pair);
void			del_pair(char ***pair);
size_t		ft_size_tab(char **tab2);

char	*tilde_path(char *str, char *home);


/*
** global
*/

extern t_alias		*g_alias;
extern t_list		*l_env;
extern t_list		*g_env;
extern t_ftl_root g_hist;
extern t_line *g_line;
extern t_prompt g_prompt;
extern int *g_exit;
extern int g_debug;

#endif
