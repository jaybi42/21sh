/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/13 14:10:31 by jguthert          #+#    #+#             */
/*   Updated: 2016/12/11 17:52:57 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stdint.h>
# include <stdbool.h>
# include <stddef.h>
# include <stdarg.h>
# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <errno.h>

# include "acurse.h"

# define BUFF_SIZE	512
# define READLEN	4096

# define ERROR ft_error(__FILE__, __FUNCTION__, __LINE__)
# define ERRORNO ft_errorno(__FILE__, __FUNCTION__, __LINE__)
# define ABS(NBR) (((NBR) < 0) ? -(NBR) : (NBR))
# define FTV_INIT(TYPE) ((t_ftv){NULL, 0, 0, sizeof(TYPE)})

# define FTL_NODE	struct	s_ftl_node
# define FTL_NODEC	struct	s_ftl_node const

typedef struct				s_output
{
	int						len;
	char					*string;
	int						ret_code;
}							t_output;

typedef struct				s_ftl_node
{
	FTL_NODE				*next;
	FTL_NODE				*prev;
}							t_ftl_node;

typedef struct				s_ftl_root
{
	FTL_NODE				*next;
	FTL_NODE				*prev;
	size_t					size;
	size_t					node_size;
}							t_ftl_root;

typedef struct				s_list
{
	void					*content;
	size_t					content_size;
	struct s_list			*next;
}							t_list;

typedef struct				s_gnl
{
	int						fd;
	char					*tmp;
	struct s_gnl			*next;
}							t_gnl;

typedef struct				s_ftv
{
	void					*data;
	size_t					capacity;
	size_t					size;
	size_t					element_size;
}							t_ftv;

/*
**	##=- ft_printf -=##
*/

typedef struct				s_type
{
	int						n;
	long int				ln;
	long long int			lln;
	unsigned long int		lu;
	unsigned long long int	llu;
	char					*str;
	wchar_t					*wstr;
	uintmax_t				ju;
	intmax_t				j;
	char					c;
	size_t					z;
	double					f;
	unsigned int			octal;
	unsigned int			u;
	unsigned int			x;
	void					*ptr;
}							t_type;

typedef struct				s_flag
{
	int						sp_b;
	int						less_b;
	int						sign_b;
	int						zero_b;
	int						diese_b;
	int						end;
	int						preci;
	int						preci_b;
	int						p_b;
	int						mini;
	int						bonus_base;
	char					modif[2];
}							t_flag;

typedef struct				s_fdp
{
	int						len_str;
	int						len_buff;
	char					*str;
	char					buffer[READLEN + 1];
}							t_fdp;

void						ft_putstr_dbcote(const char *s);
void						writex(char *str, size_t length, int print);
char						*ft_dela_to_b(char *s, int a, int b);
char						*ft_litoa(unsigned long long nb);
char						*ft_itoa_base(unsigned long long nb, char base,
							int size);
int							ft_printf(const char *restrict format, ...);
int							ft_addmodif(const char *restrict pf, t_flag *f);
int							ft_findflag3(const char *restrict pf, t_flag *f);
int							ft_findbonus(const char *restrict format);
int							ft_print_wc(unsigned int c, t_flag *f);
int							ft_print_char(char c, t_flag *f);
int							ft_print_s(char *s, t_flag *f);
int							ft_print_ws(wchar_t *s, t_flag *f);
int							ft_print_valu(unsigned long long int n, t_flag *f,
							int base);
int							ft_print_val(long long int n, t_flag *f);
void						ft_putnorme(int base, t_flag *f, int upper);
void						ft_mini(int longueur, t_flag *f, long long int *nb);
void						ft_miniu(int longueur, t_flag *f, int b);
void						ft_putns(char *s, int n);
void						ft_putc_r(int q, char c);
int							ret_val(int val, int first);
void						ft_putc(char c);
void						ft_puts(char *s);
void						ft_putnws(wchar_t *s, size_t len);
void						ft_putwchar(unsigned int c);
size_t						ft_wwlen(wchar_t c);
size_t						ft_wwstrlen(wchar_t *s);
size_t						ft_wwstrlen(wchar_t *s);
int							find_ubase(char type, t_flag *f);
int							tbase(char type);
int							utbase(char type);
int							find_dioux(char type, va_list pa, t_flag *f,
							t_type *t);
int							find_scp(char type, va_list pa, t_flag *flag,
							t_type *t);
int							ft_findflag(const char *restrict pf, t_flag *f,
							va_list pa);
void						ft_initflag(t_flag *flag);
int							get_fd(int fd);
int							find_bonus(char type);
int							ft_dprintf(int fd, const char *restrict format,
							...);

/*
**	##=-  Print  -=##
*/

void						ft_putchar(char c);
void						ft_putstr(char const *s);
void						ft_putendl(char const *s);
void						ft_putnbr(int n);
void						ft_putchar_fd(char c, int fd);
void						ft_putstr_fd(char const *s, int fd);
void						ft_putendl_fd(char const *s, int fd);
void						ft_putnbr_fd(int n, int fd);

/*
**	##=-  String  -=##
*/

