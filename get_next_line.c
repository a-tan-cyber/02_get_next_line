/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 13:53:55 by amtan             #+#    #+#             */
/*   Updated: 2025/12/07 19:42:52 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*read_to_stash(int fd, char *stash);
static char	*extract_line(char *stash);
static char	*stash_remainder(char *stash);

char	*get_next_line(int fd)
{
	static char	*stash;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	stash = read_to_stash(fd, stash);
	if (!stash)
		return (NULL);
	line = extract_line(stash);
	if (!line)
	{
		stash = free_and_ret_null(stash);
		return (NULL);
	}
	stash = stash_remainder(stash);
	return (line);
}

static char	*read_to_stash(int fd, char *stash)
{
	char	*buffer;
	ssize_t	rd;

	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (free_and_ret_null(stash));
	rd = 1;
	while (rd > 0 && !gnl_strchr(stash, '\n'))
	{
		rd = read(fd, buffer, BUFFER_SIZE);
		if (rd < 0)
		{
			free(buffer);
			return (free_and_ret_null(stash));
		}
		buffer[rd] = '\0';
		stash = gnl_strjoin(stash, buffer);
		if (!stash)
		{
			free(buffer);
			return (NULL);
		}
	}
	free(buffer);
	return (stash);
}

static char	*extract_line(char *stash)
{
	size_t	i;
	char	*line;

	if (!stash || !*stash)
		return (NULL);
	i = 0;
	while (stash[i] && stash[i] != '\n')
		i++;
	if (stash[i] == '\n')
		i++;
	line = malloc(i + 1);
	if (!line)
		return (NULL);
	ft_memcpy(line, stash, i);
	line[i] = '\0';
	return (line);
}

static char	*stash_remainder(char *stash)
{
	size_t	i;
	size_t	len;
	char	*new_stash;

	if (!stash)
		return (NULL);
	i = 0;
	while (stash[i] && stash[i] != '\n')
		i++;
	if (!stash[i])
		return (free_and_ret_null(stash));
	i++;
	len = gnl_strlen(stash + i);
	new_stash = malloc(len + 1);
	if (!new_stash)
		return (free_and_ret_null(stash));
	ft_memcpy(new_stash, stash + i, len);
	new_stash[len] = '\0';
	free(stash);
	return (new_stash);
}
