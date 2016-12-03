#include "21sh.h"
#include <stdlib.h>

/*
** - clean screen
*/
int exec_glob(int argc, char **argv)
{
  int i;
  char **t;
  char **arg;

  if (argc < 3)
  {
    ft_printf("{red}usage{eoc}: glob \'string*\' \'string1\' \'string2\' ...\n");
    return (0);
  }
  arg = argv + 2;
  t = ft_globing(argv[1], arg);
  i = -1;
  while (t[++i]);
  ft_printf("find {green}%d{eoc} result%c:\n", i, (i > 1) ? 's' : '\0');
  i = -1;
  while(t[++i])
    ft_printf("[%d] \'%s\'\n", i, t[i]);
  return (0);
}

int			bi_glob(t_av av, t_list **g_env, t_list **l_env)
{
	(void)g_env;
	(void)av;
	(void)l_env;
  return (exec_glob(av.argc + 1, av.argv));
}
