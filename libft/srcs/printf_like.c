#include "libft.h"
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>

static int			ft_findtype(char type, va_list pa, t_flag *flag)
{
	t_type t;

	if ((find_dioux(type, pa, flag, &t)))
		return (0);
	if ((find_scp(type, pa, flag, &t)))
		return (0);
	if (find_bonus(type) == 1)
		return (0);
	return (1);
}

static const char	*ft_gestion(const char *restrict format, va_list pa)
{
	int		num;
	t_flag	flag;

	ft_initflag(&flag);
	while ((ft_findtype(*format, pa, &flag) > 0))
	{
		num = ft_findflag(format, &flag, pa);
		if (num == 0)
		{
			ft_print_char(*format, &flag);
			return (format);
		}
		else
			format += num;
	}
	return (format);
}

int					ft_dprintf(int fd, const char *restrict format, ...)
{
	va_list pa;
  int old_fd;

	if (format == NULL)
		return (0);
  old_fd = get_fd(0);
  if (fd > 0)
    get_fd(fd);
	ret_val(0, 1);
	writex(NULL, 0, -1);
	va_start(pa, format);
	while (*format)
	{
		if (*format == '%')
		{
			format++;
			format = ft_gestion(format, pa);
		}
		else if (*format == '{')
			format += ft_findbonus(format);
		else
			ft_putc(*format);
		(*format) ? format++ : 0;
	}
	va_end(pa);
	writex(NULL, 0, 1);
  get_fd(old_fd);
	return (ret_val(0, 0));
}
