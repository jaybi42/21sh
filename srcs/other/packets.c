#include "21sh.h"

/*
**
*/
t_sf *create_packet(char *b, int len)
{
    t_sf *tmp;

    if (len > WRITING)
    {
      (g_debug) ? ft_dprintf(2, "{red}code error, sending too big packet{eoc}\n") : 0;
      return (NULL);
    }
    tmp = xmalloc(sizeof(t_sf));
    if (!tmp)
      return (NULL);
    strncpy((char *)tmp->s, b, (size_t)len);
    tmp->s[len] = '\0';
    tmp->len = len;
    tmp->next = NULL;
    return tmp;
}
