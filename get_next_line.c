/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 17:54:53 by ssoeno            #+#    #+#             */
/*   Updated: 2024/05/12 21:13:21 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "get_next_line.h"

static char	*read_from_file(char *basin_buffer, int *fd)
{
	char	*cup_buffer;
	int		bytes_read;

	cup_buffer = ft_calloc(BUFFER_SIZE + 1, sizeof(char));
	if (!cup_buffer)
		return (NULL);
	bytes_read = read(*fd, cup_buffer, BUFFER_SIZE);
	if (bytes_read == 0) //EOFが来たシグナルとしてfd=-1を返す
		return (*fd = -1, basin_buffer);
	cup_buffer[bytes_read] = '\0';//read関数はnull終端を行わないため追加
	basin_buffer = ft_strjoin(basin_buffer, cup_buffer);
	while (!ft_strchr(basin_buffer, '\n') && (bytes_read = read(*fd, cup_buffer, BUFFER_SIZE)) > 0)
	{
		cup_buffer[bytes_read] = '\0';//read関数はnull終端を行わないため追加
		basin_buffer = ft_strjoin(basin_buffer, cup_buffer);
		if (ft_strchr(basin_buffer, '\n'))
			break ;
	}
	if (bytes_read == 0) //EOFが来たシグナルとしてfd=-1とする
		*fd = -1;
	free(cup_buffer);
	if (bytes_read == -1)
		return (free(basin_buffer), NULL);
	return (basin_buffer);
}

char	*extract_line(char *buffer)
{
	char	*endl;
	int		line_length;
	char	*line;

	endl = ft_strchr(buffer, '\n');
	if (endl)
		line_length = endl - buffer;
	else
		line_length = ft_strlen(buffer); // No newline, take everything
	line = (char *)ft_calloc(line_length + 1, sizeof(char));
	if (line == NULL)
		return (NULL);
	ft_strlcpy(line, buffer, line_length + 1); //pass the size including the null terminator
	return (line);
}

char	*obtain_remaining(char *buffer)
{
	char	*endl = ft_strchr(buffer, '\n');
	char	*remaining;
	int		remaining_length;

	if (endl){
		remaining_length = ft_strlen(endl + 1);
		remaining = (char *)ft_calloc(remaining_length + 1, sizeof(char));
		if (remaining == NULL)
			return (free(buffer), NULL);
		ft_strlcpy(remaining, endl + 1, remaining_length + 1);
	} else {
		remaining = (char *)ft_calloc(1, sizeof(char));
		if (remaining == NULL)
			return(free(buffer), NULL);
	}
	return (free(buffer), remaining);
}

char	*get_next_line(int fd)
{
	static char	*basin_buffer;
	char		*line;
	char		*temp;

	if (fd < 0 || read(fd, NULL, 0) < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!basin_buffer)
		basin_buffer = ft_calloc(1, sizeof(char));
	while (!ft_strchr(basin_buffer,'\n'))
	{
		temp = read_from_file(basin_buffer, &fd);
		if (!temp || *temp == '\0')
		{
			if (basin_buffer || !*basin_buffer){
				free(basin_buffer);
				return (NULL);
			}
			break;
		}
		basin_buffer = temp;
		if (fd == -1)
			break;
	}
	if (!basin_buffer)
		return (NULL);
	line = extract_line(basin_buffer);
	basin_buffer = obtain_remaining(basin_buffer);
	if(!basin_buffer || !*basin_buffer){
		free(basin_buffer);
		basin_buffer = NULL;
	}
	return (line);
}

// #include <stdio.h>
// #include <fcntl.h>
// __attribute__((destructor))
// static void destructor() {
//     system("leaks -q a.out");
// }

// int main()
// {
// 	int		fd;
// 	char	*next_line;
// 	int		count;

// 	count = 0;
// 	fd = open("example.txt", O_RDONLY);
// 	if (fd == -1)
// 	{
// 		printf("Error opening file");
// 		return (1);
// 	}
// 	while (1)
// 	{
// 		next_line = get_next_line(fd);
// 		if (next_line == NULL)
// 			break ;
// 		count++;
// 		printf("[%d]:%s\n", count, next_line);
// 		free(next_line);
// 		next_line = NULL;
// 	}
// 	close(fd);
// 	return (0);
// }