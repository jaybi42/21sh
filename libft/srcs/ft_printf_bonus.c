/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agadhgad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/16 16:15:12 by agadhgad          #+#    #+#             */
/*   Updated: 2016/03/28 22:31:54 by agadhgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int				ft_istrstr(const char *meule_de_foin, const char *aiguille)
{
	size_t i;

	i = 0;
	if (aiguille[0] == '\0')
		return (0);
	while (meule_de_foin[i] == aiguille[i] && aiguille[i])
		i++;
	if (i == ft_strlen(aiguille))
		return (i);
	return (0);
}

static int		ft_putslen(char *s, const char *s2)
{
	ft_puts(s);
	return (ft_strlen(s2) - 1);
}

static int		ft_findbground(const char *restrict format)
{
	if (ft_istrstr(format, "{bred}"))
		return (ft_putslen("\033[0;41m", "{bred}"));
	else if (ft_istrstr(format, "{bgreen}"))
		return (ft_putslen("\033[0;42m", "{bgreen}"));
	else if (ft_istrstr(format, "{byellow}"))
		return (ft_putslen("\033[0;43m", "{byellow}"));
	else if (ft_istrstr(format, "{bblack}"))
		return (ft_putslen("\033[0;40m", "{bblack}"));
	else if (ft_istrstr(format, "{bblue}"))
		return (ft_putslen("\033[0;44m", "{bblue}"));
	else if (ft_istrstr(format, "{bpurple}"))
		return (ft_putslen("\033[0;45m", "{bpurple}"));
	else if (ft_istrstr(format, "{bcyan}"))
		return (ft_putslen("\033[0;46m", "{bcyan}"));
	return (-1);
}

int				ft_findbonus(const char *restrict format)
{
	int r;

	if (ft_istrstr(format, "{green}"))
		return (ft_putslen("\033[0;32m", "{green}"));
	else if (ft_istrstr(format, "{eoc}"))
		return (ft_putslen("\033[0m", "{eoc}"));
	else if (ft_istrstr(format, "{yellow}"))
		return (ft_putslen("\033[0;33m", "{yellow}"));
	else if (ft_istrstr(format, "{red}"))
		return (ft_putslen("\033[0;31m", "{red}"));
	else if (ft_istrstr(format, "{black}"))
		return (ft_putslen("\033[0;30m", "{black}"));
	else if (ft_istrstr(format, "{blue}"))
		return (ft_putslen("\033[0;34m", "{blue}"));
	else if (ft_istrstr(format, "{purple}"))
		return (ft_putslen("\033[0;35m", "{purple}"));
	else if (ft_istrstr(format, "{cyan}"))
		return (ft_putslen("\033[0;36m", "{cyan}"));
	else if ((r = ft_findbground(format)) != -1)
		return (r);
	ft_putc(*format);
	return (0);
}
