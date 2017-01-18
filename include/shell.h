/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agadhgad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/11 23:28:34 by agadhgad          #+#    #+#             */
/*   Updated: 2017/01/09 17:13:36 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H

# define NAME "42sh"

# define KV_SET 0
# define KV_GET 1
# define KV_DEL 2

# define PRINT 0
# define INSERT 1
# define DELETE 2
# define DELETEALL 3
# define LEN 10

# include "libft.h"
# include "edit_line.h"
# include "autocomp.h"
# include <stdlib.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>

# define IL(CMD,AV1,AV2) (char *[]){CMD, AV1, AV2, NULL}
# define IG(AV1,AV2) (char *[]){AV1, AV2, NULL}
# define N NULL
# define INIT_AV(A,B,C,D) ((t_av){A, IG(B, C), D, IL(A, B, C), N, -1, N, 0})
# define STAT	struct stat
# define PW_T	struct passwd

# define MAXSIZE	2048

# define STDIN	0
# define STDOUT	1

typedef void	(*t_sig) (int);
typedef int		(*t_red_func)();

typedef struct			s_red
{
	t_red_func			func;
	char				*value;
}						t_red;

typedef struct			s_env
{
	char				*name;
	char				*value;
	char				*str;
}						t_env;

typedef struct			s_alias
{
	char				*key;
	char				*value;
	struct s_alias		*next;
}						t_alias;

typedef struct			s_prompt
{
	int					rand;
	t_list				*g_env;
	t_list				*l_env;
	bool				son;
	t_line				*l;
	int					onshell;
}						t_prompt;

# define READLEN 4096

typedef struct			s_file
{
	int					len_str;
	int					len_buff;
	char				*str;
	char				buffer[4096 + 1];
}						t_file;

char					*file_get_contents(char *filename);
char					*allocat(int size);
char					*add_alloc(int fd, int count, t_file *f);
char					*binary_cat(char *dest, int len_dest, char *src,
						int len_src);
char					*binary_cpy(char *src, char *dest, int len);
char					*x_allocat(int size);

# define LEN_INFOS 2

typedef struct			s_info
{
	char				*name;
	char				*path;
}						t_info;

# define READER 0
# define WRITER 1

/*
** packets
*/

# define WRITING 1024

typedef struct			s_sf
{
	char				s[WRITING];
	int					len;
	struct s_sf			*next;
}						t_sf;

t_sf					*create_packet(char *b, int len);

# define BUILTIN 1
# define BIN 0
# define GET 0
# define SET 1

typedef struct			s_redirect
{
	int					type;
	int					fd_in;
	int					fd_out;
	char				*path;
	int					open_flag;
	int					fd;
}						t_redirect;

typedef struct			s_exec
{
	int					type;
	char				*path;
	char				**argv;
	t_redirect			**r;
	int					(*fnct)();
}						t_exec;

typedef struct			s_command
{
	int					type;
	int					argc;
	char				**argv;
	int					*bitcode;
}						t_command;

char					*handle_var(int state, char *key, char *value);

typedef struct			s_error
{
	bool				cmd;
	char				*str;
	bool				arg;
}						t_error;

typedef struct			s_hist
{
	t_ftl_node			node;
	char				*str;
}						t_hist;

typedef struct			s_av
{
	char				*cmd;
	char				**arg;
	int					argc;
	char				**argv;
	int					**argv_auth;
	int					bg;
	struct s_redirect	**redirect;
	int					type;
}						t_av;

typedef struct			s_sh
{
	t_av				**av;
	t_line				l;
	uint64_t			nbr;
	t_output			o;
}						t_sh;

typedef int				(*t_bi_fptr)();

typedef struct			s_builtin
{
	char				*key;
	t_bi_fptr			value;
}						t_builtin;

void					clean_exit(int ret);
void					print_err(char *err, char *what);
char					*get_path(t_list *g_env, t_list *l_env);
char					**convert_env(t_list *g_env, t_list *l_env);
char					**get_allpath(char *cmd, char *path);
char					*defragement(char *s);

/*
**	Garbage collector like
*/

void					*storage(int action, void *package);
void					*xmalloc(size_t size);
void					xmemdel(void **p);
void					xmasterfree(void);
void					xprintmem(void);

/*
** parser
*/

