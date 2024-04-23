/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfararan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 11:58:29 by nfararan          #+#    #+#             */
/*   Updated: 2024/04/23 11:52:12 by nfararan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

/*
 *  Get the current line until found "\n" or EOF 
*/
static char	*get_line(int fd, char **rest)
{
	char	*buf;
	char	*line;
	int		read_bytes;

	buf = (char *)ft_calloc(BUFFER_SIZE + 1, 1);
	if (!buf)
		return (NULL);
	line = NULL;
	if (*rest)
	{
		line = ft_strjoin(line, *rest);
		free(*rest);
		*rest = NULL;
	}
	read_bytes = 1;
	while (read_bytes && !ft_strchr(buf, '\n'))
	{
		read_bytes = read(fd, buf, BUFFER_SIZE);
		if (read_bytes <= -1)
			return (free(buf), free(line), NULL);
		buf[read_bytes] = 0;
		line = ft_strjoin(line, buf);
	}
	free(buf);
	return (line);
}

/*
 * Get the left side of the line separated by '\n
 * i + 2 => \0 + \n 
*/
static char	*get_left(char *line)
{
	int		i;
	int		j;
	char	*left;

	if (*line == 0)
		return (NULL);
	if (!ft_strchr(line, '\n'))
		return (ft_strdup(line));
	i = 0;
	while (line[i] && line[i] != '\n')
		i++;
	left = (char *)ft_calloc(i + 2, 1);
	if (!left)
		return (NULL);
	j = 0;
	while (j < i + 1)
	{
		left[j] = line[j];
		j++;
	}
	return (left);
}

/*
 * Get the right side of the line separated by \n 
 * none need +1 at length count because we already
 * have \n at the place of \0
*/
static char	*get_right(char *line)
{
	char	*right;
	int		i;
	int		j;

	if (!ft_strchr(line, '\n'))
		return (free(line), NULL);
	right = (char *)ft_calloc(ft_strlen(ft_strchr(line, '\n')), 1);
	if (!right)
		return (NULL);
	i = 0;
	j = 0;
	while (line[i] && line[i] != '\n')
		i++;
	i++;
	while (line[i])
		right[j++] = line[i++];
	free(line);
	return (right);
}

char	*get_next_line(int fd)
{
	char		*line;
	char		*left;
	static char	*rest[1024];

	left = NULL;
	if (fd < 0 || fd > 1024 || BUFFER_SIZE <= 0)
		return (NULL);
	line = get_line(fd, &rest[fd]);
	if (!line)
		return (NULL);
	left = get_left(line);
	rest[fd] = get_right(line);
	return (left);
}
