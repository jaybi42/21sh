#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "21sh.h"

#define DEBUG 1

char		*cpy_a_to_b2(char *str, int a, int b)
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

typedef struct s_globbing
{
	/*tell if we must look for it or try to not find it! */
	int is_looking;

	/*
	   tell if we looking for this expression or one c of this
	 */
	int list;

	char *exprs;
	/*
	   tell if we must look for 1 string or just 1 char
	 */
	int length_one;
}							t_globbing;

int tlen(char **t)
{
	int i;

	i = -1;
	while(t[++i]);
	return (i);
}

void finder(t_globbing *g, int **p_d, int **p_d_bannish, char **words)
{
	int a_w;

	a_w = -1;
	while(words[++a_w])
	{
		int find;
		find = 0;
		//printf("try to find |%s| ; %d|%c|\n", g->exprs,  (*p_d)[a_w], words[a_w][(*p_d)[a_w]]);
		if ((*p_d_bannish)[a_w] == 2)
		{
			//printf("info: it's ban, we don't go further\n");
			continue;
		}
		if (g->exprs != NULL)
		{
			//if we are in suspension of jugement
			if ((*p_d_bannish)[a_w] == 1)
			{
				//printf("info: last g was in 'soj'. trying to find the next\n");
				int a_w_jugement;
				int debug_list = 1;
				//printf("oh suspension of jugement!! w %d and d = %d\n", a_w, (*p_d)[a_w]);
				a_w_jugement = (*p_d)[a_w];
				//trying to find an other characters
				int leave;
				leave = 0;
				while(words[a_w][a_w_jugement])
				{
					if (g->list == 1)
					{
						if (debug_list)
						{
							//printf("info: soj: we have a list[%s], check everyone...\n", g->exprs);
							debug_list = 0;
						}
						int i;
						i = 0;
						while(g->exprs[i])
						{
							if (g->exprs[i] == words[a_w][a_w_jugement])
							{
								if (g->is_looking)
								{
									(*p_d)[a_w] += a_w_jugement + 1;
									(*p_d_bannish)[a_w] = 0;
									find = 1;
								}
								else
									(*p_d_bannish)[a_w] = 2;
								//printf("info: find a |%c|, stopping the search\n", g->exprs[i]);
								leave = 1;
								break;
							}
							i++;
						}
					}
					else if (!strncmp(words[a_w] + a_w_jugement, g->exprs, strlen(g->exprs)))
					{
						(*p_d)[a_w] = a_w_jugement + strlen(g->exprs);
						(*p_d_bannish)[a_w] = 0;
						//printf("info: find the string |%s|, stopping the search\n", g->exprs);
						find = 1;
						leave = 1;
						break;
					}
					if (leave)
						break;
					a_w_jugement++;
				}
				if (!g->is_looking && !find)
					find = 1;
			}
			else
			{
				//printf("try to force! with %d - %c\n", (*p_d)[a_w], words[a_w][(*p_d)[a_w]]);
				if (g->list == 1)
				{
					//printf("info: nor: we have a list[%s], check everyone...\n", g->exprs);
					int i;
					i = 0;
					while(g->exprs[i])
					{
						if (g->exprs[i] == words[a_w][(*p_d)[a_w]])
						{
							if (g->is_looking)
							{
								(*p_d)[a_w] += 1;
								find = 1;
							}
							else
								(*p_d_bannish)[a_w] = 2;
							//printf("info: lis: find %c\n", g->exprs[i]);
							break;
						}
						i++;
					}
					if (!g->is_looking && !find)
					{
						find = 1;
						//printf("info: nor: didn't find it, but ^ so it's ok\n");
					}
				}
				else if (g->length_one == 1)
				{
					if (words[a_w][(*p_d)[a_w]] == g->exprs[0])
						find = 1;
				}
				else if (!strncmp(words[a_w] + (*p_d)[a_w], g->exprs, strlen(g->exprs)))
				{
					(*p_d)[a_w] += strlen(g->exprs);
					//printf("info: nor: find %s\n", g->exprs);
					find = 1;
				}
			}
		}
		else if (g->exprs == NULL)
		{
			if (g->length_one == 1)
			{
				if ((size_t)(*p_d)[a_w] < strlen(words[a_w]))
				{
					//printf("info: ???: find a ?, %d|%c|\n",(*p_d)[a_w], words[a_w][(*p_d)[a_w]]);
					(*p_d)[a_w]++;
				}
				else
					(*p_d_bannish)[a_w] = 2;
			}
			else
			{
				//printf("info: soj\n");
				(*p_d_bannish)[a_w] = 1; //mean "in suspension of jugement"
			}
			find = 1;
		}
		if (!find)
		{
			//printf("bannish it!\n");
			(*p_d_bannish)[a_w] = 2;
		}
	}
}
void add_char(char **exprs, int *len, char c)
{
	int succeed;
	int tmp;

	succeed = 1;
	tmp = 0;
	while (tmp < (*len))
	{
		if ((*exprs)[tmp] == c)
		{
			succeed = 0;
			break;
		}
		tmp++;
	}
	if (succeed)
	{
		(*exprs)[(*len)++] = c;
		(*exprs)[(*len)] = '\0';
	}
}

