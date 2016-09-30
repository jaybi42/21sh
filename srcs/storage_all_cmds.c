/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   storage_all_cmds.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouchla <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/28 22:16:21 by ibouchla          #+#    #+#             */
/*   Updated: 2016/09/30 19:45:59 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"
#include <stdlib.h>

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

typedef struct s_redirector
{
	int *def;
	int name;
	int special;
	int open_waiter;
	int is_file;
	int fd_in;
	int fd_out;
}							t_redirector;

static t_redirector const g_delimiter[] =
{
{">>", "",1, 1, 0, 1, -1},
{">&","", 0, 0, 0, 1, -1},
{">", "", 0, 0, 0, 1, 1},
{"1>","", 0, 0, 0,  1, 1},
{"<<","heredoc", 1, 1, 0, -1, 0},
{"<", "", 1, 0, 1, -1, 0},
};
*/
typedef struct s_parse
{
	int	*begin;
	int	*end;
	int	*type;
	int	nb;
	int	current;
	int	one_arg;
	int dec;
}		t_parse;

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
						//that's mean we find for example another |
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

t_redirect *get_redirection(char *s)
{
	t_redirect *redirect;
	int i;

	if (!(redirect = malloc(sizeof(t_redirect))) || s == NULL)
		return (NULL);
	i = 0;
//	int fd;
	int len;
	int fd_in = 1;
	int op;
	char **decontenanceur;
	op = -1;
	len = 0;
	#define DBF 2
	#define F 1
	#define FR 3 //<
	#define DBFR 4
	if (ft_isdigit(s[0]))
	{
		len++;
		fd_in = s[0];
	}
	if (ft_strncmp(s + len, ">>", 2) == 0)
	{
		op = DBF;
		len += 2;
	}
	else if (ft_strncmp(s + len, ">", 1) == 0)
	{
		op = F;
		len += 1;
	}
	else if (ft_strncmp(s + len, "<", 1) == 0)
	{
		op = FR;
		len += 1;
	}
	else if (ft_strncmp(s + len, "<<", 2) == 0)
	{
		op = FR;
		len += 1;
	}

	if (ft_strncmp(s, ">>", 2) == 0)
	{
		decontenanceur = fstrsplit(s + 2, ft_strlen(s + 2), char_is_whitespace);
	}
	ft_memdel((void **)&s);
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
				av[i]->arg =copy_array_begin(1, av[i]->argv);
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

t_av	**parse_commands(char *expr)
{
//	int			tmp;
	t_parse		**tp;
	int			*ti;
//	int			current;
	int			pa;

	if (!(tp = malloc(sizeof(t_parse **) * (ft_strlen(expr) + 1)))
	|| (!(ti = malloc(sizeof(int *) * (ft_strlen(expr) + 1)))))
		exit(1);
	pa = 0;
	pa = 0;
	ti[pa] = 0;
	tp[pa] = parse_it(expr, ft_strlen(expr));
	tp[pa]->dec = 0;
	tp[pa + 1] = NULL;
	ti[pa + 1] = EMPTY;

	/*
	typedef struct		s_c mmand
	{
	int	type;
	int	argc;
	char	**argv;
	struct s_command	**argcmd;
	struct s_redirect **redirect;
	}	t_command;
	*/
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
				|| !(cmds[ic]->redirect = malloc(sizeof(void *) * (ft_strlen(expr) + 1)))
				|| !(cmds[ic]->bitcode = malloc(sizeof(void *) * (ft_strlen(expr) + 1))))
						return (NULL);
				ra = 0;
				cmds[ic]->bitcode[ra] = 0;
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
						/*
								struct s_redirect **redirect;
								typedef struct		s_redirect
								{
										int	type;
										int	fd_in;
										int	fd_out;
								}			t_redirect;
						*/
						#define REDIRECT_TRNC 0
						#define REDIRECT_NORMAL 1
						//redirect_stuff
						ft_printf("int %s: %d(%c) to %d(%c)\n", (expr + tp[pa]->dec), tp[pa]->begin[ti[pa] + a + ra], expr[tp[pa]->begin[ti[pa] + a + ra]], tp[pa]->end[ti[pa] + a + ra], expr[tp[pa]->end[ti[pa] + a + ra]]);
						cmds[ic]->redirect[ra] = get_redirection(cpy_a_to_b((expr + tp[pa]->dec), tp[pa]->begin[ti[pa] + a + ra], tp[pa]->end[ti[pa] + a + ra]));
						cmds[ic]->bitcode[ra + a] = 0;
						ra++;
					}
					else if (tp[pa]->type[ti[pa] + a + ra] != EMPTY && g_delimiter[tp[pa]->type[ti[pa] + a + ra]].do_recursivity)
					{
						cmds[ic]->argcmd = parse_commands(cpy_a_to_b((expr + tp[pa]->dec), tp[pa]->begin[ti[pa] + a + ra], tp[pa]->end[ti[pa] + a + ra]));
						cmds[ic]->bitcode[ra + a] = 0;
						t_tstr[a++] = NULL;
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
				cmds[ic]->argc = len;
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
					//free(t_tstr[i]);
				}
				cmds[ic]->argv[argv_i] = NULL;
				ic++;
			}
			else
			{
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
	return (updated(cmds));
}
