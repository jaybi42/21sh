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


#define NUMBER_DELIMITER 22
#define NUMBER_REDIRECT 5
#define TRUE 1
#define FALSE 0


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
	{"||", "cmdor", 1, 0, 1, 1, 0, 0},
	{";", "pvrig", 1, 0, 1, 1, 0, 0},
	{"&&", "cmdand", 1, 0, 1, 1, 0, 0},
	{"|", "pipe", 1, 0, 1, 1, 0, 0},
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
	{"<", "<", 0, 0, 0, 1, 1, 1},
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

void		handle_delimiter(char *expr, int *i, t_parse *p, int *t_ind, int *l_ind)
{
	int a;

	a = 0;
	while (t_ind && l_ind && t_ind[a] != -1)
	{
		if ((*i) == t_ind[a])
		{
			//we find the begining of ""
			//printf("find %d, |%.*s|\n",l_ind[a], l_ind[a], expr+(*i));
			p->quote_activate = TRUE;
			(*i) += l_ind[a] + 1;
			return ;
		}
		a++;
	}
	a=0;
	while (a < NUMBER_DELIMITER) //check for every delimiters if no error
	{
		if (ft_strncmp(expr + (*i), g_delimiter[a].name,
					ft_strlen(g_delimiter[a].name)) == 0)
		{
			if (p->current != EMPTY && g_delimiter[p->current].wait_another == TRUE
					&& p->current == a)
			{
				//that's mean we find for example the second " of ""
				p->end[p->nb] = (*i);
				reset_current(p, (*i));
			}
			else if (p->current != EMPTY && g_delimiter[p->current].do_recursivity == TRUE
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
	if (p->current != EMPTY && g_delimiter[p->current].is_redirection == TRUE
			&& p->one_arg == FALSE
			&& char_is_whitespace(expr[(*i)]) == FALSE)
		p->one_arg = TRUE;
	else if (p->current != EMPTY && g_delimiter[p->current].is_redirection == TRUE
			&& p->one_arg == TRUE 		&& char_is_whitespace(expr[(*i)]) == TRUE)
	{
		p->end[p->nb] = (*i);
		reset_current(p, (*i));
	}
	(*i) += 1;
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

t_parse *parse_it(char *expr, int len, int *t_ind, int *l_ind)
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
	p->quote_activate = FALSE;
	while (i < len && expr[i] != '\0')
		handle_delimiter(expr, &i, p, t_ind, l_ind);
	if (p->current != EMPTY && g_delimiter[p->current].wait_another == TRUE)
		ft_printf("[!] we were waiting another |%s| adding one for u\n", g_delimiter[p->current].name);
	if (p->current != EMPTY && g_delimiter[p->current].is_redirection == TRUE && p->one_arg == FALSE && p->quote_activate == FALSE)
	{
		ft_dprintf(2, "%s: Missing name for redirect\n", NAME);
		return (NULL);
	}
	p->end[p->nb] = i;
	p->nb++;
	return (p);
}

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

/*
   char	*clear_whitespace(char *s)
   {
   int i;
   int a;

   i = 0;
   a = 0;
   while (s[i])
   {
   if (!char_is_whitespace(s[i]))
   {
   s[a] = s[i];
   a++;
   }
   i++;
   }
   s[a] = '\0';
   return (s);
   }
 */
void print_redirect(t_redirect *redirect)
{
	//no output
	return;
	if (redirect == NULL)
	{
		printf("redirection: NULL\n");
		return ;
	}
	if (redirect->type == 1)
	{
		printf("redirection: type '<' --> |%.*s...|\n", 4, redirect->s_in);
	}
	else
	{
		printf("redirection: type '>' --> %d to %d\n", redirect->fd_in, redirect->fd_out);
	}
}
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

char **special_split(char *expr, int dec, int begin, int end, int *t_ind, int *l_ind)
{
	char ***s;

	//int begin = tp[pa]->begin[ti[pa] + a + ra];
	//int end = tp[pa]->end[ti[pa] + a + ra];
	s = xmalloc(sizeof(char **) * (ft_strlen(expr)+1));
	s[0] = NULL;
	int s_a = 0;
	int x;
	int find;
	x = begin;
	int tmp;
	int last_met = x;
	while (x < end)
	{
		find = -1;
		tmp = (x - last_met - 1);
		if (tmp < 0)
			tmp = 0;
		for (int i =0; t_ind[i] != -1; i++)
		{
			if (t_ind[i] == x + dec)
			{
				find = i;
				break;
			}
		}
		if (find != -1)
		{
			s[s_a++] = fstrsplit(expr + dec + last_met, tmp, char_is_whitespace);
			s[s_a++] = tab_from_string(cpy_a_to_b(expr, t_ind[find], t_ind[find] + l_ind[find]));
			s[s_a] = NULL;
			last_met = x + l_ind[find] + 1;
			x += l_ind[find];
		}
		x++;
	}
	s[s_a++] = fstrsplit(expr + dec + last_met, x - last_met, char_is_whitespace);
	s[s_a] = NULL;
	return (join_array(s));
}


t_redirect *get_redirection(char *expr, int dec, int begin, int end, int *t_ind, int *l_ind)
{
	int i;
	t_redirect *redirect;
	char *s;

	(void)t_ind;
	(void)l_ind;
	s = cpy_a_to_b(expr + dec, begin, end);

	//s = clear_whitespace(s);
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
	if (redirect->type == 0) //&& ft_strncmp(s + i, "&", 1) == 0)
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
		//we get the file
		char **t = fstrsplit(s + i, ft_strlen(s + i ), char_is_whitespace);
		if (t == NULL || t[0] == NULL || is_delimiter(t[0]))
			return (NULL);
		redirect->fd_out = open(t[0], open_flag, 0666);
		if (redirect->fd_out == -1)
			return (NULL);
	return (redirect);
}

void print_command(t_av *cmd)
{
	int i = 0;
	int iarg = 0;

	ft_printf("args: %d\n", cmd->argc);
	while (i < cmd->argc)
	{
		ft_printf("|%s|\n", cmd->argv[i]);
		if (cmd->argv[i] == NULL)
			ft_printf("ho, this pointing to new arg: %#x\n", cmd->argcmd[iarg++]);
		i++;
	}
}
//return null if error
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

t_av **updated(t_av **av)
{
	int i;

	i = 0;
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
		i++;
	}
	return (av);
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
			(*t_ind)[len_ind] = len;
			(*l_ind)[len_ind++] = ft_strlen(a[i]);
		}
		x_strjoins(&ns, &len, a[i], ft_strlen(a[i]));
		if (a[i + 1])
			x_strjoins(&ns, &len, " ", 1);
		i++;
	}
	ns[len] = '\0';
	(*t_ind)[len_ind] = -1;
	(*l_ind)[len_ind] = -1;
	return (ns);
}

