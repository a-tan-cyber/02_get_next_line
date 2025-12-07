/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 13:54:06 by amtan             #+#    #+#             */
/*   Updated: 2025/12/07 19:24:53 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

char	*get_next_line(int fd);
char	*free_and_ret_null(char *stash);
size_t	gnl_strlen(char *s);
void	*ft_memcpy(void *dest, const void *src, size_t n);
char	*gnl_strjoin(char *stash, char *buffer);
char	*gnl_strchr(char *s, char c);

#endif