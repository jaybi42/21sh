/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   acurse.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agadhgad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/24 12:51:54 by agadhgad          #+#    #+#             */
/*   Updated: 2016/11/27 17:07:46 by ibouchla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ACURSE_H
# define ACURSE_H

# include <unistd.h>
# include <stdarg.h>
# include <stdlib.h>
# include <termios.h>
# include <curses.h>
# include <term.h>
# include <sys/ioctl.h>
# include <fcntl.h>
# include <signal.h>
# include <stdio.h>

# include "libft.h"

# define NEWSIG -2
# define NEWCURSOR -1
# define NEWSCREEN 0
# define GETX 1
# define GETY 2
# define GETCX 3
# define GETCY 4
# define GETSIG 5
# define INIT -3

typedef enum	e_event
{
	E_KEY_LEFT = 4479771,
	E_KEY_RIGHT = 4414235,
	E_KEY_UP = 4283163,
	E_KEY_DOWN = 4348699,
	E_ESC = 27,
	E_SPACE = 32,
	E_DEL = 127,
	E_SUP = 2117294875,
	E_RET = 10,
	E_END = 4610843,
	E_TAB = 9,
	E_HOME = 4741915,
	E_RESIZE = 322
}				t_event;

/*
** a_event.c
*/

void			handle_stdout(int action, int fd_tty);
int				a_store_old_term(int ret);
int				a_event(void);
int				a_wait_event(unsigned char **b);
int				a_stop(int ret);
void			a_exit(int ret);
int				a_init(void);

/*
** a_screen.c
*/

int				a_storage_data(int id, int tx, int ty);
void			a_getscreen(int *x, int *y);
int				a_getscreen_y(void);
int				a_getscreen_x(void);
void			a_getcursor(int *x, int *y);

/*
** a_handle.c
*/

int				a_print(int c);
void			a_tputs(char *s);
void			a_mv(int x, int y);
void			a_resize(int id);
void			a_tputs(char *s);
void			a_handle_signal(void);
void			a_stopit(int id);
void			a_leave(int id);

/*
** a_mode.c
*/

void			a_mode_video_inverted(void);
void			a_mode_surlight(void);
void			a_mode_off(void);
void			a_cursor_off(void);
void			a_cursor_on(void);

/*
** a_print.c
*/

int				a_printf(const char *restrict format, ...);
int				a_printg(int x, int y, const char *restrict format, ...);

/*
** a_clear.c
*/
void			a_init_clear(void);
void			a_clear(void);

#endif
