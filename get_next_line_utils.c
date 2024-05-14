/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 19:16:13 by ssoeno            #+#    #+#             */
/*   Updated: 2024/05/14 19:56:55 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	free((char *)s1);
	//free((char *)s2);
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