char *apply_var(char *s, int do_extra)
{
	int i;
	char *ns;
	char **tmp;
	size_t len;
	char **env;
	int find_tilde;

	find_tilde = 1; //to_modify
	env = convert_env(g_env, l_env);
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
				x_strjoins(&ns,&len, tmp[1],ft_strlen(tmp[1]));
				i += ft_strlen(tmp[1]);
		}
		else if (do_extra && !find_tilde)
		{

		}
		else
		{
			ns[len++] = s[i];
			find_tilde = 1;
		}
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
	p = parse_it2(expr, l,(t_delimiter *)&g_delimiter_quo, 3);
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
				ts[i] = cpy_a_to_b(expr, p->begin[i],p->end[i]);
		else
		{
				ts[i] = apply_var(cpy_a_to_b(expr, p->begin[i],p->end[i]), TRUE);
				//ft_dprintf(2, "%s\n", ts[i]);
				//char **d = ret_match(ts[i]);
				//if (d != NULL) {for (int x = 0; d[x];x++)ft_dprintf(2, " %d |%s|\n", x, d[x]); }
		}
		if (p->type[i] == 0 || p->type[i] == 1)
				marked_ind[i] = 1;
		else
			marked_ind[i] = 0;
		i++;
	}
	ts[i] = NULL;
	return (join_string_array(ts, marked_ind, t_ind, l_ind));
}

