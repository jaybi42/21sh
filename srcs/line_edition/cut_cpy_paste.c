/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cut_cpy_paste.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malaine <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/28 17:51:23 by malaine           #+#    #+#             */
/*   Updated: 2016/12/10 20:01:44 by ibouchla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "edit_line.h"
#include "shell.h"

static void	print_str(t_line *l, int window)
{
	int a;

	a = l->start_print - 1;
	while (l->str[++a] != '\0' && --window)
	{
		//On ne peut pas faire du putstr directement ??
		ft_putchar(l->str[a]);
		if (l->largeur != 0 && l->count != 0
		&& (l->count + l->sizeprompt + 1) % l->largeur == 0)
			do_term("do");
		l->count++;
	}
}

static void	put_cursor(t_line *l, int window)
{
	//Je ne comprends pas pourquoi on va de count à final_count. Pourquoi on ne va pas de l->start_print à count ?!
	//En sachant que pour un little_print, start_print vaut forcément 0. Comme ça on se débarasse de window par la même occasion
	//a = l->start_print;
	// while (a++ < l->count){lafonction;}
	while (l->count < l->final_count && --window)
	{
		if (l->largeur != 0 && l->count != 0
		&& (l->count + l->sizeprompt + 1) % l->largeur == 0)
			do_term("do");
		else
			do_term("nd");
		l->count++;
	}
}

static void	big_print(t_line *l, int window)
{
	//Début - Fonction d'initialisation
	//fonction ci pour avoir les 5 fonctions de print dans un même fichier...
	if (l->start_print == -1)//Si c'est la première fois que l'on affiche le gros print
	{
		l->start_print = size - window;
		l->count = l->start_print;//l->count = size si on modifie la manière de placer le curseur
		l->final_count= size;
	}
	else if (l->count < l->start_print)//si on a déplacé le curseur vers le début, on met à jour start_print
		l->start_print = count;//Voir si il faut mettre final_count à la place de count
	else if (l->count > l->start_print + window)//Si on a déplacé le curseur vers la fin, on met à jour start_print
		l->start_print = count - window;//Voir si il faut mettre final_count à la place de count
	//Fin - Fonction d'initialisation
	do_term("cl");
	print_prompt(g_prompt.rand, g_prompt.g_env, g_lenv, g_prompt.l);
    	if (l->start_print != 0)//On gère le cas où le début de la str a été print
		ft_putstr(". . . .\n");
	print_str(l, window);
	do_goto("cm", 0, 0);
	put_cursor(l, window);
	if (l->start_print + window < size)//On gère le cas où la fin de la str n'a pas été print
		ft_putstr("\n. . . .");
}

static void	little_print(t_line *l, int window)
{
	ft_home(l);
	do_term("cd");
	print_str(l, window);
	ft_home(l);
	put_cursor(l, window);
}

void		print(t_line *l)
{
	int window;
	
	window = (l->hauteur - 4) * l->largeur;//4 lignes pour ne pas se prendre la tête
	if (size > window)
		big_print(l, window);
	else
		little_print(l, window);
}

void		ft_cut(t_line *l)
{
	char	*tmp;

	do_term("cd");
	l->strcpy = x_strsub(l->str, l->count, l->size);
	if (l->strcpy == NULL)
		return ;
	if (l->count != 0)
		tmp = x_strsub(l->str, 0, l->count);
	else
		tmp = x_strdup("\0");
	if (tmp == NULL)
		return ;
	l->str = tmp;
	l->size = ft_strlen(l->str);
}

static void	do_join_paste(char *tmp, char *tmp2, t_line *l)
{
	char	*str;
	char	*str2;

	str = x_strjoin(tmp, l->strcpy);
	if (str == NULL)
		return ;
	str2 = x_strjoin(str, tmp2);
	if (str2 == NULL)
		return ;
	l->str = str2;
}

void		ft_paste(t_line *l)
{
	char	*tmp;
	char	*tmp2;

	if (l->strcpy != NULL)
	{
		tmp = x_strsub(l->str, 0, l->count);
		if (tmp == NULL)
			return ;
		tmp2 = x_strsub(l->str, l->count, l->size);
		if (tmp2 == NULL)
			return ;
		do_join_paste(tmp, tmp2, l);
		l->size = ft_strlen(l->str);
		l->final_count = l->count + ft_strlen(l->strcpy);
		print(l);
	}
}
