/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/18 16:59:38 by jguthert          #+#    #+#             */
/*   Updated: 2016/12/18 17:08:20 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "edit_line.h"
#include "shell.h"

static void print_str(t_line *l, int window)
{
    int a;

    a = l->start_print;
    while (l->str[++a] != '\0' && --window)
    {
        ft_putchar(l->str[a]);
        if (l->largeur != 0 && l->count != 0
			&& (l->count + l->sizeprompt + 1) % l->largeur == 0)
            do_term("do");
        l->count++;
    }
}

static void put_cursor(t_line *l, int window)
{
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

static void big_print(t_line *l, int window)
{
    //Début - Fonction d'initialisation
    //fonction ci pour avoir les 5 fonctions de print dans un même fichier...
    if (l->start_print == -1)//Si c'est la première fois que l'on affiche le gros print
    {
        l->start_print = l->size - window;
        l->final_count = l->size;
    }
    else if (l->final_count < l->start_print)//si on a déplacé le curseur vers le début, on met à jour start_print
        l->start_print = l->final_count;//Voir si il faut mettre final_count à la place de count
    else if (l->final_count > l->start_print + window)//Si on a déplacé le curseur vers la fin, on met à jour start_print
	l->start_print = l->final_count - window;//Voir si il faut mettre final_count à la place de count
    //Fin - Fonction d'initialisation
    do_term("cl");
    print_prompt(g_prompt.rand, g_prompt.g_env, g_lenv, g_prompt.l);
	if (l->start_print != 0)//On gère le cas où le début de la str a été print
        ft_putstr(". . . .\n");
    print_str(l, window);
    do_goto("cm", 0, 0);
    put_cursor(l, window);
    if (l->start_print + window < l->size)//On gère le cas où la fin de la str n'a pas été print
        ft_putstr("\n. . . .");
}

static void little_print(t_line *l, int window)
{
    ft_home(l);
    do_term("cd");
    print_str(l, window);
    ft_home(l);
    put_cursor(l, window);
}

void        print(t_line *l)
{
    int window;

    window = (l->hauteur - 4) * l->largeur;//4 lignes pour ne pas se prendre la tête
    if (l->size > window)
        big_print(l, window);
    else
        little_print(l, window);
}
