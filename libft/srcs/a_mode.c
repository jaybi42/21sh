/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   a_mode.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agadhgad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/31 23:59:16 by agadhgad          #+#    #+#             */
/*   Updated: 2016/03/31 23:59:17 by agadhgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "acurse.h"

void	a_mode_video_inverted(void)
{
	a_tputs("mr");
}

void	a_mode_surlight(void)
{
	a_tputs("us");
}

void	a_mode_off(void)
{
	a_tputs("me");
}

void	a_cursor_off(void)
{
	a_tputs("vi");
}

void	a_cursor_on(void)
{
	a_tputs("ve");
}
