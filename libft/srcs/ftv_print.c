/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vector_print.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguthert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/02 16:58:20 by jguthert          #+#    #+#             */
/*   Updated: 2016/01/03 13:44:03 by jguthert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void		ftv_print(t_ftv *ftv)
{
	char	buff[65];

	ft_putstr_fd("Capacity : ", 2);
	ft_putnbr_fd(ftv->capacity, 2);
	ft_putstr_fd("    Size : ", 2);
	ft_putnbr_fd(ftv->size, 2);
	ft_putstr_fd("    Element_size : ", 2);
	ft_putnbr_fd(ftv->element_size, 2);
	ft_putstr_fd("    Data : 0x", 2);
	ft_putstr_fd(ft_ulltoa_base((uint64_t)ftv->data, 16, buff), 2);
	ft_putchar_fd('\n', 2);
}