char					*cpy_a_to_b(char *str, int a, int b);
char					**fstrsplit(char *str, int len,
						int (*is_whatever)(char));
char					**join_array(char ***t);
char					**copy_array_begin(size_t b, char **array);

/*
** Name: Parsing
** File: read.c get_env.c
** Desc: Parse read and env
*/

t_av					**read_init(t_line *l, t_ftl_root *hist);
int						init_env(t_list **g_env, t_list **l_env);
void					storage_env(t_list **e, char *new_var);
void					print_all_exported_variables(t_list *env);
int						x_strjoins(char **s1, size_t *len1,
						char *s2, size_t len2);
char					*get_input(char *prompt);

/*
**	Name: Shell
**	File: shell.c print_prompt.c do_exec.c
**	Desc: Do shell functions
*/

t_output				shell(t_av **av, int ret);
int						do_fork(char *bin, char **all, char **env);
char					*get_alias(char *key);
int						array_key_exists(t_alias *alias, char *key);
char					*get_alias_elem_by_id(t_alias *alias, int id);
int						check_bin(t_list *g_env, t_list *l_env, t_av av);
void					print_prompt(int rand, t_list *g_env,
						t_list *l_env, t_line *l);
void					catch_signal(void);
void					uncatch_signal(void);

/*
**	Name: history
**	File: history.c parse_history.c
**	Desc: Function to use history builtin
*/

void					history(void);
int						put_history(t_ftl_root *root);
int						get_history(t_ftl_root *root);
int						add_history(char *str, t_ftl_root *root);

/*
**	Name: builtin
**	File: builtin.c bi_*.c
**	Desc: Do the builtin if it exist
*/

int						builtin(t_list **g_env, t_list **l_env,
						t_ftl_root *hist, t_av *av);
int						bi_cd(t_av av, t_list **g_env, t_list **l_env);
int						bi_env(t_av av, t_list **g_env, t_list **l_env);
int						bi_unsetenv(t_av av, t_list **g_env, t_list **l_env);
int						bi_unset(t_av av, t_list **g_env, t_list **l_env);
int						bi_setenv(t_av av, t_list **g_env, t_list **l_env);
int						bi_exit(t_av av, t_list **g_env, t_list **l_env);
int						bi_getenv(t_av av, t_list **g_env, t_list **l_env);
int						bi_echo(t_av av, t_list **g_env, t_list **l_env);
int						bi_clear(t_av av, t_list **g_env, t_list **l_env);
int						bi_history(t_av av, t_list **g_env, t_list **l_env);
int						bi_export(t_av av, t_list **g_env, t_list **l_env);
void					option_p(t_av av, t_list **g_env);
int						bi_42info(t_av av, t_list **g_env, t_list **l_env);
int						bi_glob(t_av av, t_list **g_env, t_list **l_env);
int						bi_alias(t_av av, t_list **g_env, t_list **l_env);

/*
**	Name: Free list
**	File: free_list.c
**	Desc: free list functions to work with ft_lstdel() && ft_lstdelone()
*/

void					free_av(void *content, size_t size);
void					free_env(void *content, size_t size);
void					free_key(void *conent, size_t size);

/*
**	Name: Print error
**	File: print_error.c
**	Desc: print error from builtins
*/

int						print_error(t_av av, int error);

# define TO_EXEC_IN_BG -10
# define TYPE_OTHER 0
# define TYPE_PIPE 7
# define TYPE_OR 4
# define TYPE_AND 6

t_av					**parse_commands(char *expr);
int						ret_exit(int state, int value);
char					*delete_key(char ***keys, char ***values, char *key);
int						search_key_values(char **keys, char **values,
						char *key);
int						insert_arr(char ***a, char *s);
int						fd_get_binary(int fd, char **str, int *len);
int						char_is_whitespace(char c);
char					**fstrsplit(char *str, int len,
						int (*is_whatever)(char));
char					**ft_globing(char *expr, char **words);
t_output				shell_exec(char *expr);
char					**x_strsplit(char const *s, char c);
char					*x_strjoin(char const *s1, char const *s2);
char					*x_strdup(char const *src);
char					*x_strnew(size_t size);
int						x_fd_get_binary(int fd, char **str, int *len);
int						fd_get_binary(int fd, char **str, int *len);
int						file_get_binary(char *filename, char **str, int *len);
void					create_or_update_key(t_alias **addr, char **pair);
void					del_pair(char ***pair);
size_t					ft_size_tab(char **tab2);
char					*tilde_path(char *str, char *home);

