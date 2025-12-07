/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 13:54:20 by amtan             #+#    #+#             */
/*   Updated: 2025/12/07 19:24:49 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*free_and_ret_null(char *stash)
{
	free(stash);
	return (NULL);
}

size_t	gnl_strlen(char *s)
{
	size_t	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i])
		i++;
	return (i);
}

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char		*p_dest;
	const unsigned char	*p_src;
	size_t				i;

	p_dest = (unsigned char *)dest;
	p_src = (const unsigned char *)src;
	i = 0;
	while (i < n)
	{
		p_dest[i] = p_src[i];
		i++;
	}
	return (dest);
}

char	*gnl_strjoin(char *stash, char *buffer)
{
	char	*new_stash;
	size_t	stash_len;
	size_t	buffer_len;

	stash_len = gnl_strlen(stash);
	buffer_len = gnl_strlen(buffer);
	new_stash = malloc(stash_len + buffer_len + 1);
	if (!new_stash)
		return (free_and_ret_null(stash));
	if (stash_len > 0)
		ft_memcpy(new_stash, stash, stash_len);
	ft_memcpy(new_stash + stash_len, buffer, buffer_len);
	new_stash[stash_len + buffer_len] = '\0';
	free(stash);
	return (new_stash);
}

char	*gnl_strchr(char *s, char c)
{
	if (!s)
		return (NULL);
	while (*s)
	{
		if (*s == c)
			return (s);
		s++;
	}
	if (!c)
		return (s);
	return (NULL);
}
