/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 20:02:36 by ssoeno            #+#    #+#             */
/*   Updated: 2024/05/14 21:00:42 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "get_next_line.h"

void	read_from_file(char **basin, int fd)
{
	char	*cup;
	int		red;

	if (!*basin)
		*basin = ft_calloc(1, 1);
	cup = ft_calloc(BUFFER_SIZE + 1, sizeof(char));
	if (!cup)
	{
		free(*basin);
		*basin = NULL;
		return ;
	}
	red = read(fd, cup, BUFFER_SIZE);
	while (red > 0)
	{
		cup[red] = '\0';
		*basin = ft_strjoin(*basin, cup);
		if (ft_strchr(*basin, '\n'))
			break ;
		red = read(fd, cup, BUFFER_SIZE);
	}
	free(cup);
	return ;
}

char	*extract_line(char *buffer)
{
	char	*endl;
	int		line_length;
	char	*line;

	if (!*buffer)
		return (NULL);
	endl = ft_strchr(buffer, '\n');
	if (endl)
		line_length = endl - buffer;
	else
		line_length = ft_strlen(buffer);
	line = (char *)ft_calloc(line_length + 1, sizeof(char));
	if (line == NULL)
		return (NULL);
	ft_strlcpy(line, buffer, line_length + 1);
	return (line);
}

char	*obtain_remaining(char *buffer)
{
	char	*endl;
	char	*remaining;
	int		remaining_length;

	endl = ft_strchr(buffer, '\n');
	if (!endl)
	{
		free(buffer);
		return (NULL);
	}
	else
	{
		remaining_length = ft_strlen(endl + 1);
		remaining = (char *)ft_calloc(remaining_length + 1, sizeof(char));
		// if (remaining == NULL)
		// 	return (free(buffer), NULL);
		ft_strlcpy(remaining, endl + 1, remaining_length + 1);
	}
	return (free(buffer), remaining);
}

char	*get_next_line(int fd)
{
	char		*line;
	static char	*basin;

	if (fd < 0 || read(fd, NULL, 0) < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!basin)
		basin = ft_calloc(1, sizeof(char));
	read_from_file(&basin, fd);
	if (!basin)
		return (NULL);
	line = extract_line(basin);
	basin = obtain_remaining(basin);
	if (!line && !basin)
	{
		free(basin);
		basin = NULL;
		return (NULL);
	}
	return (line);
}
#include <stdio.h>
#include <fcntl.h>
__attribute__((destructor))
static void destructor() {
    system("leaks -q a.out");
}

int main()
{
	int     fd;
	char    *next_line;
	int     count;

	count = 0;
	fd = open("example.txt", O_RDONLY);
	if (fd == -1)
	{
		printf("%s", "Error opening file");
		return (1);
	}
	while (1)
	{
		next_line = get_next_line(fd);
		if (next_line == NULL)
			break ;
		count++;
		printf("[%d]:%s\n", count, next_line);
		free(next_line);
		next_line = NULL;
	}
	close(fd);
	// system("leaks a.out");
	return (0);
}