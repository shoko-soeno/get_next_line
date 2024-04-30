/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 17:54:53 by ssoeno            #+#    #+#             */
/*   Updated: 2024/04/30 16:20:25 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>
# define BUFFER_SIZE 1000

// #include "get_next_line.h"

size_t	ft_strlen(const char *str)
{
	size_t	len;

	if (!str)
		return (0);
	len = 0;
	while (str[len] != '\0')
	{
		len++;
	}
	return (len);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*result;
	char	*original_address;

	if (!s1 || !s2)
		return (NULL);
	result = (char *)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!result)
		return (NULL);
	original_address = result;
	while (*s1)
		*result++ = *s1++;
	while (*s2)
		*result++ = *s2++;
	*result = '\0';
	return (original_address);
}

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	dst_len;
	size_t	i;

	dst_len = 0;
	if (dst)
		dst_len = ft_strlen(dst);
	if (dstsize <= dst_len || dstsize == 0)
		return (dstsize + ft_strlen(src));
	i = 0;
	while (src[i] != '\0' && dst_len + i < dstsize - 1)
	{
		dst[dst_len + i] = src[i];
		i++;
	}
	if (dst_len < dstsize)
		dst[dst_len + i] = '\0';
	return (dst_len + ft_strlen(src));
}

char	*ft_strchr(const char *s, int c)
{
	char	ch;

	if (!s)
		return (NULL);
	ch = (char)c;
	while (*s != ch)
	{
		if (*s == '\0')
			return (NULL);
		s++;
	}
	return ((char *)s);
}

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	src_len;

	i = 0;
	src_len = ft_strlen(src);
	if (dstsize != 0)
	{
		while (src [i] != '\0' && i < (dstsize - 1))
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
	}
	return (src_len);
}

char	*ft_strdup(const char *src)
{
	size_t	i;
	size_t	len;
	char	*dup;

	len = ft_strlen(src);
	dup = (char *)malloc((len + 1) * sizeof(char));
	if (!dup)
		return (NULL);
	i = -1;
	while (++i <= len)
		dup[i] = src[i];
	return (dup);
}

char	*ft_strndup(const char *src, size_t n)
{
	size_t	i;
	char	*dup;

	if (!src)
		return (NULL);
	dup = (char *)malloc((n + 1) * sizeof(char));
	if (!dup)
		return (NULL);
	i = 0;
	while (i < n && src[i])
	{
		dup[i] = src[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

bool	read_to_buffersize(int fd, char *buffer)
{
	size_t	read_bytes;
	char	*temp_buffer;
	char	*new_buffer;
	
	read_bytes = read(fd, temp_buffer, BUFFER_SIZE);
	if (read_bytes <= 0)
		return(false);
	temp_buffer[read_bytes] = '\0';
	if(buffer)
	{
		new_buffer = ft_strjoin(buffer, temp_buffer);
	} else {
		new_buffer = ft_strdup(temp_buffer);
	}
	if (!new_buffer)
		return (false);
	*buffer = *new_buffer;
	return (true);
}

char	*update_line (char **buffer)
{
	char	*line; //バッファ内のデータを先頭から改行文字 (\n) まで、またはバッファの終端までの文字列を格納
	char	*leftover_str; //抽出された行の次に続くバッファの残りの部分
	char	*endl_position;
	int		i;

	if(!*buffer || **buffer == '\0')
		return (NULL);
	while ((*buffer)[i] && (*buffer)[i] != '\n')
		i++;
	if ((*buffer)[i] == '\n')
	{
		line = ft_strndup(*buffer, i + 1); //改行を含めた文字をコピー
		leftover_str = ft_strdup(endl_position + i + 1); //改行の次の文字からコピー
	} else { //改行がないがデータが残っている場合
		line = ft_strdup(*buffer);
		leftover_str = NULL;
	}
	free(*buffer); //古いbufferを解放
	*buffer = leftover_str; //leftoverで置き換え
	return (line);
}

char	*get_next_line(int fd)
{
	static char	*buffer;
	char		*next_line;

	buffer = NULL;
	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, NULL, 0) < 0)
		return (NULL);
	while (true)
	{
		if (!(next_line = update_line(&buffer)))
			return next_line;
		if (!(read_to_buffersize(fd, buffer)))
			break;
	}
	if (!buffer || *buffer == '\0')
		return (NULL);
	next_line = ft_strdup(buffer);
	free(buffer);
	buffer = NULL; //ポインタがどこも指さない状態（ヌルポインタ）にする。
	return (next_line);
}

#include <fcntl.h>
#include <stdio.h>
int	main(void)
{
	char	*line;
	int		i;
	int		fd1;
	int		fd2;
	int		fd3;
	fd1 = open("tests/test.txt", O_RDONLY);
	fd2 = open("tests/test2.txt", O_RDONLY);
	fd3 = open("tests/test3.txt", O_RDONLY);
	i = 1;
	while (i < 7)
	{
		line = get_next_line(fd1);
		printf("line [%02d]: %s", i, line);
		free(line);
		line = get_next_line(fd2);
		printf("line [%02d]: %s", i, line);
		free(line);
		line = get_next_line(fd3);
		printf("line [%02d]: %s", i, line);
		free(line);
		i++;
	}
	close(fd1);
	close(fd2);
	close(fd3);
	return (0);
}