/*
**	GLOB
*/

typedef struct			s_globbing
{
	int					is_looking;
	int					list;
	char				*exprs;
	int					length_one;
}						t_globbing;

typedef struct			s_norm_fg
{
	int					*d;
	int					*d_bannish;
	int					len;
	int					a_g;
	int					a_w;
	char				**ret;
}						t_norm_fg;

typedef struct			s_norm_glob
{
	t_globbing			***tgs;
	int					*gti;
	int					*gta;
	char				**exprs;
	t_globbing			*g_tmp;
	int					c;
	int					ta;
	char				***t;
}						t_norm_glob;

typedef struct			s_brack
{
	char				**t;
	int					*i_a;
	int					ta;
	int					len;
}						t_brak;

typedef struct			s_arr
{
	char				**t_s;
	int					*t_a;
	int					t_i;
	int					i;
	int					tog;
	int					len;
}						t_arr;

typedef struct			s_finder
{
	int					a_w;
	int					a_w_jugement;
	int					debug_list;
	int					find;
	int					leave;
	int					**p_d;
	int					**p_d_bannish;
}						t_finder;

int						accolade_isit(char c);
int						tlen(char **t);
char					*cpy_a_to_b2(char *str, int a, int b);
char					**ret_arr(char *s);

int						parse_glob(t_norm_glob *g);
char					**brace_handler(char *s);
t_globbing				*quarter(char *expr, int *tmp_len);
t_globbing				*rec_g(char *expr, int *i, int iquare);
void					update_t(char ***p_t, int **p_i_a, int *p_ta, char c);
char					**find_globbing(t_globbing **gs, int a, char **words);
void					finder_s_j_soj(t_finder *f, t_globbing *g,
						char **words);
void					finder_s_j_soj_list(t_finder *f, t_globbing *g,
						char **words);
void					finder_s_j_list(t_finder *f, t_globbing *g,
						char **words);
void					finder_not_suspension_judgement(t_finder *f,
						t_globbing *g, char **words);
int						ret_virgule(t_arr *r);
char					*x_strjoin2(char *s1, size_t len1, char *s2,
						size_t len2);
char					*x_strdup2(char *s, int len);
char					*x_strndup(const char *s1, size_t n);
char					*x_strsub(char const *s, unsigned int start,
						size_t len);
size_t					len_depth(char *s);
char					**fusion_tarray(char ***t);
int						ttlen(char ***t);
void					shell_exec_in(char *expr);

/*
**	SHELL
*/

# define TRUE 1
# define FALSE 0
# define TYPE_REDIRECT_NORMAL 0
# define TYPE_REDIRECT_TRUNC 1

typedef struct			s_f
{
	int					fd[2];
	int					oldout;
	int					tempout;
	int					fd_to_redir;
	int					fd_to_write;
}						t_f;

# define NB_BUILT 14
# define BASIC 0
# define IGNORE 1

typedef struct			s_pipe
{
	int					activate;
	int					fds[2];
}						t_pipe;

typedef struct			s_built_redir
{
	int					cpystdout;
	int					cpystderr;
	t_pipe				p[3];
	int					x;
	int					b[50];
	int					special;
	int					fdout;
}						t_built_redir;

typedef struct			s_handle_r
{
	int					is_redirecting;
	int					b_out;
	int					fdout[2];
	int					b_err;
	int					fderr[2];
	t_sf				*packets_out;
	t_sf				*packets_err;
	t_pipe				p[3];
	t_pipe				**pr;
}						t_handle_r;

/*
**	struct for handle the shell norme
*/

typedef struct			s_executor
{
	t_exec				ex;
	t_av				av;
}						t_executor;

typedef struct			s_shells
{
	int					a;
	t_exec				ex;
	t_output			all;
	t_output			output;
	t_executor			**stack;
	int					stack_index;
	int					find;
}						t_shells;

typedef struct			s_do_exec
{
	t_output			o;
	int					fdout[2];
	char				**env;
	int					i;
	int					y;
	char				b[WRITING];
	int					r;
}						t_do_exec;

