/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/29 17:07:46 by jguthert          #+#    #+#             */
/*   Updated: 2016/02/17 12:22:10 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <errno.h>
#include <string.h>

int		ft_error(char const *file, char const *function, int line)
{
	ft_putstr_fd("\033[31mError\033[0m \033[33m[\033[0m", 2);
	ft_putstr_fd(file, 2);
	ft_putstr_fd("\033[33m]\033[0m : \033[33m[\033[0m", 2);
	ft_putnbr_fd(line, 2);
	ft_putstr_fd("\033[33m]\033[0m \033[33m[\033[0m", 2);
	ft_putstr_fd(function, 2);
	ft_putendl_fd("\033[33m]\033[0m", 2);
	return (1);
}

int		ft_errorno(char const *file, char const *function, int line)
{
	int const err = errno;

	ft_putstr_fd("\033[31mError\033[0m \033[33m[\033[0m", 2);
	ft_putstr_fd(file, 2);
	ft_putstr_fd("\033[33m]\033[0m : \033[33m[\033[0m", 2);
	ft_putnbr_fd(line, 2);
	ft_putstr_fd("\033[33m]\033[0m \033[33m[\033[0m", 2);
	ft_putstr_fd(function, 2);
	ft_putstr_fd("\033[33m]\033[0m errno\033[33m[\033[0m", 2);
	ft_putstr_fd(strerror(err), 2);
	ft_putendl_fd("\033[33m]\033[0m", 2);
	return (1);
}
