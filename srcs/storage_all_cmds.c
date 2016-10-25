/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   storage_all_cmds.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouchla <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/28 22:16:21 by ibouchla          #+#    #+#             */
/*   Updated: 2016/10/23 15:00:53 by agadhgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"
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
};

static t_delimiter const	g_delimiter_quo[3] =
{
	{"\"", "dquote", 1, 1, 0, 0, 0, 1},
	{"\'", "quote", 1, 1, 0, 0, 0, 1},
	{"`", "bquote", 1, 1, 1, 1, 0, 1}
};

static t_delimiter const	g_delimiter_sep[5] =
{
	{"\\", "", 1, 0, 0, 0, 0, 1},
	{"||", "cmdor", 1, 0, 1, 1, 0, 0},
	{";", "pvrig", 1, 0, 1, 1, 0, 0},
	{"&&", "cmdand", 1, 0, 1, 1, 0, 0},
	{"|", "pipe", 1, 0, 1, 1, 0, 0}
};

typedef struct	s_parse
{
	int	*begin;
	int	*end;
	int	*type;
	int	nb;
	int	current;
	int	one_arg;
	int dec;
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

void reset_current(t_parse *p, int index)
{
	p->nb++;
	p->current = EMPTY;
	p->begin[p->nb] = index + 1;
	p->end[p->nb] = 0;
	p->type[p->nb] = EMPTY;
	p->one_arg = FALSE;
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
}

void		handle_delimiter(char *expr, int *i, t_parse *p)
{
	int a;

	a = 0;
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
	if (!(new_str = malloc(sizeof(char) * (len + 1))))
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

	if (!(t_str = malloc(sizeof(char **) * (len + 1))))
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

	if (!(p = malloc(sizeof(t_parse))) ||
			!(p->begin = malloc(sizeof(int) * ft_strlen(expr))) ||
			!(p->end = malloc(sizeof(int) * ft_strlen(expr))) ||
			!(p->type = malloc(sizeof(int) * ft_strlen(expr))))
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
	if (p->current != EMPTY && d[p->current].is_redirection == TRUE && p->one_arg == FALSE)
		ft_printf("[!] we were waiting another stuff to redirect! outrepassing\n");
	p->end[p->nb] = i;
	p->nb++;
	return (p);
}

t_parse *parse_it(char *expr, int len)
{
	int i;
	t_parse *p;
	//printf("[%.*s] - connector: %s\n", sh->nb_stack, "++++++++++++++++++++++++++++++++", sh->connector == NULL ? "none" : sh->connector);
	if (!(p = malloc(sizeof(t_parse))) ||
			!(p->begin = malloc(sizeof(int) * ft_strlen(expr))) ||
			!(p->end = malloc(sizeof(int) * ft_strlen(expr))) ||
			!(p->type = malloc(sizeof(int) * ft_strlen(expr))))
		exit(0);
	i = 0;
	p->nb = 0;
	p->current = -1;
	p->begin[0] = 0;
	p->type[0] = -1;
	while (i < len && expr[i] != '\0')
		handle_delimiter(expr, &i, p);
	if (p->current != EMPTY && g_delimiter[p->current].wait_another == TRUE)
		ft_printf("[!] we were waiting another |%s| adding one for u\n", g_delimiter[p->current].name);
	if (p->current != EMPTY && g_delimiter[p->current].is_redirection == TRUE && p->one_arg == FALSE)
		ft_printf("[!] we were waiting another stuff to redirect! outrepassing\n");
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

t_redirect *get_redirection(char *s)
{
	int i;
	t_redirect *redirect;

	//s = clear_whitespace(s);
	if (!(redirect = malloc(sizeof(t_redirect))) || s == NULL)
		return (NULL);
	i = 0;
	int len;
	int open_flag;
	redirect->fd_in = 1;
	redirect->type = 1;
	open_flag = O_CREAT | O_RDWR;
	len = 0;
	if (ft_isdigit(s[i])) {
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
		i += 1;
	}
	else if (ft_strncmp(s + i, "<<", 2) == 0)
	{
		ft_printf("parsing info: didn't handle yet <<\n");
		return (NULL);
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
		if (t == NULL && t[0] != NULL)
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

char **check_var(char *s)
{
	char **env;
	int i;
	char **tmp;

	env = convert_env(g_env, l_env);
	i = 0;
	while (env[i])
	{
		tmp = ft_strsplit(env[i], '=');
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
char *join_string_array(char **a)
{
	int i;
	size_t len;
	char *ns;

	len = 0;
	i = -1;
	while (a[++i])
		len += ft_strlen(a[i]) + 1;
	ns = malloc(sizeof(char) * (len + 1));
	i = 0;
	len = 0;
	while (a[i])
	{
		x_strjoins(&ns, &len, a[i], ft_strlen(a[i]));
		x_strjoins(&ns, &len, " ", 1);
		i++;
	}
	ns[len] = '\0';
	return (ns);
}

char *apply_var(char *s)
{
	int i;
	char *ns;
	char **tmp;
	int ret;
	int len;

	i = -1;
	len = 0;
	while (s[++i])
	{
		if (s[i] == '$' && (tmp = check_var(s + i + 1)) != NULL)
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
		if (s[i] == '$' && (tmp = check_var(s + i + 1)) != NULL)
		{
				x_strjoins(&ns,(size_t *)&len,tmp[1],ft_strlen(tmp[1]));
				i += ft_strlen(tmp[1]);
		}
		else
			ns[len++] = s[i];
	}
	ns[len] = '\0';
	return (ns);
}

//retourne une chaine de charactere parsé, globbé, variable env et separer par des espaces!
char *decortique_parse(char *expr, size_t l)
{
	char **ts;
	char *s;
	t_parse *p;
	/*
		int	*begin;
		int	*end;
		int	*type;
		int	nb;
		int	current;
		int	one_arg;
		int dec;
	*/
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
				ts[i] = apply_var(o.string);
		}
		else if (p->type[i] == 1)
				ts[i] = cpy_a_to_b(expr, p->begin[i],p->end[i]);
		else
				ts[i] = apply_var(cpy_a_to_b(expr, p->begin[i],p->end[i]));
		i++;
	}
	ts[i] = NULL;