typedef struct			s_solvefive
{
	pid_t				pid;
	int					fdin;
}						t_solvefive;

/*
**	shell
*/

int						shell_pre_exec(t_shells *s, t_av **av);
int						shell_post_exec(t_shells *s, int ret);
t_output				shell(t_av **av, int ret);

/*
**	shell2
*/

void					clear_output(t_output *o);
void					shell_init(t_shells *s);
void					shell_print_debug(t_shells *s, int w, int v);
int						shell_pre_exec_logical_and_or(t_shells *s, t_av **av);
int						shell_pre_exec_logical_pipe(t_shells *s, t_av **av);

/*
**	exec
*/

int						exec_all(t_executor **exs, char **env, int fdin);
t_output				do_exec(t_executor **exs, int ret);
void					impossibru_error(char *s);
int						special_redir(t_redirect **r);
void					writor(t_executor **exs, int fd, int *b, int x);

/*
**	exec1
*/

void					exec_bin_child(t_executor **exs, int fdin,
									t_handle_r *hr, char **env);
void					redir_out_father(t_executor **exs, t_handle_r *hr,
		char **env, int pid);
void					redir_err_father(t_executor **exs, t_handle_r *hr);
/*
**	exec2
*/

t_sf					*read_from_fd(int fd);
void					father_handle_redirect(t_handle_r *hr);
void					active_redirect(t_redirect **r, t_handle_r *hr);
void					son_handle_in(int fdin, t_redirect **r);
void					init_handle_redirect(t_redirect **redirect,
						t_handle_r *hr, int ispipe);

/*
**	exec3
*/

void					impossibru_error(char *s);
void					son_handle_redirect(t_handle_r *hr);
void					switch_fd_begin(int fd, int *new_fd);
void					switch_fd_end(int fd, int *old_fd);
void					dup_writer_pipe(int fd, int writer);

t_sf					*read_from_fd(int fd);
void					exec_builtin_init(t_executor **exs, int *cpystdout,
						int *cpystderr, t_handle_r *hr);
void					exec_builtin(t_executor **exs, t_handle_r *hr,
						char **env);

/*
**	prepare_exec
*/

t_exec					make_exec_bin(t_av *av, t_list *g_env, t_list *l_env);
void					make_r(t_redirect ***r);
t_exec					make_exec(t_av *av, t_list **g_env, t_list **l_env);

/*
**	prepare_exec2
*/

int						get_out(t_redirect ***r, int fd_out);
char					**convert_env(t_list *g_env, t_list *l_env);
char					**get_allpath(char *cmd, char *path);
int						exec_path(char *cmd, char **path);
t_exec					make_exec_builtin(t_av *av);

/*
**	prepare_exec3
*/

char					*get_path(t_list *g_env, t_list *l_env);

/*
**	shelluse
*/

int						x_strjoins(char **s1, size_t *len1,
						char *s2, size_t len2);
void					set_retcode(unsigned char ret_code);
void					clear_stack(t_executor ***stack, int *stack_index);
void					update_stack(t_executor ***p_stack, int *p_stack_index,
						t_exec *current_ex, t_av *current_av);
void					extend_stack(t_executor ***stack, int *stack_index,
						t_exec *current_ex, t_av *current_av);

/*
** shelluse2
*/

void					print_err(char *err, char *what);
void					extend_stack(t_executor ***stack, int *stack_index,
						t_exec *current_ex, t_av *current_av);

/*
**	PARSER
*/

typedef struct			s_nparse
{
	int					nb;
	int					*type;
	int					*begin;
	int					*end;
	int					failed;
}						t_nparse;

typedef struct			s_norm_quarter
{
	int					i;
	int					a_str;
	t_globbing			*globbing;
	int					c;
}						t_norm_quarter;

# define NUMBER_DELIMITER 8
# define NUMBER_REDIRECT 5
# define TRUE 1
# define FALSE 0
# define UNTOUCHABLE -2
# define TYPE_UNDEFINED -1
# define TYPE_CONNECTOR 0
# define TYPE_NORMAL 1
# define TYPE_UNTOUCHABLE 2

typedef struct			s_redirect_info
{
	char				*name;
}						t_redirect_info;

