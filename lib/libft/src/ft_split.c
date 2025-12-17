/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgazur <bgazur@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 16:19:25 by bgazur            #+#    #+#             */
/*   Updated: 2025/07/27 12:22:32 by bgazur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	count_words(char const *s, char c);
static char		**allocate_array(char const *s, char c, char **arr);
static void		free_array(char **arr, size_t i);

char	**ft_split(char const *s, char c)
{
	char	**arr;
	size_t	word_count;

	if (!s)
		return (NULL);
	word_count = count_words(s, c);
	arr = malloc(sizeof(char *) * (word_count + 1));
	if (!arr)
		return (NULL);
	arr = allocate_array(s, c, arr);
	if (!arr)
		return (NULL);
	arr[word_count] = NULL;
	return (arr);
}

// Counts how many words to split the string into.
static size_t	count_words(char const *s, char c)
{
	size_t	count;

	count = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
		{
			count++;
			while (*s && *s != c)
				s++;
		}
	}
	return (count);
}

// Allocates each single word into its array.
static char	**allocate_array(char const *s, char c, char **arr)
{
	size_t	i;
	size_t	word_len;

	i = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
		{
			if (ft_strchr(s, c))
				word_len = ft_strchr(s, c) - s;
			else
				word_len = ft_strlen(s);
			arr[i] = ft_substr(s, 0, word_len);
			if (!arr[i])
			{
				free_array(arr, i);
				return (NULL);
			}
			s += word_len;
			i++;
		}
	}
	return (arr);
}

// Frees all allocated arrays.
static void	free_array(char **arr, size_t i)
{
	while (i > 0)
	{
		free(arr[i]);
		arr[i] = NULL;
		i--;
	}
	free(arr[i]);
	arr[i] = NULL;
	free(arr);
	arr = NULL;
}
