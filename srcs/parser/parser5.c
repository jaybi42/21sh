#include "21sh.h"

int in_cmd_is_intouchable(t_av *cmd, int id_argv, int i)
{
	if (cmd->argv_auth[id_argv][i] == 1)
		return (0);
	return (1);
}

void convert_other(t_av ***cmds)
{
	int i;

	i = 0;
	while((*cmds)[i])
	{
		get_redirect(&(*cmds)[i]);
		i++;
	}
}
