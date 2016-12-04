#include "21sh.h"

void print_err(char *err, char *what)
{
	ft_dprintf(2, "%s: %s: %s\n", NAME, err, what);
}
