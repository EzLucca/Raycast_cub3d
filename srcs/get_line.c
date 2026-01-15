/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_line.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgueon <jgueon@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 18:36:47 by jgueon            #+#    #+#             */
/*   Updated: 2026/01/13 19:45:47 by jgueon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief Check if a string contains a newline character.
 *
 * This helper scans the given C-string and returns true as soon as it finds
 * a '\n'. It is used by get_line() to know whether the currently accumulated
 * content already contains a full line.
 *
 * @param s The string to scan (must be non-NULL).
 *
 * @return 1 if a newline is found, 0 otherwise.
 */
static int	has_nl(const char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}

/**
 * @brief Append a buffer chunk to an existing line accumulator.
 *
 * If @p line is NULL, this function duplicates @p buf and returns it.
 * Otherwise, it joins @p line and @p buf into a new allocated string,
 * frees the old @p line, and returns the newly allocated result.
 *
 * This is used by get_line() while reading from the file descriptor.
 *
 * @param line Current accumulated content (may be NULL).
 * @param buf Newly read chunk to append (must be a valid C-string).
 *
 * @return Newly allocated concatenated string, or NULL on allocation failure.
 */
static char	*append_buf(char *line, char *buf)
{
	char	*new_line;

	if (!line)
		return (ft_strdup(buf));
	new_line = ft_strjoin(line, buf);
	free(line);
	return (new_line);
}

/**
 * @brief Extract one line from accumulated content and update the static
 * 	buffer.
 *
 * This function takes the full accumulated @p content (which may contain a
 * newline) and:
 * - Extracts the substring up to (but not including) '\n' as the returned line.
 * - Copies the remainder after the newline into @p buf (the persistent buffer),
 *   so it will be used on the next get_line() call.
 * - Frees @p content before returning.
 *
 * Notes about behavior:
 * - If @p content is NULL or empty, it frees it (if needed) and returns NULL.
 * - The returned line does not include the '\n' character.
 *
 * @param content Full accumulated content (allocated string).
 * @param buf Persistent buffer to store leftover data for the next call.
 *
 * @return Newly allocated extracted line, or NULL if there is nothing to return
 *         or if allocation fails.
 */
static char	*extract_line(char *content, char *buf)
{
	int		i;
	char	*line;

	if (!content || content[0] == '\0')
		return (free(content), NULL);
	i = 0;
	while (content[i] && content[i] != '\n')
		i++;
	line = ft_substr(content, 0, i);
	if (!line)
		return (free(content), NULL);
	if (content[i] == '\n')
		i++;
	ft_strlcpy(buf, content + i, BUFFER_SIZE + 1);
	free(content);
	return (line);
}

/**
 * @brief Reset the internal static buffer used by get_line().
 *
 * This calls get_line(-1), which is treated as a special case by get_line():
 * it clears the static buffer and returns NULL. This is useful when switching
 * file descriptors or restarting a parsing process.
 *
 * @return None.
 */
void	get_line_reset(void)
{
	char	*buf;

	buf = get_line(-1);
	(void)buf;
}

/**
 * @brief Read and return the next line from a file descriptor with newline
 * 	removed.
 *
 * This function behaves similarly to a simple get-next-line:
 * - It keeps a static internal buffer that stores leftover data between calls.
 * - It accumulates data into a dynamic string until a '\n' is present or read()
 *   returns 0/negative.
 * - It returns the next line extracted from the accumulated content
 * 	(without '\n').
 * - Any remaining data after the newline is kept for the next call.
 *
 * Special case:
 * - If @p fd is negative, the internal static buffer is cleared and NULL is
 * 	returned.
 *
 * Allocation/IO behavior:
 * - Returns NULL on allocation failure.
 * - When EOF is reached, it returns the final remaining line if any;
 * 	otherwise NULL.
 *
 * @param fd File descriptor to read from.
 *
 * @return A newly allocated line (without '\n'), or NULL on EOF/error/reset.
 */
char	*get_line(int fd)
{
	static char	buf[BUFFER_SIZE + 1];
	char		*content;
	char		tmp[BUFFER_SIZE + 1];
	ssize_t		r;

	if (fd < 0)
	{
		buf[0] = '\0';
		return (NULL);
	}
	content = ft_strdup(buf);
	if (!content)
		return (NULL);
	while (!has_nl(content))
	{
		r = read(fd, tmp, BUFFER_SIZE);
		if (r <= 0)
			break ;
		tmp[r] = '\0';
		content = append_buf(content, tmp);
		if (!content)
			return (NULL);
	}
	return (extract_line(content, buf));
}
