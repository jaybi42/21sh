
#include <shell.h>

int	bi_unset(t_av av, t_list **g_env, t_list **g_lenv)
{
	return (bi_unsetenv(av, g_env, g_lenv));
}