//	for (int j = 0;ts[j];j++){dprintf(2, "%d:[%s]\n",j, ts[j]);}
	//printf("ended..\n");
	return (join_string_array(ts));
}


t_av	**parse_commands(char *expr)
{
//	int			tmp;
	t_parse		**tp;
	int			*ti;
	int			*waiting_type;
//	int			current;
	int			pa;

	if (!(tp = malloc(sizeof(t_parse **) * (ft_strlen(expr) + 1)))
	|| (!(ti = malloc(sizeof(int *) * (ft_strlen(expr) + 1)))))
		exit(1);
	if (!(waiting_type = malloc(sizeof(int) * (ft_strlen(expr) + 1))))
		exit(1);
	waiting_type[0] = 0;
	pa = 0;
	ti[pa] = 0;
	//dprintf(2, "before: |%s|\n", expr);
	expr = decortique_parse(expr, ft_strlen(expr));
	//dprintf(2, "after: |%s|\n---\n", expr);
	tp[pa] = parse_it(expr, ft_strlen(expr));
	tp[pa]->dec = 0;
	tp[pa + 1] = NULL;
	ti[pa + 1] = EMPTY;

	t_av **cmds;

if (!(cmds = malloc(sizeof(t_av **) * (ft_strlen(expr) + 1))))
return (NULL);
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
				if (!(cmds[ic] = malloc(sizeof(t_av)))
				|| !(cmds[ic]->redirect = malloc(sizeof(t_redirect *) * (ft_strlen(expr) + 1)))
				|| !(cmds[ic]->bitcode = malloc(sizeof(void *) * (ft_strlen(expr) + 1)))
				|| !(cmds[ic]->argcmd = malloc(sizeof(t_av **) * (ft_strlen(expr) + 1))))
						return (NULL);
				ra = 0;
				int argcmd_i = 0;
				cmds[ic]->bitcode[ra] = 0;
				cmds[ic]->type = tp[pa]->type[ti[pa]];
				cmds[ic]->redirect[ra] = NULL;
				//its for separate each command
				char ***t_tstr;

				if (!(t_tstr = malloc(sizeof(char ***) * (oldi - ti[pa] + 1))))
					return (NULL);
				a = 0;
				while (ti[pa] + a + ra < oldi)
				{
						//if it's a redirection stuff!!
					if (tp[pa]->type[ti[pa] + a + ra] != EMPTY && g_delimiter[tp[pa]->type[ti[pa] + a + ra]].is_redirection)
					{
						//redirect_stuff
						cmds[ic]->redirect[ra] = get_redirection(
						cpy_a_to_b((expr + tp[pa]->dec),
						tp[pa]->begin[ti[pa] + a + ra],
						tp[pa]->end[ti[pa] + a + ra]));
						cmds[ic]->bitcode[ra + a] = 0;
						ra++;
					}
					else
					{
						t_tstr[a] = fstrsplit((expr + tp[pa]->dec) + tp[pa]->begin[ti[pa] + a + ra], tp[pa]->end[ti[pa] + a + ra] - tp[pa]->begin[ti[pa] + a + ra], char_is_whitespace);
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
				if ((cmds[ic]->argv = malloc(sizeof(char *) * (len + 1))) == NULL)
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
				tp[pa + 1] = parse_it((expr + tp[pa]->dec) + tp[pa]->begin[ti[pa]],
				tp[pa]->end[ti[pa]] - tp[pa]->begin[ti[pa]]);
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