size_t						ft_strlen(char const *s);
char						*ft_strcpy(char *dst, char const *src);
char						*ft_strncpy(char *dst, const char *src, size_t n);
char						*ft_strcat(char *s1, const char *s2);
char						*ft_strdup(const char *s1);
char						*ft_strndup(const char *s1, size_t n);
char						*ft_strncat(char *s1, const char *s2, size_t n);
size_t						ft_strlcat(char *dst, const char *src, size_t size);
char						*ft_strchr(const char *s, int c);
char						*ft_strrchr(const char *s, int c);
char						*ft_strtrim(char const *s);
char						*ft_strjoin(char const *s1, char const *s2);
char						**ft_strsplit(char const *s, char c);
char						**ft_strsplit_blank(char const *s);
char						**ft_strsplit_space(char const *s);
char						*ft_strstr(const char *s1, const char *s2);
char						*ft_strnstr(const char *s1, const char *s2,
							size_t n);
char						*ft_strsub(char const *s, unsigned int start,
							size_t len);
int							ft_strcmp(const char *s1, const char *s2);
int							ft_strncmp(const char *s1, const char *s2,
							size_t n);
int							ft_strequ(char const *s1, char const *s2);
int							ft_strnequ(char const *s1, char const *s2,
							size_t n);
char						*ft_strnew(size_t size);
void						ft_strdel(char **as);
void						ft_strclr(char *s);

void						ft_striter(char *s, void (*f)(char *));
void						ft_striteri(char *s,
							void (*f)(unsigned int, char *));
char						*ft_strmap(char const *s, char (*f)(char));
char						*ft_strmapi(char const *s,
							char (*f)(unsigned int, char));
void						ft_tabdel(char **tab2);
int							ft_strisnum(char *str);
char						*ft_first_word(char *str);
void						ft_print_tab(char **tab2, char *name);

/*
**	##=-  Get information  -=##
*/

int							ft_nbrlen(uint64_t nbr);
int							ft_tablen(char **tab2);
int							ft_listlen(t_list *list);

/*
**	##=-  Adaptator  -=##
*/

int							ft_atoi(const char *str);
char						*ft_itoa(int n);
char						*ft_ulltoa_base(uint64_t nbr, int base, char *str);
int							ft_getnbr(char *str);

/*
**	##=-  Casse  -=##
*/

int							ft_isspace(int c);
int							ft_isblank(int c);
int							ft_isalpha(int c);
int							ft_isdigit(int c);
int							ft_isalnum(int c);
int							ft_isascii(int c);
int							ft_isprint(int c);
int							ft_toupper(int c);
int							ft_tolower(int c);
int							ft_strisalnum(char *str);
int							ft_strisprint(char *str);

/*
**	##=-  Memory  -=##
*/

void						ft_bzero(void *s, size_t n);
void						*ft_memset(void *b, int c, size_t len);
void						*ft_memcpy(void *dst, const void *src, size_t n);
void						*ft_memccpy(void *dst, const void *src, int c,
							size_t n);
void						*ft_memmove(void *dst, const void *src, size_t len);
void						*ft_memchr(const void *s, int c, size_t n);
int							ft_memcmp(const void *s1, const void *s2, size_t n);
void						*ft_memalloc(size_t size);
void						ft_memdel(void **ap);
void						*ft_memdup(void const *data, size_t size);

/*
**	##=-  Simple chained list  -=##
*/

void						ft_lst_sort(t_list **alst, int (*f_cmp)());
t_list						*ft_lstnew(void const *content,
							size_t content_size);
void						ft_lstdelone(t_list **alst,
							void (*del)(void *, size_t));
void						ft_lstdel(t_list **alst,
							void (*del)(void *, size_t));
void						ft_lstadd(t_list **alst, t_list *new);
void						ft_lstadd_last(t_list **alst, t_list *new);
void						ft_lstiter(t_list *lst, void (*f)(t_list *elem));
t_list						*ft_lstmap(t_list *lst, t_list *(*f)(t_list *elem));

/*
**	##=-  Double chained list looped  -=##
*/

t_ftl_root					ftl_uninit(void);
void						ftl_init(t_ftl_root *l, size_t size);
int							ftl_insert_pos(t_ftl_root *l, FTL_NODE *pos,
							FTL_NODEC *nod);
int							ftl_push_front(t_ftl_root *l, FTL_NODEC *node);
int							ftl_push_back(t_ftl_root *l, FTL_NODEC *node);
void						ftl_release(t_ftl_root *l, void (*delalloc)());
void						ftl_pop_back(t_ftl_root *l, void (*delalloc)());
void						ftl_pop_front(t_ftl_root *l, void (*delalloc)());
void						ftl_erase_pos(t_ftl_root *l, t_ftl_node *pos,
							void (*delalloc)());

/*
**	##=-  Math  -=##
*/

int							ft_sqrt(int nbr);
int							ft_power(int nbr, int index);

/*
**	##=-  GNL  -=##
*/

int							get_next_line(int const fd, char **line);

/*
**	##=-  Vector  -=##
*/

void						ftv_print(t_ftv *ftv);
int							ftv_pushback(t_ftv *ftv, void const *new_data);
int							ftv_reserve(t_ftv *ftv, size_t new_cap);
void						ftv_del(t_ftv *ftv, void (*del)());
void						ftv_popback(t_ftv *ftv, void (*del)());
void						ftv_del_len(t_ftv *v, size_t s, size_t e,
							void (*del)());

/*
**	##=-  Error  -=##
*/

int							ft_error(char const *file, char const *function,
							int line);
int							ft_errorno(char const *file,
							char const *function, int line);

#endif
