#include <unistd.h>
#include "get_next_line.h"

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

void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char	*ptr;

	if (b == NULL)
		return (NULL);
	if (len == 0)
		return (b);
	ptr = (unsigned char *)b;
	while (len-- > 0)
		*ptr++ = (unsigned char)c;
	return (b);
}

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*array;

	if (nmemb && size > SIZE_MAX / nmemb)
		return (NULL);
	array = (void *)malloc(nmemb * size);
	if (array == NULL)
		return (NULL);
	ft_memset(array, 0, (nmemb * size));
	return (array);
}

/* Function to convert and print the newline character as ? */
void	print_newline_helper(char *buffer)
{
	while (*buffer && *buffer != '\0')
	{
		if (*buffer == '\n')
			*buffer = '\\';
		printf("%c", *buffer);
		buffer++;
	}
}
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
	bytes_read = 1;
	while (bytes_read > 0)
	{
		bytes_read = read(fd, cup_buffer, BUFFER_SIZE);
		if(bytes_read == -1)
			return (free(cup_buffer), NULL);
		cup_buffer[bytes_read] = '\0';
		basin_buffer = append_buffer(basin_buffer, cup_buffer);
		if (ft_strchr(basin_buffer, '\n'))
			break ;
		free (cup_buffer);
		return (basin_buffer);
	}
	read(fd, cup_buffer, BUFFER_SIZE);
	print_newline_helper(cup_buffer);
	if (bytes_read <= 0)
		return (free(cup_buffer), NULL);
	return (cup_buffer);
}

/*  */
char	*extract_line(char *buffer)
{
	char *new_line = ft_strchr(buffer, '\n');
	int	line_length;
	char	*line;

	if (new_line){
		line_length = new_line - buffer + 1; //include the newline character
	} else {
		line_length = ft_strlen(buffer); // No newline, take everything
	}
	line = (char *)malloc(sizeof(char) * (line_length + 1));
	if (line == NULL)
		return (NULL);
	strncpy(line, buffer, line_length);
	line[line_length] = '\0';
	return (line);
}

/* create a new buffer that starts right after the line that was extracted */
char	*obtain_remaining(char *buffer)
{
	char	*newline = ft_strchr(buffer, '\n');
	char	*remaining;
	int		remaining_length;

	if (newline){
		remaining_length = ft_strlen(buffer) - (newline - buffer + 1);
		remaining = (char *)malloc(sizeof(char) * remaining_length + 1));
		if (remaining == NULL)
			return NULL;
		strcpy(remaining, newline + 1); //copy everything after newline
	} else {
		//if no newline is found, return an empty string
		remaining = (char *)malloc(sizeof(char));
		if (remaining == NULL)
			return NULL;
			*remaining = '\0';
	}
	free(buffer); //free the old buffer
	return remaining;
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
	// static int	count = 1;

	// printf("ft_calloc#[%d]---", count++);
	if (fd < 0 || read(fd, NULL, 0) < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!basin_buffer)
		basin_buffer = ft_calloc(1, sizeof(char));
	if (!ft_strchr(basin_buffer,'\n'))
		basin_buffer = read_from_file(basin_buffer, fd);
	if (!basin_buffer)
		return (free(basin_buffer), NULL);
	line = extract_line(basin_buffer);
	basin_buffer = obtain_remaining(basin_buffer);
	return (line);
}


#include <stdio.h>
#include <fcntl.h>

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
