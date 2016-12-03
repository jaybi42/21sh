#include "21sh.h"
#include <stdlib.h>

/*
** - clean screen
*/
typedef struct s_info
{
  char *name;
  char *info;
}             t_info;
/*
** !!!!!!! DON'T ADD: %s in your info or any %*
*/
#define LEN_INFOS 1

t_info g_infos[LEN_INFOS] = {
{"|",
"{green}#definition{eoc}\n\
\'|\' is called  a \'pipe\'. it's a specific separator between two command,\n\
the format is: \"cmd1 | cmd2\"\n\
\n\
{green}#how it works?{eoc}\n\
the output (stdout{red}(?){eoc}) of the cmd1 will be put in the stdin\
{red}(?){eoc} of the cmd2.\n\
\n\
{green}#examples{eoc}\n\
$>echo \"ok\" | cat -e <ENTER>\n\
ok$\n\
"
}
};

int exec_42info(int ac, char **argv)
{
  int i;

  if (ac == 0)
  {
    ft_dprintf(2, "usage: 42info \'?\'\n");
    return (0);
  }
  i = -1;
  while (++i < LEN_INFOS)
    if (ft_strcmp(g_infos[i].name, argv[1]) == 0)
    {
      ft_printf(g_infos[i].info);
      return (0);
    }
  ft_printf("didn't find '%s'!\n", argv[1]);
  return (0);
}

int			bi_42info(t_av av, t_list **g_env, t_list **l_env)
{
	(void)g_env;
	(void)av;
	(void)l_env;
	return (exec_42info(av.argc, av.argv));
}
