/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   storage_all_cmds.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouchla <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/28 22:16:21 by ibouchla          #+#    #+#             */
/*   Updated: 2016/11/17 18:20:33 by malaine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"
#include "autocomp.h"
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define NUMBER_DELIMITER 8
#define NUMBER_REDIRECT 5
#define TRUE 1
#define FALSE 0

#define TYPE_UNDEFINED -1
#define TYPE_CONNECTOR 0
#define TYPE_NORMAL 1
#define TYPE_UNTOUCHABLE 2

typedef struct		s_redirect_info
{
	char *name;
}									t_redirect_info;

/*
   static t_redirect_info const	g_redirect[NUMBER_REDIRECT] =
   {
   {">>"},
   {">&"},
   {">"},
   {"<<"},
   {"<"}
   };
   */
typedef struct		s_delimiter
{
	char 			*name;
	char 			*description;
	int				wait;
	/*
	   boolean for check if we must have to go to the next line
	   and wait an output if the user type it at last (if wait_another == false)
	   or (wait another == true) and the another isn't here
	   */
	int				wait_another;
	/*
	   boolean for check if we must wait another of this,
	   for example the " have to wait another ".
	   */
	int				do_recursivity;
	/*
	   if true we consider the stuff inside as a new command to parse!
false: we do nothing (it can be by example a simple redirection)
*/
	int				dont_give_shit_about_whitespace;
	/*
true: we split the white spaces inside,
otherwise we don't touch it :)
*/
	int				is_redirection;
	/*
	   don't consider it as a command, consider it as a redirection
	   */
	int				is_arg;
	/*
	   it's not a new command, it's still an arg
	   */
}					t_delimiter;

static t_delimiter const	g_delimiter[NUMBER_DELIMITER] =
{
	{"\"", "dquote", 1, 1, 0, 0, 0, 1},
	{"\'", "quote", 1, 1, 0, 0, 0, 1},
	{"`", "bquote", 1, 1, 1, 1, 0, 1},
	{"\\", "", 1, 0, 0, 0, 0, 1},
	{"||", "cmdor", 1, 0, 1, 0, 0, 0},
	{";", "pvrig", 1, 0, 1, 0, 0, 0},
	{"&&", "cmdand", 1, 0, 1, 0, 0, 0},
	{"|", "pipe", 1, 0, 1, 0, 0, 0},
	/*{">>", ">>", 0, 0, 0, 1, 1, 1},
	{">&", ">&", 0, 0, 0, 1, 1, 1},
	{">", ">", 0, 0, 0, 1, 1, 1},
	{"1>", "1>", 0, 0, 0, 1, 1, 1},
	{"2>", "2>", 0, 0, 0, 1, 1, 1},
	{"3>", "3>", 0, 0, 0, 1, 1, 1},
	{"4>", "4>", 0, 0, 0, 1, 1, 1},
	{"5>", "5>", 0, 0, 0, 1, 1, 1},
	{"6>", "6>", 0, 0, 0, 1, 1, 1},
	{"7>", "7>", 0, 0, 0, 1, 1, 1},
	{"8>", "8>", 0, 0, 0, 1, 1, 1},
	{"9>", "9>", 0, 0, 0, 1, 1, 1},
	{"<<", "heredoc", 0, 0, 0, 1, 1, 1},
	{"<", "<", 0, 0, 0, 1, 1, 1},*/
};

/*
static t_delimiter const	g_delimiter_red[14] =
{
	{">>", ">>", 0, 0, 0, 1, 1, 1},
	{">&", ">&", 0, 0, 0, 1, 1, 1},
	{">", ">", 0, 0, 0, 1, 1, 1},
	{"1>", "1>", 0, 0, 0, 1, 1, 1},
	{"2>", "2>", 0, 0, 0, 1, 1, 1},
	{"3>", "3>", 0, 0, 0, 1, 1, 1},
	{"4>", "4>", 0, 0, 0, 1, 1, 1},
	{"5>", "5>", 0, 0, 0, 1, 1, 1},
	{"6>", "6>", 0, 0, 0, 1, 1, 1},
	{"7>", "7>", 0, 0, 0, 1, 1, 1},
	{"8>", "8>", 0, 0, 0, 1, 1, 1},
	{"9>", "9>", 0, 0, 0, 1, 1, 1},
	{"<<", "heredoc", 0, 0, 0, 1, 1, 1},
	{"<", "<", 0, 0, 0, 1, 1, 1}
};*/

static t_delimiter const	g_delimiter_quo[3] =
{
	{"\"", "dquote", 1, 1, 0, 0, 0, 1},
	{"\'", "quote", 1, 1, 0, 0, 0, 1},
	{"`", "bquote", 1, 1, 1, 1, 0, 1}
};
/*
static t_delimiter const	g_delimiter_sep[5] =
{
	{"\\", "", 1, 0, 0, 0, 0, 1},
	{"||", "cmdor", 1, 0, 1, 1, 0, 0},
	{";", "pvrig", 1, 0, 1, 1, 0, 0},
	{"&&", "cmdand", 1, 0, 1, 1, 0, 0},
	{"|", "pipe", 1, 0, 1, 1, 0, 0}
};*/

