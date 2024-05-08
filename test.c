#include <unistd.h>
#include "get_next_line.h"

/* Function to append the read buffer data to the partial content (line). */
/* The explorer (get_next_line) adds a scoop of water (read buffer) to */
/* the current cup(line) */
/* and returns the updated line for further exmination */

char	*append_buffer(char *basin_buffer, char *read_buffer)
{
	char	*temp;

	temp = ft_strjoin(basin_buffer, read_buffer);
	free(basin_buffer);
	return (temp);
}


/* Function to read data form the file and append it to partial content */
/* The explorer dips its thimble (buffer) into the aquarium (file) */
/* and retrieves a scoop of water(characters from the file) */
/* The explorer continues scooping until it encounters the fish (new line) or */
/* examines the entire thimble (buffer) */

static char	*read_from_file(char *basin_buffer, int fd)
{
	char	*cup_buffer;
	int		bytes_read;

	cup_buffer = ft_calloc(BUFFER_SIZE + 1, sizeof(char));
	if (!cup_buffer)
		return (NULL);
	while ((bytes_read = read(fd, cup_buffer, BUFFER_SIZE)) > 0)
	//EOFまでループが回る。readがfdからcup_bufferへのデータを読み込む過程は、
	//ファイルの読み込み位置（ファイルポインタ）をもとに動作する。
	//readを連続して呼び出すとファイルポインタが自動的に更新され、読み込んだバイト数
	//だけ前進する。
	{
		cup_buffer[bytes_read] = '\0';//read関数はnull終端を行わないため追加
		basin_buffer = append_buffer(basin_buffer, cup_buffer);
		//cupからbasinに追加した新しい文字列を生成
		//basin_bufferにあった古いメモリを解放後、新しいメモリを割り当てる。
		if (ft_strchr(basin_buffer, '\n'))
			break ; //改行を見つけたらそれ以上読み込む必要がない。
	}
	//EOFが来た時に、whileループが終了、cupが解放される
	free(cup_buffer);
	if (bytes_read == -1)
		return (free(basin_buffer), NULL);
	return (basin_buffer);
}
//read関数がゼロを返す＝EOF

/*  */
char	*extract_line(char *buffer)
{
	char	*endl;
	int		line_length;
	char	*line;

	endl = ft_strchr(buffer, '\n');
	if (endl)
		line_length = endl - buffer + 1; //include the newline character
	else
		line_length = ft_strlen(buffer); // No newline, take everything
	line = (char *)ft_calloc(line_length + 1, sizeof(char));
	if (line == NULL)
		return (NULL);
	ft_strlcpy(line, buffer, line_length + 1); //pass the size including the null terminator
	return (line);
}

/* create a new buffer that starts right after the line that was extracted */
char	*obtain_remaining(char *buffer)
{
	char	*endl = ft_strchr(buffer, '\n'); //最初の改行文字（'\n'）を指すポインタを保持
	char	*remaining;
	int		remaining_length;

	if (endl){
		remaining_length = ft_strlen(endl + 1); //改行の次の文字からの文字数をカウント
		remaining = (char *)ft_calloc(remaining_length + 1, sizeof(char));
		if (remaining == NULL)
			return (free(buffer), NULL);
		ft_strlcpy(remaining, endl + 1, remaining_length + 1); //copy everything after newline
	} else {
		//if no newline is found, allocate memory for the empty string
		remaining = (char *)ft_calloc(1, sizeof(char));
		if (remaining == NULL)
			return(free(buffer), NULL);
	}
	return (free(buffer), remaining);  //free the old buffer
}

/* The get_next_line function to get the next line (fish) form the file descripter */
/* read lines from the aquarium(input file) using the cup (buffer) */
/* aims to find and return fish(line) one at a time without disturbing rather rest of  */
/* the aquarium (file) and avoiding reading the entire file at once. */
/* The explorer also handles edge cases and errors */

char	*get_next_line(int fd)
{
	static char	*basin_buffer;
	char		*line;
	char		*temp;

	if (fd < 0 || read(fd, NULL, 0) < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!basin_buffer)
		basin_buffer = ft_calloc(1, sizeof(char)); //初回読み込み時に初期化
	while (!ft_strchr(basin_buffer,'\n')) //改行が含まれていなければさらにデータを読み込む
	{
		temp = read_from_file(basin_buffer, fd);
		//basin_bufferに少なくとも1つの完全な行が含まれるまで繰り返される
		if (!temp)
			break;
		basin_buffer = temp;
	}
	if (!basin_buffer)
		return (free(basin_buffer), NULL);
	line = extract_line(basin_buffer); //basin_bufferから1行を抽出
	basin_buffer = obtain_remaining(basin_buffer); //抽出後に残った文字列を再度basin_bufferに保存
	if(!*basin_buffer)
		free(basin_buffer);
		basin_buffer = NULL;
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
	int		fd;
	char	*next_line;
	int		count;

	count = 0;
	fd = open("example.txt", O_RDONLY);
	if (fd == -1)
	{
		printf("Error opening file");
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
	return (0);
}
