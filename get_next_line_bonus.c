/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktan <ktan@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/12 15:18:20 by ktan              #+#    #+#             */
/*   Updated: 2023/02/04 01:15:23 by ktan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static char	*ft_strjoin(char *s1, char *s2)
{
	char	*join;
	size_t	len;
	char	*tmp;

	tmp = s1;
	len = ft_strlen(s1) + ft_strlen(s2);
	join = malloc(sizeof(char) *(len + 1));
	if (!join)
		return (0);
	len = 0;
	while (*s1)
		join[len++] = *s1++;
	while (*s2)
		join[len++] = *s2++;
	join[len] = '\0';
	ft_free((void **)&tmp);
	return (join);
}

static char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*dup;
	size_t	sublen;

	dup = malloc(sizeof(char) * (len + 1));
	if (!dup)
		return (NULL);
	sublen = 0;
	while (len-- && *(s + start))
		dup[sublen++] = *(s++ + start);
	dup[sublen] = '\0';
	return (dup);
}

static char	*gnlret(char **str, char **buf, int mode)
{
	char	*ret;
	char	*tmp;
	char	*nl;
	int		nllen;

	if (mode != 2)
		ft_free((void **)buf);
	if (!mode || !*str)
		return (0);
	nl = ft_strchr(*str, '\n');
	if (!nl || !nl[1])
	{
		ret = ft_substr(*str, 0, ft_strlen(*str));
		ft_free((void **)str);
		*str = 0;
		return (ret);
	}
	nllen = ft_strlen(nl);
	tmp = *str;
	ret = ft_substr(*str, 0, ft_strlen(*str) - nllen + 1);
	*str = ft_substr(nl, 1, nllen - 1);
	ft_free((void **)&tmp);
	return (ret);
}

char	*get_next_line(int fd)
{
	static char	*str[_SC_OPEN_MAX];
	char		*buf;
	int			readval;

	if (str[fd] && ft_strchr(str[fd], '\n'))
		return (gnlret(str + fd, 0, 2));
	buf = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buf || BUFFER_SIZE < 1 || fd < 0 || read(fd, buf, 0) == -1)
		return (gnlret(str + fd, &buf, 0));
	readval = read(fd, buf, BUFFER_SIZE);
	while (readval > 0)
	{
		buf[readval] = 0;
		if (!str[fd])
			str[fd] = ft_substr(buf, 0, readval);
		else
			str[fd] = ft_strjoin(str[fd], buf);
		if (ft_strchr(buf, '\n'))
			break ;
		readval = read(fd, buf, BUFFER_SIZE);
	}
	return (gnlret(str + fd, &buf, 1));
}