typedef struct			s_delimiter
{
	char				*name;
	char				*description;
	int					wait;
	int					wait_another;
	int					do_recursivity;
	int					dont_give_shit_about_whitespace;
	int					is_redirection;
	int					is_arg;
}						t_delimiter;

typedef struct			s_parse
{
	int					*begin;
	int					*end;
	int					*type;
	int					nb;
	int					current;
	int					one_arg;
	int					dec;
	int					quote_activate;
}						t_parse;

# define EMPTY -1
# define ENDI -2

static t_delimiter const	g_delimiter[NUMBER_DELIMITER] =
{
	{"\"", "dquote", 1, 1, 0, 0, 0, 1},
	{"\'", "quote", 1, 1, 0, 0, 0, 1},
	{"`", "bquote", 1, 1, 1, 1, 0, 1},
	{"\\", "", 1, 0, 0, 0, 0, 1},
	{"||", "cmdor", 1, 0, 1, 0, 0, 0},
	{";", "pvrig", 1, 0, 1, 0, 0, 0},
	{"&&", "cmdand", 1, 0, 1, 0, 0, 0},
	{"|", "pipe", 1, 0, 1, 0, 0, 0}
};

static t_delimiter const	g_delimiter_quo[3] =
{
	{"\"", "dquote", 1, 1, 0, 0, 0, 1},
	{"\'", "quote", 1, 1, 0, 0, 0, 1},
	{"`", "bquote", 1, 1, 1, 1, 0, 1}
};

/*
**	parser
*/

typedef struct			s_norm_simplify
{
	char				**expr;
	int					**t_ind;
	int					**l_ind;
	int					a;
	int					i;
}						t_norm_simplify;

typedef struct			s_norm_hag
{
	int					i;
	int					a;
	int					continu;
	int					len;
	int					first_time;
}						t_norm_hag;

void					handle_alias_glob(char **expr, int **t_ind,
										int **l_ind);
void					inject_in_string(char **s1, char *s2, int i_to_insert,
										int del_length);
void					update_debug(t_av **av, int i);
void					update_arg(t_av ***av, int i);
t_av					**updated(t_av **av);
t_av					**nparse(char *expr, int *t_ind, int *l_ind);
t_av					**parse_commands(char *expr);
void					exclude_s(t_norm_simplify *t);
void					simplify_space(char **expr, int **t_ind, int **l_ind);

/*
**	parser2
*/

typedef struct			s_norm_parse
{
	t_nparse			np;
	int					i;
	int					i32_tmp;
}						t_norm_parse;

typedef struct			s_norm_convert_parse
{
	t_av				**cmds;
	int					id_cmds;
	int					i;
	char				**expr;
}						t_norm_convert_parse;

t_av					**convert_parse(char *expr, t_nparse np,
						int *t_ind, int *l_ind);
t_nparse				parse(char *expr, int *t_ind, int *l_ind);
void					convert_parse_if1(t_norm_convert_parse *t, t_nparse np);
void					convert_parse_if2(t_norm_convert_parse *t,
						t_nparse np, int *t_ind, int *l_ind);
t_av					**convert_parse(char *expr, t_nparse np,
						int *t_ind, int *l_ind);

/*
**	parser3
*/

typedef struct			s_norm_connector
{
	int					id_tab;
	char				*s;
	int					num_tmp;
	int					i_tmp;
}						t_norm_connector;

int						is_intouchable(int i, int *t_ind, int *l_ind);
int						is_connector(char *tested_s, int i,
						int *t_ind, int *l_ind);
int						nparse_close(t_nparse *np, int id_close);
void					nparse_extend(t_nparse *np, int id_open);

/*
**	parser4
*/

typedef struct			s_handle_d_param
{
	t_nparse			np;
	int					i;
}						t_handle_d_param;

t_handle_d_param		create_np(t_nparse np, int i);
void					nparse_init(t_nparse *np, char *expr);
void					init_cmd(t_av **cmd, size_t len);
char					*xget_string_l(char *s, int l);
int						get_type(char *s);
int						*handle_d(t_handle_d_param t, int *t_ind,
						int *l_ind, int len);

/*
**	parser5
*/

int						in_cmd_is_intouchable(t_av *cmd, int id_argv, int i);
void					convert_other(t_av ***cmds);
void					p_s_a_init(t_norm_simplify *t, char **tmp);

