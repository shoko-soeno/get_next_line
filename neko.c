#include <unistd.h>
#include "get_next_line.h"

void	read_from_file(char **basin, int fd)
{
	char	*cup;
	int		red;

	cup = ft_calloc(BUFFER_SIZE + 1, sizeof(char));
	if (!cup){
		free(*basin);
		*basin = NULL;
		return ;
	}
	red = read(fd, cup, BUFFER_SIZE);
	// if (!*basin)
	// 	*basin = ft_calloc(1,1);
	// cup[red] = '\0';//read関数はnull終端を行わないため追加
	// *basin = ft_strjoin(*basin, cup);
	while (red > 0)
	{
		cup[red] = '\0';//read関数はnull終端を行わないため追加
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
	char	*endl;
	char	*remaining;
	int		remaining_length;

	endl = ft_strchr(buffer, '\n');
	if (endl)
	{
		remaining_length = ft_strlen(endl + 1);
		remaining = (char *)ft_calloc(remaining_length + 1, sizeof(char));
		if (remaining == NULL)
			return (free(buffer), NULL);
		ft_strlcpy(remaining, endl + 1, remaining_length + 1); //copy everything after endl
	} else {
		//if no newline is found, allocate memory for the empty string
		remaining = (char *)ft_calloc(1, sizeof(char));
		if (remaining == NULL)
			return(free(buffer), NULL);
	}
	return (free(buffer), remaining);
}

char	*get_next_line(int fd)
{
	static char	*basin;
	char		*line;
	char		*cup;

	if (fd < 0 || read(fd, NULL, 0) < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	// if (!basin)
	// 	basin = ft_calloc(1, sizeof(char)); //初回読み込み時に初期化
	cup = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if(!tmp)
		retrun (0);
	while (!ft_strchr(basin,'\n')) //改行が含まれていなければデータを読み込む
	{
		read_from_file(&basin, fd);
		if (!basin || !*basin)
			return (free(basin), NULL);
	}
	if (!basin)
		return (NULL);
	line = extract_line(basin);
	basin = obtain_remaining(basin);
	if(!basin || !*basin){
		free(basin);
		basin = NULL;
	}
	printf("basin;%s\n", basin);
	printf("line;%s\n", line);
	return (line);
}

// #include <stdio.h>
// #include <fcntl.h>
// __attribute__((destructor))
// static void destructor() {
//     system("leaks -q a.out");
// }

int main()
{
	int		fd;
	char	*next_line;
	int		count;

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