t_globbing *quarter(char *expr, int *tmp_len)
{
	int i;
	int a_str;
	t_globbing *globbing;

	if (!(globbing = xmalloc(sizeof(t_globbing))))
		return (NULL);
	if (!(globbing->exprs = xmalloc(sizeof(char) * 127)))
		return (NULL);
	globbing->list = 1;
	i = 0;
	a_str = 0;
	if (expr[i] == '^')
	{
		globbing->is_looking = 0;
		i++;
	}
	else
		globbing->is_looking = 1;
	while (expr[i] && expr[i] != ']')
	{
		if (expr[i] == '-' && i != 0 && (size_t)(i + 1) != strlen(expr))
		{
			if (expr[i - 1] <= expr[i + 1])
			{
				char c;

				c = expr[i - 1] - 1;
				while (++c <= expr[i + 1])
					add_char(&globbing->exprs, &a_str, c);
			}
			else
				add_char(&globbing->exprs, &a_str, expr[i]);
		}
		else
			add_char(&globbing->exprs, &a_str, expr[i]);
		i++;
	}
	if (expr[i] != ']' || i == 0)
	{
		return (NULL);
	}
	(*tmp_len) = i;
	return (globbing);
}

t_globbing *rec_g(char *expr, int *i, int iquare)
{
	t_globbing *g;
	int c;
	int careone;

	//printf("%d|%c|\n",(*i), expr[(*i)]);
	if (!(g = xmalloc(sizeof(t_globbing))))
		return (NULL);
	c = 0;
	careone = 0;
	while (expr[(*i) + c] && expr[(*i) + c] != '?' && expr[(*i) + c] != '*')
	{
		if (iquare && expr[(*i) + c] == '[')
		{
			//can have pblm... to correct
			break;
		}
		c++;
	}
	g->is_looking = 1;
	g->list = 0;
	g->exprs = cpy_a_to_b2(expr, (*i), (*i) + c);
	(*i) += c - 1;
	return (g);
}

char	**find_globbing(t_globbing **gs, int a, char **words)
{
	int *d;
	int *d_bannish;
	int len = tlen(words);
	int a_g;
	int a_w;

	if (!(d = xmalloc(sizeof(int) * (len + 1))))
		return (NULL);
	if (!(d_bannish = xmalloc(sizeof(int) * (len + 1))))
		return (NULL);
	a_w = -1;

	while (++a_w < len)
	{
		d_bannish[a_w] = 0;
		d[a_w] = 0;
	}
	a_g = -1;
	while (++a_g < a)
	{
		//printf(">%d\n", a_g);
		finder(gs[a_g], &d, &d_bannish, words);
	}
	char **ret;
	if (!(ret = xmalloc(sizeof(char *) * (tlen(words) + 1))))
		return (NULL);
	a_w = -1;
	a = 0;
	while (words[++a_w] != NULL)
		if (d_bannish[a_w] == 0 || d_bannish[a_w] == 1)
			ret[a++] = strdup(words[a_w]);
	ret[a] = NULL;
	return (ret);
}

