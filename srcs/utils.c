/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgueon <jgueon@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 18:06:36 by jgueon            #+#    #+#             */
/*   Updated: 2026/01/14 17:08:57 by jgueon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief Skip leading whitespace characters in a string.
 *
 * This helper advances the pointer past leading whitespace so parsing can
 * accept lines with indentation (e.g., "   F 0,0,255").
 *
 * Characters skipped include:
 * - space ' '
 * - tab '\t'
 * - carriage return '\r' (useful for Windows CRLF handling)
 * - vertical tab '\v'
 * - form feed '\f'
 *
 * @param s Input string pointer (must be non-NULL).
 *
 * @return Pointer to the first non-whitespace character in @p s.
 */
char	*skip_spaces(char *s)
{
	while (*s == ' ' || *s == '\t' || *s == '\r' || *s == '\v' || *s == '\f')
		s++;
	return (s);
}

/**
 * @brief Free a NULL-terminated array of strings (char **).
 *
 * This function frees each string in the array, then frees the array itself.
 * It safely does nothing if @p colors is NULL.
 *
 * @param colors NULL-terminated array of allocated strings.
 *
 * @return None.
 */
void	free_split(char **colors)
{
	int	i;

	if (!colors)
		return ;
	i = 0;
	while (colors[i])
	{
		free(colors[i]);
		i++;
	}
	free(colors);
}

/**
 * @brief Check whether a string is a valid signed integer representation.
 *
 * Accepted format:
 * - Optional leading '+' or '-'
 * - Followed by one or more digits only
 *
 * Rejected examples:
 * - "" (empty)
 * - "+" / "-" alone
 * - "12a3"
 * - " 42" (leading space is not allowed here; use skip_spaces() before)
 *
 * @param s Input string to validate.
 *
 * @return 1 if @p s is a valid signed number string, 0 otherwise.
 */
int	is_signed_number(char *s)
{
	int	i;

	i = 0;
	if (!s || s[0] == '\0')
		return (0);
	if (s[i] == '+' || s[i] == '-')
		i++;
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

/**
 * @brief Count the number of elements in a NULL-terminated string array.
 *
 * This function counts how many non-NULL entries exist before the terminating
 * NULL pointer.
 *
 * @param arr NULL-terminated array of strings (may be NULL).
 *
 * @return Number of elements in @p arr (0 if @p arr is NULL).
 */
int	arrlen(char **arr)
{
	int	len;

	len = 0;
	if (!arr)
		return (0);
	while (arr[len])
		len++;
	return (len);
}

/**
 * @brief Free a NULL-terminated map/grid (char **).
 *
 * This function frees each row string, then frees the array pointer itself.
 * It safely handles a NULL input pointer.
 *
 * @param map NULL-terminated array representing a grid/map.
 *
 * @return None.
 */
void	free_map(char **map)
{
	int	i;

	i = 0;
	while (map && map[i])
	{
		free(map[i]);
		i++;
	}
	free(map);
}