typedef struct	s_parse
{
	int	*begin;
	int	*end;
	int	*type;
	int	nb;
	int	current;
	int	one_arg;
	int dec;
	int quote_activate;
}				t_parse;

#define EMPTY -1
#define ENDI -2

int			char_is_whitespace(char c)
{
	/*
	   useful notice:
	   ' '      space
	   '\t'     horizontal tab
	   '\n'     newline
	   '\v'     vertical tab
	   '\f'     feed
	   '\r'     carriage return
	   */
	if (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r')
		return (TRUE);
	else
		return (FALSE);
}

#define UNTOUCHABLE -2

void reset_current(t_parse *p, int index)
{
	p->nb++;
	p->current = EMPTY;
	p->begin[p->nb] = index + 1;
	p->end[p->nb] = 0;
	p->type[p->nb] = EMPTY;
	p->one_arg = FALSE;
	p->quote_activate = FALSE;
}

void update_new(t_parse *p, int index, int delimiter_index)
{
	p->end[p->nb++] = index;
	p->current = delimiter_index;
	p->type[p->nb] = delimiter_index;
	p->begin[p->nb] = index + ft_strlen(g_delimiter[delimiter_index].name);
	if (g_delimiter[delimiter_index].is_redirection == TRUE)
		p->begin[p->nb] = index;
	p->one_arg = FALSE;
	p->quote_activate = FALSE;
}

void		handle_delimiter2(char *expr, int *i, t_parse *p, t_delimiter *d, int l)
{
	int a;

	a = 0;
	while (a < l) //check for every delimiters if no error
	{
		if (ft_strncmp(expr + (*i), d[a].name,
					ft_strlen(d[a].name)) == 0)
		{
			if (p->current != EMPTY && d[p->current].wait_another == TRUE
					&& p->current == a)
			{
				//that's mean we find for example the second " of ""
				p->end[p->nb] = (*i);
				reset_current(p, (*i));
			}
			else if (p->current != EMPTY && d[p->current].do_recursivity == TRUE
					&& p->current == a)
			{
				//that's mean we find for example another
				update_new(p, (*i), a);
			}
			else if (p->current == EMPTY)
			{
				update_new(p, (*i), a);
			}
			(*i) += 1;
			return ;
		}
		a++;
	}
	if (p->current != EMPTY && d[p->current].is_redirection == TRUE
			&& p->one_arg == FALSE
			&& char_is_whitespace(expr[(*i)]) == FALSE)
		p->one_arg = TRUE;
	else if (p->current != EMPTY && d[p->current].is_redirection == TRUE
			&& p->one_arg == TRUE 		&& char_is_whitespace(expr[(*i)]) == TRUE)
	{
		p->end[p->nb] = (*i);
		reset_current(p, (*i));
	}
	(*i) += 1;
}

char		*cpy_a_to_b(char *str, int a, int b)
{
	char *new_str;
	int len;
	int i;

	len = (b - a < 0) ? 0: b - a;
	if (!(new_str = xmalloc(sizeof(char) * (len + 1))))
		return (NULL);
	i = 0;
	while (a + i < b)
	{
		new_str[i] = str[a + i];
		i++;
	}
	new_str[i] = '\0';
	return (new_str);
}

char            **fstrsplit(char *str, int len, int (*is_whatever)(char))
{
	char **t_str;
	int i;
	int a;
	int beg;

	if (!(t_str = xmalloc(sizeof(char **) * (len + 1))))
		return (NULL);
	t_str[0] = NULL;
	i = 0;
	a = 0;
	beg = 0;
	while (i < len && str[i] != '\0')
	{
		while (i < len && is_whatever(str[i]) && str[i++] != '\0');
		beg = i;
		while (i < len && !is_whatever(str[i]) && str[i++] != '\0');
		if (beg != i)
			t_str[a++] = cpy_a_to_b(str, beg, i);
	}
	t_str[a] = NULL;
	return(t_str);
}

t_parse		*parse_it2(char *expr, int len, t_delimiter *d, int l)
{
	int i;
	t_parse *p;

	if (!(p = xmalloc(sizeof(t_parse))) ||
			!(p->begin = xmalloc(sizeof(int) * ft_strlen(expr))) ||
			!(p->end = xmalloc(sizeof(int) * ft_strlen(expr))) ||
			!(p->type = xmalloc(sizeof(int) * ft_strlen(expr))))
		exit(0);
	i = 0;
	p->nb = 0;
	p->current = -1;
	p->begin[0] = 0;
	p->type[0] = -1;
	while (i < len && expr[i] != '\0')
		handle_delimiter2(expr, &i, p, d, l);
	if (p->current != EMPTY && d[p->current].wait_another == TRUE)
		ft_printf("[!] we were waiting another |%s| adding one for u\n", d[p->current].name);
	p->end[p->nb] = i;
	p->nb++;
	return (p);
}
/*
int		t_len(char ***t, int len)
{
	int r;
	int a;
	int i;

	r = 0;
	i = -1;
	while (++i < len)
	{
		a = -1;
		if (t[i] == NULL)
		{
			r++;
		}
		else
			while(t[i][++a])
				r++;
	}
	return (r);
}
*/
void handle_heredoc(char *expected, t_redirect **r)
{
	int i;
	size_t len;
	char *tmp;
	(*r)->s_in = malloc(sizeof(char) * 2);
	(*r)->len_in = 0;
	(*r)->s_in[0] = '\0';

	i = 0;
	len = 0;
	while (1)
	{
		tmp = get_input("heredoc>");
		if (ft_strcmp(expected, tmp) == 0)
			break;
		else
		{
			x_strjoins(&(*r)->s_in, &len, tmp, ft_strlen(tmp));
			x_strjoins(&(*r)->s_in, &len, "\n", 1);
		}
	}
	(*r)->s_in[len] = '\0';
	(*r)->len_in = (int)(len);
}

int is_delimiter(char *s)
{
	int a;

	a = 0;
	while (a < NUMBER_DELIMITER)
	{
		if (ft_strncmp(s, g_delimiter[a].name, ft_strlen(g_delimiter[a].name)) == 0)
		{
			ft_dprintf(2, "%s: parse error near `%s'\n", NAME, g_delimiter[a].name);
			return (1);
		}
		a++;
	}
	return (0);
}


char **join_array(char ***t)
{
	char **ts;
	int a;
	int i;
	int l;

	a = -1;
	l = 0;
	while (t[++a])
	{
		i = -1;
		while(t[a][++i])
		{
			//dprintf(2, "%d|%s|\n", a, t[a][i]);
			l++;
		}
	}
	ts = xmalloc(sizeof(char *) * (l + 1));
	a = -1;
	l = 0;
	while (t[++a])
	{
		i = -1;
		while (t[a][++i])
			ts[l++] = t[a][i];
	}
	ts[l] = NULL;
	return (ts);
}

char **tab_from_string(char *s)
{
	char **ts;

	ts = xmalloc(sizeof(char*) * 2);
	ts[0] = s;
	ts[1] = NULL;
	return (ts);
}

t_redirect *get_redirection(char *expr, int dec, int begin, int end, int *t_ind, int *l_ind)
{
	int i;
	t_redirect *redirect;
	char *s;

	(void)t_ind;
	(void)l_ind;
	s = cpy_a_to_b(expr + dec, begin, end);

	if (!(redirect = xmalloc(sizeof(t_redirect))) || s == NULL)
		return (NULL);
	i = 0;
	int len;
	int open_flag;
	redirect->fd_in = 1;
	redirect->type = 1;
	open_flag = O_CREAT | O_RDWR;
	len = 0;
	if (ft_isdigit(s[i])){
		redirect->fd_in = atoi(s + i);
		i++;
	}
	if (ft_strncmp(s + i, ">>", 2) == 0) {
		redirect->type = 0;
		open_flag |= O_APPEND;
		i += 2;
	}
	else if (ft_strncmp(s + i, ">", 1) == 0) {
		redirect->type = 0;
		open_flag |= O_TRUNC;
		i += 1;
	}
	else if (ft_strncmp(s + i, "<<", 2) == 0)
	{
		char **t = fstrsplit(s + i + 2, ft_strlen(s + i + 2), char_is_whitespace);
		if (t == NULL || t[0] == NULL || is_delimiter(t[0]))
			return (NULL);
		else
		{
			handle_heredoc(t[0], &redirect);
			return (redirect);
		}
	}
	else if (ft_strncmp(s + i, "<", 1) == 0)
	{
		i++;
		char **t = fstrsplit(s + i, ft_strlen(s + i), char_is_whitespace);
		int fd = open(t[0], open_flag, 0666);
		if (fd == -1)
			return (NULL);
		if (!fd_get_binary(fd, &redirect->s_in, &redirect->len_in))
			return (NULL);
		close(fd);
		return (redirect);
	}
	if (redirect->type == 0)
	{
		if (ft_strncmp(s + i, "&", 1) == 0)
		{
		 	if (ft_isdigit(s[i + 1]) && !((open_flag & O_APPEND) != 0))
			{
				redirect->fd_out = ft_atoi(s + i + 1);
				return (redirect);
			}
			else
				i++;
		}
	}
	char **t = fstrsplit(s + i, ft_strlen(s + i ), char_is_whitespace);
	if (t == NULL || t[0] == NULL || is_delimiter(t[0]))
			return (NULL);
		redirect->fd_out = open(t[0], open_flag, 0666);
	if (redirect->fd_out == -1)
			return (NULL);
	return (redirect);
}

char	**copy_array_begin(size_t b, char **array)
{
	int i;
	char **new_array;

	if (array == NULL)
		return (NULL);
	i = 0;
	while (array[i] != NULL)
		i++;
	if (!(new_array = xmalloc(sizeof(char **) * (i + 1))))
		return (NULL);
	if (i == 0)
	{
		new_array[i] = NULL;
		return (new_array);
	}
	i = 0;
	while (array[i + b] != NULL)
	{
		new_array[i] = array[i + b];
		i++;
	}
	new_array[i] = NULL;
	return (new_array);
}

t_av **check(t_av **av)
{
	int i;

	i = -1;
	/*while (av[++i])
	{
		if (av[i]->argc == 0 && (av[i]->type != TYPE_NORMAL || (av[i + 1] != NULL && av[i + 1]->type == TYPE_NORMAL)))
		{
				print_err("Parsing", "invalid null command");
				return (NULL);
			}
}*/
return (av);
}
t_av **updated(t_av **av)
{
	int i;

	i = 0;
	(g_debug) ? ft_dprintf(2, "DEBUG: END PARSE:\n") : 0;
	while (av[i] != NULL)
	{
		if (av[i]->argv != NULL)
		{
			if (av[i]->argv[0] != NULL)
			{
				av[i]->cmd = av[i]->argv[0];
				av[i]->arg = copy_array_begin(1, av[i]->argv);
				av[i]->argc--;
			}
			else
			{
				av[i]->cmd = NULL;
				av[i]->arg = NULL;
			}
		}
		else
		{
			av[i]->cmd = NULL;
			av[i]->arg = NULL;
		}
		if (g_debug)
		{
		for (int x = 0; av[i]->argv[x]; x++)
		{
			dprintf(2, "DEBUG:cmd %.*s %d argv[%d] : %%",(i + 1)*2, "---------------------------------------", i, x);
			for (int b = 0; av[i]->argv[x][b]; b++)
			{
				if (av[i]->argv_auth[x][b] == 1)
					ft_dprintf(2, "{green}%c{eoc}", av[i]->argv[x][b]);
				else
					ft_dprintf(2, "{red}%c{eoc}", av[i]->argv[x][b]);
			}
			ft_dprintf(2, "%%\n");
		}
		}
		i++;
	}
	(g_debug) ? ft_dprintf(2, "{yellow}----------------------{eoc}\n") : 0;
	return (check(av));
}

char **check_var(char *s, char **env)
{
	int i;
	char **tmp;
	char *tmpe;

	tmpe = handle_var(KV_GET, s, NULL);
	if (tmpe != NULL)
	{
		char **ret;

		ret = xmalloc(sizeof(char *) * 3);
		if (!ret)
			return (NULL);
		ret[0] = s;
		ret[1] = tmpe;
		ret[2] = NULL;
		return (ret);
	}

	i = 0;
	while (env[i])
	{
		tmp = x_strsplit(env[i], '=');
		if (tmp != NULL && tmp[0] != NULL && tmp[1] != NULL)
			if (ft_strncmp(s, tmp[0], ft_strlen(tmp[0])) == 0)
			{
				char **ret;
				ret = xmalloc(sizeof(char *) * 3);
				if (!ret)
					return (NULL);
				ret[0] = tmp[0];
				ret[1] = tmp[1];
				ret[2] = NULL;
				return (ret);
			}
		i++;
	}
	return (NULL);
}

/*
** check the var from the env!
*/
char *join_string_array(char **a, int *marked_ind, int **t_ind, int **l_ind)
{
	int i;
	size_t len;
	char *ns;
	int len_ind;

	len = 0;
	len_ind = 0;
	i = -1;
	while (a[++i])
	{
		len += ft_strlen(a[i]) + 1;
		if (marked_ind[i] == 1)
			len_ind++;
	}
	ns = xmalloc(sizeof(char) * (len + 1));
	(*t_ind) = xmalloc(sizeof(int) * (len_ind + 1));
	(*l_ind) = xmalloc(sizeof(int) * (len_ind + 1));
	i = 0;
	len = 0;
	len_ind = 0;
	while (a[i])
	{
		if (marked_ind[i] == 1)
		{
			(*t_ind)[len_ind] = len ;
			(*l_ind)[len_ind++] = ft_strlen(a[i])-1;
		}
		x_strjoins(&ns, &len, a[i], ft_strlen(a[i]));
		/*if (a[i + 1])
			x_strjoins(&ns, &len, " ", 1);*/
		i++;
	}
	ns[len] = '\0';
	(*t_ind)[len_ind] = -1;
	(*l_ind)[len_ind] = -1;
	return (ns);
}

char *find_home(char **env)
{
	int i;
	char **t;

	i = 0;
	while (env[i])
	{
			if (ft_strncmp(env[i], "HOME=", 5) == 0)
			{
					if (!(t = ft_strsplit(env[i], '=')))
						return (NULL);
					if (t[0] != NULL && t[1] != NULL)
						return (t[1]);
					else
						return (NULL);
			}
			i++;
	}
	return (NULL);
}

char *handle_tilde(char *s, char **env)
{
	char *home;
	int i;
	char **tmpx = fstrsplit(s, ft_strlen(s), char_is_whitespace); //DO A XFTSTRSPLIT
	char *tmp = NULL;
	char **ret;
	int find;

	if (!tmpx)
		return (s);
	if (!(home = find_home(env)))
		return (s);
	i = -1;
	while(tmpx[++i]);
	ret = xmalloc(sizeof(char *) * (i + 2));
	i = 0;
	find = 0;
	while(tmpx[i])
	{
			tmp = tilde_path(tmpx[i], home);
			if (tmp != NULL)
			{
				find = 1;
				ret[i] = tmp;
			}
			else
				ret[i] = tmpx[i];
			i++;
	}
	ret[i] = NULL;
	if (!find)
		return (s);
	char *ret_s;
	size_t len;
	ret_s = NULL;

	len = 0;
	i = -1;
	while(ret[++i])
		len += ft_strlen(ret[i]) + 1;
	ret_s = xmalloc(sizeof(char) * (len + 1));
	i = -1;
	len = 0;
	ret_s[0] = '\0';
	while (ret[++i])
	{
		x_strjoins(&ret_s, &len, ret[i], ft_strlen(ret[i]));
		x_strjoins(&ret_s, &len, " ", 1);
	}
	return (ret_s);
}

char *apply_var(char *s, int do_extra)
{
	int i;
	char *ns;
	char **tmp;
	size_t len;
	char **env;

	env = convert_env(g_env, l_env);
	if (do_extra == TRUE)
		s = handle_tilde(s, env);
	i = -1;
	len = 0;
	while (s[++i])
	{
		if (s[i] == '$' && (tmp = check_var(s + i + 1, env)) != NULL)
			len += ft_strlen(tmp[1]);
		else
			len += 1;
	}
	if (!(ns = xmalloc(sizeof(char) * (len + 1))))
		return (NULL);
	i = -1;
	len = 0;
	while (s[++i])
	{
		if (s[i] == '$' && (tmp = check_var(s + i + 1, env)) != NULL)
		{
				x_strjoins(&ns, &len, tmp[1], ft_strlen(tmp[1]));
				ns[len] = '\0';
				i += (ft_strlen(tmp[1]) > 0) ? ft_strlen(tmp[1]) - 1 : 1;
		}
		else
			ns[len++] = s[i];
	}
	ns[len] = '\0';
	return (ns);
}

//retourne une chaine de charactere parsé, globbé, variable env et separer par des espaces!
char *decortique_parse(char *expr, size_t l, int **t_ind, int **l_ind)
{
	char **ts;
	t_parse *p;
	int *marked_ind;
	/*
		int	*begin;
		int	*end;
		int	*type;
		int	nb;
		int	current;
		int	one_arg;
		int dec;
	*/
	marked_ind = xmalloc(sizeof(int) * (l + 1));
	ts = xmalloc(sizeof(char*) * (l + 1));
	ts[0] = NULL;
	p = parse_it2(expr, l, (t_delimiter *)&g_delimiter_quo, 3);
	int i = 0;
	while (i < p->nb)
	{
		if (p->type[i] == 2)
		{
				t_output o;
				o = shell_exec(cpy_a_to_b(expr, p->begin[i], p->end[i]));
				ts[i] = apply_var(o.string, FALSE);
		}
		else if (p->type[i] == 1)
				ts[i] = cpy_a_to_b(expr, p->begin[i], p->end[i]);
		else
				ts[i] = apply_var(cpy_a_to_b(expr, p->begin[i],p->end[i]), TRUE);
		if (p->type[i] == 0 || p->type[i] == 1)
				marked_ind[i] = 1;
		else
			marked_ind[i] = 0;
		i++;
	}
	ts[i] = NULL;
	return (join_string_array(ts, marked_ind, t_ind, l_ind));
}


typedef struct s_nparse
{
	int nb;
	int *type;
	int *begin;
	int *end;
}							t_nparse;

int is_intouchable(int i, int *t_ind, int *l_ind)
{
	int id_ind;

	id_ind = 0;
	while(t_ind[id_ind] != -1)
	{
		if (i >= t_ind[id_ind] && i <= t_ind[id_ind] + l_ind[id_ind])
			return (1);
		id_ind++;
	}
	return (0);
}

int is_connector(char *tested_s, int i, int *t_ind, int *l_ind)
{
	int id_tab;
	char *s;
	char *tab_connector[] = {";", "||", "&&", "|", "&", NULL};
	int num_tmp;
	int i_tmp;

	id_tab = 0;
	s = tested_s + i;
	if (!tested_s || !s || !s[0])
		return (0);
	if (is_intouchable(i, t_ind, l_ind))
		return (0);
	while (tab_connector[id_tab])
	{
		if (ft_strncmp(s, tab_connector[id_tab], (num_tmp = ft_strlen(tab_connector[id_tab]))) == 0)
		{
			//ft_dprintf(2 , "find a cnn\n");
			if (num_tmp > 2)
			{
				i_tmp = 0;
				while (i + (++i_tmp) < i + num_tmp)
					if (is_intouchable(i, t_ind, l_ind))
						return (0);
			}
			if (ft_strcmp("&", tab_connector[id_tab]) == 0
			&& i > 0 && !(is_intouchable(i-1, t_ind, l_ind)) && tested_s[i - 1] == '>')
				return (0);
			return (num_tmp);
		}
		id_tab += 1;
	}
	return (0);
}

void nparse_close(t_nparse *np, int id_close)
{
	np->end[np->nb] = id_close;
	//ft_dprintf(2 , "%d close bet %d - %d\n",np->nb,np->begin[np->nb], np->end[np->nb]);
	if (np->begin[np->nb] > np->end[np->nb])
	{
			dprintf(2, "PARSE ERROR\n");
			exit(0);
	}
}
void nparse_extend(t_nparse *np, int id_open)
{
	//ft_dprintf(2 , "nb __ %d  __ extend %d\n",np->nb, id_open);
	np->nb += 1;
	np->begin[np->nb] = id_open;
	np->end[np->nb] = -1;
	np->type[np->nb] = 0;
}

void nparse_init(t_nparse *np, char *expr)
{
	int len;

	len = ft_strlen(expr);
	np->type = xmalloc(sizeof(int) * (len + 1));
	np->begin = xmalloc(sizeof(int) * (len + 1));
	np->end = xmalloc(sizeof(int) * (len + 1));
	np->nb = -1;
}

void init_cmd(t_av **cmd, size_t len)
{
	(*cmd) = xmalloc(sizeof(t_av));
	(*cmd)->argv = xmalloc(sizeof(char *) * (len + 1));
	(*cmd)->argv_auth = xmalloc(sizeof(int *) * (len + 1));
	(*cmd)->argv_auth[0] = NULL;
	(*cmd)->argv[0] = NULL;
	(*cmd)->argc = 0;
	(*cmd)->bg = FALSE;
	(*cmd)->redirect = xmalloc(sizeof(t_redirect*) * (len + 1));
	(*cmd)->redirect[0] = NULL;
}

char *xget_string_l(char *s,int l)
{
	int i;
	char *ret;

	i = 0;
	ret = xmalloc(sizeof(char *) * (l + 1));
	while (i < l && s[i])
	{
		ret[i] = s[i];
		i++;
	}
	ret[i] = '\0';
	return (ret);
}

int get_type(char *s)
{
	if (ft_strncmp(s, ";", ft_strlen(";")) == 0)
		return (TYPE_NORMAL);
	if (ft_strncmp(s, "||", ft_strlen("||")) == 0)
			return (TYPE_OR);
	if (ft_strncmp(s, "&&", ft_strlen("&&")) == 0)
			return (TYPE_AND);
	if (ft_strncmp(s, "|", ft_strlen("|")) == 0)
			return (TYPE_PIPE);
	if (ft_strncmp(s, "&", ft_strlen("&")) == 0)
			return (-10);
	else {
		return (TYPE_NORMAL);
	}
}

int *handle_d(t_nparse np, int i, int *t_ind, int *l_ind, int len)
{
	int *x;
	int a;
	int id_x;

	x = xmalloc(sizeof(int *) * (len + 1));
	a = np.begin[i];
	id_x = 0;
	while (a <= np.end[i])
	{
			if (is_intouchable(a, t_ind, l_ind))
					x[id_x] = 0;
			else
					x[id_x] = 1;
			id_x++;
			a++;
	}
	return (x);
}

t_av **convert_parse(char *expr, t_nparse np, int *t_ind, int *l_ind)
{
	t_av **cmds;
	int id_cmds;
	int i;
	cmds = xmalloc(sizeof(t_av*) * (ft_strlen(expr) + 1));
	id_cmds = 0;
	init_cmd(&cmds[0], ft_strlen(expr));
	cmds[id_cmds]->type = TYPE_NORMAL;
	cmds[id_cmds + 1] = NULL;
	i = 0;
	while (i < np.nb)
	{
		if (np.type[i] > 0)
		{
				init_cmd(&cmds[++id_cmds], ft_strlen(expr));
				cmds[id_cmds]->type = get_type(xget_string_l(expr + np.begin[i], np.end[i] - np.begin[i]));
				if (cmds[id_cmds]->type == -10)
				{
					if (id_cmds > 0)
							cmds[id_cmds-1]->bg = TRUE;
					else
							dprintf(2, "21sh: parse error\n");
					cmds[id_cmds]->type = TYPE_NORMAL;
				}
				cmds[id_cmds + 1] = NULL;
		}
		else
		{
				char *x = xget_string_l(expr + np.begin[i], np.end[i] - np.begin[i]);
				cmds[id_cmds]->argv[cmds[id_cmds]->argc] = x;
				cmds[id_cmds]->argv_auth[cmds[id_cmds]->argc] = handle_d(np, i, t_ind, l_ind, ft_strlen(expr));
				cmds[id_cmds]->argc++;
				cmds[id_cmds]->argv_auth[cmds[id_cmds]->argc] = NULL;
				cmds[id_cmds]->argv[cmds[id_cmds]->argc] = NULL;
		}
		i++;
	}
	cmds[id_cmds + 1] = NULL;
/*	for (int x = 0; cmds[x]; x++)
	{
		for (int j = 0; cmds[x]->argv[j];j++)
			ft_dprintf(2, "%%{red}%s{eoc}%%\n", cmds[x]->argv[j]);
	}
*/
	return (cmds);
}

int is_whitespace(char c)
{
	if (c == ' ' || c == '\n')
		return (1);
	return (0);
}

t_nparse parse(char *expr, int *t_ind, int *l_ind)
{
	t_nparse np;
	int i;
	int i32_tmp;

	nparse_init(&np, expr);
	i = 0;
	//ft_dprintf(2 , "begin parse\n");
	while(expr[i] && is_whitespace(expr[i]) && !is_intouchable(i, t_ind, l_ind))
		i++;
	nparse_extend(&np, i);
	while (expr[i] != '\0')
	{
		if (is_intouchable(i, t_ind, l_ind))
		{
			//ft_dprintf(2 , "charactere at pos %d is in\n", i);
				while(expr[i] && is_intouchable(i, t_ind, l_ind))
					i++;
				i--;
		}
		else if (is_whitespace(expr[i]))
		{
				//ft_dprintf(2 , "charactere at pos %d is ws\n", i);
				nparse_close(&np, i);
				int id_old = i;
				while(expr[i] && is_whitespace(expr[i]) && !is_intouchable(i, t_ind, l_ind))
					i++;
				//ft_dprintf(2 , "ended char at pos %d is ws\n", i);
				if (!is_connector(expr, i, t_ind, l_ind))
					nparse_extend(&np, i);
				i--;
		}
		else if ((i32_tmp = is_connector(expr, i, t_ind,l_ind)))
		{
			//ft_dprintf(2 , "charactere at pos %d is cnn\n", i);
			nparse_close(&np, (is_intouchable(i-1, t_ind, l_ind) || !is_whitespace(expr[i-1])) ? i : i-1);
			nparse_extend(&np, i);
			np.type[np.nb] = 2;
			nparse_close(&np, i + i32_tmp-1);
			if (is_intouchable(i + i32_tmp, t_ind, l_ind) || !is_whitespace(expr[i + i32_tmp]))
				nparse_extend(&np, i + i32_tmp);
			i += i32_tmp - 1;
		}
		i++;
	}
	nparse_close(&np, i);
	np.nb++;
	if (i > 0 && !is_intouchable(i-1, t_ind, l_ind) && is_whitespace(expr[i - 1]))
		np.nb--;
	return (np);
}

int in_cmd_is_intouchable(t_av *cmd, int id_argv, int i)
{
	if (cmd->argv_auth[id_argv][i] == 1)
		return (0);
	return (1);
}
char *is_redir_right(char *s)
{
	int i;
	int tmp;
	char *redirs[] = {">>", ">", NULL};

	i = 0;
	while (redirs[i])
	{
		if (ft_strncmp(s, redirs[i], ft_strlen(redirs[i])) == 0)
			return (redirs[i]);
		i++;
	}
	return (NULL);
}

void delete_c(t_av **cmd, int id_argv, int i, int *pi)
{
	int a;

	a = 0;
	while ((*cmd)->argv[id_argv][i + a])
	{
		(*cmd)->argv[id_argv][i + a] = (*cmd)->argv[id_argv][i + a + 1];
		(*cmd)->argv_auth[id_argv][i + a] = (*cmd)->argv_auth[id_argv][i + a + 1];
		a++;
	}
	(*pi) -= 1;
	//TO DO HANDLE -1 ARGV
}

void delete_s(t_av **cmd, int id_argv)
{
	int a;

	a = id_argv;
	while((*cmd)->argv[a])
	{
		(*cmd)->argv[a] = (*cmd)->argv[a + 1];
		(*cmd)->argv_auth[a] = (*cmd)->argv_auth[a + 1];
		a++;
	}
	(*cmd)->argc--;
}

int set_redir_right(t_av **pcmd, int id_argv, int i, char *r)
{
	char *s;
	t_av *cmd;
	int		findfile;
	t_redirect *redir;
	int len;

	cmd = (*pcmd);
	redir = xmalloc(sizeof(t_redirect));
	redir->fd_in = 1;
	redir->fd_out = -1;
	redir->s_in = NULL;
	redir->len_in = -1;
	redir->type = 0;
	if (i > 0 && !in_cmd_is_intouchable(cmd, id_argv, i - 1)
	&& ft_isdigit(cmd->argv[id_argv][i - 1]))
	{
		redir->fd_in = cmd->argv[id_argv][i-1] - 0x30;
		delete_c(pcmd, id_argv, i - 1, &i);
	}
	len = ft_strlen(r);
	//NOW DELETING THE > or >>
	int b;
	b = -1;
	while ((++b) < (int)ft_strlen(r))
		delete_c(pcmd, id_argv, i + b, &i);
	if (!cmd->argv[id_argv])
		return (FALSE);
	findfile = TRUE;
	if (!in_cmd_is_intouchable(cmd, id_argv, i + len) && cmd->argv[id_argv][i + len] == '&')
	{
		findfile = FALSE;
		delete_c(pcmd, id_argv, i + len, &i);
	}
	if (cmd->argv[id_argv][i + len] == '\0')
	{
		i = 0;
		len = 0;
		if (ft_strlen(cmd->argv[id_argv]) == 0)
		{
			delete_s(pcmd, id_argv);
			id_argv--;
		}
		id_argv++;
		//dprintf(2, "IIIIINNN ... |%s|\n", cmd->argv[id_argv]);
	}
	if (cmd->argv[id_argv] == NULL)
	{
		print_err("parse error with the redirection", "Parsing");
		return (FALSE);
	}
	int x;
	s = xmalloc(sizeof(char *) * (ft_strlen(cmd->argv[id_argv]) + 1));
	x = 0;
	//dprintf(2, "file is %%");
	while (cmd->argv[id_argv][i + len])
	{
			s[x] = cmd->argv[id_argv][i + len];
			//dprintf(2, "%c", s[x]);
			if (!in_cmd_is_intouchable(cmd,id_argv,i) &&
						is_redir_right(cmd->argv[id_argv] + i + len))
						break;
			delete_c(pcmd, id_argv, i + len, &i);
			i++;
			x++;
	}
	//dprintf(2, "%%\n");
	s[x] = '\0';
	if (ft_strlen(cmd->argv[id_argv]) == 0)
	{
		delete_s(pcmd, id_argv);
		id_argv--;
	}
	if (findfile == TRUE)
	{
		redir->path = s;
		redir->open_flag = ((ft_strcmp(r, ">>") == 0) ? O_CREAT | O_RDWR | O_APPEND : O_CREAT | O_RDWR | O_TRUNC);
		(g_debug) ? dprintf(2, "CREATE: %s -> %d\n", s, redir->fd_out) : 0;
	}

	else
	{
			//NOT HANDLE
	}
	//ADDING IN OUR CMD;
	i = 0;
	while (cmd->redirect[i])
		i++;
	cmd->redirect[i] = redir;
	cmd->redirect[i + 1] = NULL;
	return (TRUE);
}

int get_redirect(t_av **cmd)
{
	int id_argv;
	char *s;
	int i;
	char *tmp;
	t_redirect **r;

	id_argv = 0;
	while((*cmd)->argv[id_argv] != NULL)
	{
		s = (*cmd)->argv[id_argv];
		i = -1;
		while(s[++i])
		{
			if (in_cmd_is_intouchable((*cmd), id_argv, i))
					continue;
			if ((tmp = is_redir_right(s + i)))
			{
				if (!set_redir_right(cmd, id_argv, i, tmp))
					return (FALSE);
				break;
			}
	}
		id_argv++;
	}
	return (TRUE);
}

void convert_other(t_av ***cmds)
{
	int i;

	i = 0;
	while((*cmds)[i])
	{
		get_redirect(&(*cmds)[i]);
		i++;
	}
}

t_av **nparse(char *expr, int *t_ind, int *l_ind)
{
	t_av **cmds;
	t_nparse np;

	np = parse(expr, t_ind, l_ind);
	cmds = convert_parse(expr, np, t_ind, l_ind);
	convert_other(&cmds);
	return (cmds);
}


t_av	**parse_commands(char *expr)
{
	int			*ti;
	int			*waiting_type;
	int			*t_ind;
	int			*l_ind;
	t_av **cmds;
	int id_cmd;
	int id_p;
	int id_ind;
	int id_exp;

	(g_debug) ? ft_dprintf(2, "{yellow}----------------------{eoc}\n") : 0;
	if (!(cmds = xmalloc(sizeof(t_av **) * (ft_strlen(expr) + 1))))
		return (NULL);
	cmds[(id_cmd = 0)] = NULL;
	expr = decortique_parse(expr, ft_strlen(expr), &t_ind, &l_ind);
	if (g_debug)
	{
		ft_dprintf(2, "DEBUG: FIRST PARSE: \n%%");
	for (int i = 0; expr[i];i++)
	{
		if (is_intouchable(i, t_ind, l_ind))
			ft_dprintf(2, "{red}%c{eoc}", expr[i]);
		else
			ft_dprintf(2, "%c", expr[i]);
	}
	ft_dprintf(2, "%%\n");
	}
	cmds = nparse(expr, t_ind, l_ind);
	id_p = -1;
	int color = 0;
	id_exp = 0;
	return (updated(cmds));
}