/*char            **fstrsplit(char *str, int len, int (*is_whatever)(char))
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
			t_str[a++] = cpy_a_to_b2(str, beg, i);
	}
	t_str[a] = NULL;
	return(t_str);
}*/

int accolade_isit(char c)
{
	if (c == ',')
		return (1);
	else
		return (0);
}

int ttlen(char ***t)
{
	int i;
	int a;
	int len;

	len = 0;
	a = 0;
	while (t[a])
	{
		i = 0;
		while(t[a][i])
		{
			len++;
			i++;
		}
		a++;
	}
	return (len);
}

char **fusion_tarray(char ***t)
{
	char **ret;
	int a;
	int i;
	int len;

	if (!(ret = malloc(sizeof(char **) * (ttlen(t) + 1))))
		return (NULL);
	a = 0;
	len = 0;
	while (t[a])
	{
		i = 0;
		while (t[a][i])
		{
			ret[len++] = ft_strdup(t[a][i]);
			i++;
		}
		a++;
	}
	ret[len] = NULL;
	return (ret);
}

char	*x_strjoin2(char *s1, size_t len1, char *s2, size_t len2)
{
	int i;
	int a;
	char *s;

	a = 0;
	s = xmalloc(sizeof(char) * (len1 + len2 + 1));
	if (!s)
		return (NULL);
	i = -1;
	while (++i < (int)len1)
		s[a++] = s1[i];
	i = -1;
	while (++i < (int)len2)
		s[a++] = s2[i];
	s[a] = '\0';
	return (s);
}

size_t len_depth(char *s)
{
	size_t i;
	int d;

	i = 0;
	d = 1;
	while (s[i])
	{
		if (s[i] == '{')
			d++;
		else if (s[i] == '}')
			d--;
		i++;
		if (d == 0)
			break;
	}
	return (i);
}

char *x_strdup2(char *s, int len)
{
        int i;
        char *ns;

        i = 0;
        ns = xmalloc(sizeof(char) * (len + 1));
        if (ns == NULL)
                return (NULL);
        while (i < len)
        {
                ns[i] = s[i];
                i++;
        }
        ns[i] = '\0';
        return (ns);
}

char **ret_arr(char *s)
{
	char **t_s;
	int *t_a;
	int t_i;
	int i;
	int tog;
	int len;

	i = 0;
	len = strlen(s);
	t_s = xmalloc(sizeof(char *) * (len + 1));
	t_a = xmalloc(sizeof(int) * (len + 1));
	t_i = 0;
	t_a[t_i] = 0;
	t_s[t_i++] = xmalloc(sizeof(char) * (len + 1));
	t_s[t_i] = NULL;
	t_s[t_i - 1][0] = '\0';
	tog = 1;
	while (s[i])
	{
		if (s[i] == '}')
			break;
		else if (s[i] == ',')
		{
			t_s[t_i] = xmalloc(sizeof(char) * (len + 1));
			t_s[t_i][0] = '\0';
			t_a[t_i] = 0;
			tog = 1;
			t_s[++t_i] = NULL;
		}
		else if (s[i] == '{')
		{
			char **a_tmp = ret_arr(s + i + 1);
			int i_tmp;
			if (a_tmp == NULL)
				return (NULL);
			char *old_tmp = x_strdup2(t_s[t_i - 1], t_a[t_i - 1]);
			if (old_tmp == NULL)
				return (NULL);
			i_tmp = 0;
			while (a_tmp[i_tmp] != NULL)
			{
				if (t_s[t_i - 1 + i_tmp] != NULL)
				{
					t_s[t_i - 1 + i_tmp] = x_strjoin2(t_s[t_i - 1 + i_tmp], t_a[t_i - 1 + i_tmp],
							a_tmp[i_tmp],strlen(a_tmp[i_tmp]));
					t_a[t_i - 1 + i_tmp] += strlen(a_tmp[i_tmp]);
				}
				else
				{
					t_s[t_i - 1 + i_tmp] = x_strjoin2(old_tmp, strlen(old_tmp),
							a_tmp[i_tmp], strlen(a_tmp[i_tmp]));
					t_a[t_i - 1 + i_tmp] = strlen(old_tmp) + strlen(a_tmp[i_tmp]);
				}
				i_tmp++;
			}
			t_i += i_tmp - 1;
			tog += i_tmp - 1;
			int xx;
			i += (xx = len_depth(s + i + 1));
		}
		else
		{
			int i_tmp = 0;
			while (i_tmp < tog)
			{
				t_s[t_i - 1 - i_tmp][t_a[t_i - 1 - i_tmp]++] = s[i];
				t_s[t_i - 1 - i_tmp][t_a[t_i - 1 - i_tmp]] = '\0';
				i_tmp++;
			}
		}
		i++;
	}
	if (i == len)
	{
		return (NULL);
	}
	return (t_s);
}