/*
**	parseruse
*/

void					delete_c(t_av **cmd, int id_argv, int i, int *pi);
void					delete_s(t_av **cmd, int id_argv);
int						is_whitespace(char c);
void					print_step1(char *expr, int *t_ind, int *l_ind);

/*
**	parseruse2
*/

typedef struct			s_norm_j_s_a
{
	int					i;
	size_t				len;
	char				*ns;
	int					len_ind;
	int					**t_ind;
	int					**l_ind;
}						t_norm_j_s_a;

int						char_is_whitespace(char c);
char					*cpy_a_to_b(char *str, int a, int b);
char					*find_home(char **env);
char					*join_string_array(char **a, int *marked_ind,
						int **t_ind, int **l_ind);

/*
**	parseruse3
*/

void					reset_current(t_parse *p, int index);
void					update_new(t_parse *p, int index, int delimiter_index);

/*
**	preparse
*/

typedef struct			s_norm_parse2
{
	char				*expr;
	int					len;
	t_parse				*p;
	int					i;
	t_delimiter			*d;
	int					l;
}						t_norm_parse2;

typedef struct			s_norm_d_p
{
	char				**ts;
	t_parse				*p;
	int					*marked_ind;
	int					i;
	t_output			o;
}						t_norm_d_p;

typedef struct			s_norm_handle_tilde
{
	char				*home;
	int					i;
	char				**tmpx;
	char				*tmp;
	char				**ret;
	int					find;
	char				*ret_s;
	size_t				len;
}						t_norm_handle_tilde;

typedef struct			s_c_v
{
	int					i;
	char				**tmp;
	char				*tmpe;
	char				**ret;
	int					x;
}						t_c_v;

typedef struct			s_apply_var
{
	int					i;
	char				*ns;
	char				**tmp;
	size_t				len;
	char				**env;
}						t_apply_var;

char					*handle_tilde(char *s, char **env);
char					*decortique_parse(char *expr, size_t l,
						int **t_ind, int **l_ind);

/*
**	preparse2
*/

char					**check_var(char *s, char **env);
char					*apply_var(char *s, int do_extra);

/*
**	preparse3
*/

t_parse					*parse_it2(char *expr, int len, t_delimiter *d, int l);

/*
**	redirection
*/

typedef struct			s_set_redir
{
	char				*s;
	t_av				*cmd;
	int					findfile;
	t_redirect			*redir;
	int					fd;
	int					len;
	int					b;
	int					x;
	int					i;
	int					id_argv;
}						t_set_redir;

typedef struct			s_g_a_p
{
	char				**env;
	char				**path;
	char				*str;
	t_exec				ex;
	int					ret;
}						t_g_a_p;

int						set_redir_init(t_set_redir *t, t_av **pcmd, char *r);
int						set_redir(t_av **pcmd, int id_argv, int i, char *r);
int						get_redirect(t_av **cmd);

/*
**	redirection2
*/

char					*is_redir(char *s);
int						get_heredoc(char *s);

/*
**	HASH
*/

typedef struct			s_hash
{
	unsigned int		hash;
	char				*bin;
	char				*path;
	struct s_hash		*next;
}						t_hash;

/*
**	hash_functions
*/

unsigned int			hash_algo(char *key, int nb);
char					*get_hash_path(t_hash ***hash_addr, char *bin);
void					hash_del(t_hash ***hash);
t_hash					**hash_table(char *path);
void					init_bin_list(t_hash **addr, char *path);
void					set_redir_inside2_bis(t_set_redir *t, char *r);
void					set_redir_inside_bis(t_set_redir *t, t_av **pcmd);
t_exec					exec_ret_err(t_exec e, char *msg, char *cmd);

/*
**	global
*/

extern int				g_quithist;
extern int				g_inheredoc_2;
extern int				g_inheredoc;
extern t_alias			*g_alias;
extern t_list			*g_lenv;
extern t_list			*g_env;
extern t_ftl_root		g_hist;
extern t_line			*g_line;
extern t_prompt			g_prompt;
extern int				*g_exit;
extern int				g_debug;
extern t_hash			**g_hash;
extern unsigned int		g_hash_size;
extern unsigned int		g_heredoc_actif;
extern char				*g_heredoc_expected;
extern t_line			*g_heredoc;

#endif
