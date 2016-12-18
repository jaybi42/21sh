/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autocomp_utils_1_1.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mseinic <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/05 20:07:25 by mseinic           #+#    #+#             */
/*   Updated: 2016/12/18 19:42:54 by mseinic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "autocomp.h"
#include <sys/stat.h>

void			get_slashes(char **t)
{
	int		i;
	int		len;

	len = 0;
	i = 0;
	while (t[i] != NULL)
	{
		len = ft_strlen(t[i]);
		if (t[i][len - 1] == '/')
			t[i][len - 1] = '\0';
		i++;
	}
}

char			**del_return_null(char ***t)
{
	del_tab(*t);
	return (NULL);
}

void			init_var(int *n, char ***t, char ***tab2, char ***ret)
{
	xmalloc(100);
	*n = 0;
	*t = NULL;
	*tab2 = NULL;
	*ret = NULL;
}

int				auto_my_cmp(char *d_name, char *tmp, char *str)
{
	return (ft_strcmp(d_name, ".") != 0
			&& ft_strcmp(d_name, "..") != 0
			&& ft_strcmp(tmp, str) == 0);
}

int				is_dir_file(const char *path)
{
	struct stat tmp;

	lstat(path, &tmp);
	return (S_ISDIR(tmp.st_mode));
}