typedef struct s_brack
{
	char **t;
	int *i_a;
	int ta;
	int len;
}				t_brak;


void	insert_in_arr(t_brak *b, char **t_add)
{
	int i;
	int a;
	char **tmp;
	int x;

	a = 0;
	x = 0;
	tmp = xmalloc(sizeof(char *) * b->len);
	while (a < b->ta)
	{
		i = 0;
		while (t_add[i])
		{
			tmp[x++] = x_strjoin2(b->t[a],b->i_a[a], t_add[i], strlen(t_add[i]));
			i++;
		}
		a++;
	}
	tmp[x] = NULL;
	b->t = tmp;
	a = 0;
	while (b->t[a] != NULL)
	{
		b->i_a[a] = strlen(b->t[a]);
		a++;
	}
	b->ta = a;
}

void update_t(char ***p_t, int **p_i_a, int *p_ta, char c)
{
	int i;

	i = -1;
	while (++i < (*p_ta))
		(*p_t)[i][(*p_i_a)[i]++] = c;
}


int	play_with_bracket(t_brak *b, char *s)
{
	char **t_to_add;
	int d;
	int len;
	int i;

	if ((t_to_add = ret_arr(s)) != NULL)
	{
		i = 0;

		insert_in_arr(b,t_to_add);

		d = 1;
		len = len_depth(s);
		return (len + 1);
	}
	else
	{
		update_t(&b->t, &b->i_a, &b->ta, '{');
	}
	return (1);
}
void close_t(char ***p_t, int **p_i_a, int *p_ta)
{
	int i;

	i = 0;

	while (i < (*p_ta))
	{
		(*p_t)[i][(*p_i_a)[i]] = '\0';
		i++;
	}
	(*p_t)[i] = NULL;
}

char	**braceSplit(char *s)
{
	t_brak b;
	int i;

	b.len = strlen(s) + 1;
	if (!(b.t = xmalloc(sizeof(char *) * b.len)) ||
			!(b.i_a = xmalloc(sizeof(int) * b.len)))
		return (NULL);
	if (!(b.t[0] = xmalloc(sizeof(char) * b.len)))
		return (NULL);
	b.i_a[0] = 0;
	b.ta = 1;
	i = 0;
	while(s[i])
	{
		if (s[i] == '{')
			i += play_with_bracket(&b, s + i + 1);
		else
		{
			update_t(&b.t, &b.i_a, &b.ta, s[i]);
			i++;
		}
	}
	close_t(&b.t, &b.i_a, &b.ta);
	return (b.t);
}

char **brace_handler(char *s)
{
	char **exprs;

	exprs = braceSplit(s);

	return (exprs);
}