t_av	**parse_commands(char *expr)
{
	t_parse		**tp;
	int			*ti;
	int			*waiting_type;
	int			pa;
	int			*t_ind;
	int			*l_ind;

	if (!(tp = xmalloc(sizeof(t_parse **) * (ft_strlen(expr) + 1)))
	|| (!(ti = xmalloc(sizeof(int *) * (ft_strlen(expr) + 1)))))
		exit(1);
	if (!(waiting_type = xmalloc(sizeof(int) * (ft_strlen(expr) + 1))))
		exit(1);
	waiting_type[0] = 0;
	pa = 0;
	ti[pa] = 0;
	//dprintf(2, "before: |%s|\n", expr);
	expr = decortique_parse(expr, ft_strlen(expr), &t_ind, &l_ind);
	//dprintf(2, "after: |%s|\n---\n", expr);
	if (!(tp[pa] = parse_it(expr, ft_strlen(expr), t_ind, l_ind)))
		return (NULL);
	tp[pa]->dec = 0;
	tp[pa + 1] = NULL;
	ti[pa + 1] = EMPTY;
	t_av **cmds;
	pa = -1;
if (!(cmds = xmalloc(sizeof(t_av **) * (ft_strlen(expr) + 1))))
return (NULL);
/*int x_tmp = 0;
//printf("in |%s|\n", expr);
while(x_tmp < tp[pa]->nb)
{
	printf("|%.*s|(%d_%d) - %d\n", tp[pa]->end[x_tmp], expr + tp[pa]->begin[x_tmp], tp[pa]->begin[x_tmp], tp[pa]->end[x_tmp], tp[pa]->type[x_tmp]);
	x_tmp++;
}*/
int ic = 0;
//int skip_it = FALSE;
int oldi = 0;
pa = 0;
while (tp[pa] != NULL)
{
	//for each parsed params:
	while (ti[pa] < tp[pa]->nb)
	{
			oldi = ti[pa];
			while (ti[pa] + oldi < tp[pa]->nb && (tp[pa]->type[ti[pa] + oldi] == EMPTY ||
			g_delimiter[tp[pa]->type[ti[pa] + oldi]].is_arg == TRUE))
			{
				oldi++;
			}
			if (oldi != ti[pa])
			{
				//creating a command
				int ra;
				int a;
				if (!(cmds[ic] = xmalloc(sizeof(t_av)))
				|| !(cmds[ic]->redirect = xmalloc(sizeof(t_redirect *) * (ft_strlen(expr) + 1)))
				|| !(cmds[ic]->bitcode = xmalloc(sizeof(void *) * (ft_strlen(expr) + 1)))
				|| !(cmds[ic]->argcmd = xmalloc(sizeof(t_av **) * (ft_strlen(expr) + 1))))
						return (NULL);
				ra = 0;
				int argcmd_i = 0;
				cmds[ic]->bitcode[ra] = 0;
				cmds[ic]->type = tp[pa]->type[ti[pa]];
				cmds[ic]->redirect[ra] = NULL;
				char ***t_tstr;

				if (!(t_tstr = xmalloc(sizeof(char ***) * (oldi - ti[pa] + 1))))
					return (NULL);
				a = 0;
				while (ti[pa] + a + ra < oldi)
				{
						//if it's a redirection stuff!!
					if (tp[pa]->type[ti[pa] + a + ra] >= 0 && g_delimiter[tp[pa]->type[ti[pa] + a + ra]].is_redirection)
					{
						//redirect_stuff
						if (!(cmds[ic]->redirect[ra] = get_redirection(
						expr, tp[pa]->dec,
						tp[pa]->begin[ti[pa] + a + ra],
						tp[pa]->end[ti[pa] + a + ra], t_ind,l_ind)))
							return (NULL);
						cmds[ic]->bitcode[ra + a] = 0;
						ra++;
					}
					else
					{
						t_tstr[a] = special_split(expr, tp[pa]->dec, tp[pa]->begin[ti[pa] + a + ra],tp[pa]->end[ti[pa] + a + ra], t_ind, l_ind);//fstrsplit((expr + tp[pa]->dec) + tp[pa]->begin[ti[pa] + a + ra], tp[pa]->end[ti[pa] + a + ra] - tp[pa]->begin[ti[pa] + a + ra], char_is_whitespace);
						//for (int j = 0; t_tstr[a][j];j++)ft_dprintf(2, "_|%s|_\n", t_tstr[a][j]);
						if (tp[pa]->type[ti[pa] + a + ra] != EMPTY && g_delimiter[tp[pa]->type[ti[pa] + a + ra]].dont_give_shit_about_whitespace == 0)
							cmds[ic]->bitcode[ra + a] = 0;
						else
							cmds[ic]->bitcode[ra + a] = 1;
						a++;
					}
				}
				ti[pa] += a + ra;
				t_tstr[a] = NULL;
				int len = t_len(t_tstr, a);
				if ((cmds[ic]->argv = xmalloc(sizeof(char *) * (len + 1))) == NULL)
				return (NULL);
				int ia;
				int i = -1;
				int argv_i;
				argv_i = 0;
				while (++i < a)
				{
					if (t_tstr[i] == NULL)
						cmds[ic]->argv[argv_i++] = NULL;
					else
					{
						ia = -1;
						while (t_tstr[i][++ia] != NULL)
								cmds[ic]->argv[argv_i++] = t_tstr[i][ia];
					}
				}
				cmds[ic]->argv[argv_i] = NULL;
				cmds[ic]->argcmd[argcmd_i] = NULL;
				cmds[ic]->argc = argv_i;
				cmds[ic]->redirect[ra] = NULL;
				if (waiting_type[0] > 0)
						cmds[ic]->type = waiting_type[waiting_type[0]--];
				else
					cmds[ic]->type = TYPE_OTHER;
				ic++;
			}
			else
			{
				waiting_type[0]++;
				waiting_type[waiting_type[0]] = tp[pa]->type[ti[pa]];
				int x = 0;
				int a = 0;
				while(t_ind && l_ind && t_ind[x] != -1)
				{
					int b = tp[pa]->dec + tp[pa]->begin[ti[pa]];
					if (t_ind[x] > b)
					{
						t_ind[a] = t_ind[x] - b;
						l_ind[a++] = l_ind[x];
					}
					x++;
				}
				t_ind[a] = -1;
				l_ind[a] = -1;
				if (!(tp[pa + 1] = parse_it((expr + tp[pa]->dec) + tp[pa]->begin[ti[pa]],
				tp[pa]->end[ti[pa]] - tp[pa]->begin[ti[pa]], t_ind, l_ind)))
					return (NULL);
				tp[pa + 1]->dec = tp[pa]->dec + tp[pa]->begin[ti[pa]];
				ti[pa]++;
				pa++;
				ti[pa] = 0;
				tp[pa + 1] = NULL;
				ti[pa + 1] = EMPTY;
			}
		}
		if (tp[pa + 1] == NULL)
		{
			pa = -1;
			while (tp[++pa] != NULL)
				if (ti[pa] != tp[pa]->nb)
					break;
			if (tp[pa] == NULL)
				break;
		}
		else
			pa++;
	}
	cmds[ic] = NULL;
	int f = 0;
	while (cmds[f])
	{
	//	printf("printing for %d\n",f);
		int dd = 0;
		//printf("type: %d\n", cmds[f]->type);
		while (cmds[f]->redirect[dd])
		{
			print_redirect(cmds[f]->redirect[dd]);
			dd++;
		}
		f++;
	}
	return (updated(cmds));
}