char	**ft_globing(char *expr, char **words)
{
	t_globbing ***tgs;
	int *gti;
	int *gta;
	char **exprs;
	t_globbing *g_tmp;
	int c;
	int ta;

	if (!(exprs = xmalloc(sizeof(int) * (strlen(expr) + 1))))
		return (NULL);
	exprs = brace_handler(expr);
	if (!(gti = xmalloc(sizeof(int) * (strlen(expr) + 1))))
		return (NULL);
	if (!(tgs = xmalloc(sizeof(t_globbing **) * (strlen(expr) + 1))))
		return (NULL);
	if (!(gta = xmalloc(sizeof(int) * (strlen(expr) + 1))))
		return (NULL);
	ta = 0;
	while (exprs[ta])
	{
		gti[ta] = 0;
		gta[ta] = 0;
		if (!(tgs[ta] = xmalloc(sizeof(t_globbing *) * (strlen(expr) + 1))))
			return (NULL);
		ta++;
	}
	tgs[ta] = NULL;
	ta = 0;
	while(exprs[ta])
	{
		while (exprs[ta][gti[ta]])
		{
			c = 0;
			if (exprs[ta][gti[ta]] == '[')
			{
				int tmp_len;
				tmp_len = 0;
				t_globbing *g_tmp;
				if ((g_tmp = quarter(exprs[ta] + gti[ta] + 1, &tmp_len)) != NULL)
				{
					tgs[ta][gta[ta]++] = g_tmp;
					gti[ta] += tmp_len + 1;
				}
				else if ((g_tmp = rec_g(exprs[ta], &gti[ta], 0)) != NULL)
					tgs[ta][gta[ta]++] = g_tmp;
				else
					return (NULL);
			}
			else if (exprs[ta][gti[ta]] == '*')
			{
				while(exprs[ta][gti[ta] + c] == '*')
					c++;
				if (!(tgs[ta][gta[ta]] = xmalloc(sizeof(t_globbing))))
					return (NULL);
				tgs[ta][gta[ta]]->is_looking = 1;
				tgs[ta][gta[ta]]->length_one = 0;
				tgs[ta][gta[ta]]->list = 0;
				tgs[ta][gta[ta]++]->exprs = NULL;
				gti[ta] += c - 1;
			}
			else if (exprs[ta][gti[ta]] == '?')
			{
				if (!(tgs[ta][gta[ta]] = xmalloc(sizeof(t_globbing))))
					return (NULL);
				tgs[ta][gta[ta]]->is_looking = 1;
				tgs[ta][gta[ta]]->length_one = 1;
				tgs[ta][gta[ta]]->list = 0;
				tgs[ta][gta[ta]++]->exprs = NULL;
			}
			else if ((g_tmp = rec_g(exprs[ta], gti + ta, 1)) != NULL)
			{
				tgs[ta][gta[ta]++] = g_tmp;
			}
			else
				return (NULL);
			gti[ta]++;
		}
		if (!(tgs[ta][gta[ta]] = xmalloc(sizeof(t_globbing))))
			return (NULL);
		tgs[ta][gta[ta]]->exprs = strdup("");
		tgs[ta][gta[ta]]->is_looking = 1;
		tgs[ta][gta[ta]]->list = 0;
		tgs[ta][gta[ta]]->length_one = 1;
		tgs[ta][++gta[ta]] = NULL;
		ta++;
	}
	tgs[ta] = NULL;
	int x = 0;
	ta = 0;
	while (tgs[ta])
	{
		x = 0;
		while (tgs[ta][x])
		{
			//printf("|%s| %d\n", tgs[ta][x]->exprs, tgs[ta][x]->list);
			x++;
		}
		ta++;
	}
	char ***t;
	if (!(t = xmalloc(sizeof(char ***) * (strlen(expr) + 1))))
		return (NULL);
	x = 0;
	while (tgs[x] != NULL)
	{
		t[x] = find_globbing(tgs[x], gta[x], words);
		if (t[x] == NULL)
		{
			return (NULL);
		}
		x++;
	}
	t[x] = NULL;
	return (fusion_tarray(t));
}
