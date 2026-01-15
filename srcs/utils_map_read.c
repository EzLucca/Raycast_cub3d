/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_map_read.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgueon <jgueon@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 21:48:53 by jgueon            #+#    #+#             */
/*   Updated: 2026/01/14 00:47:22 by jgueon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief Compute the length of a map line up to (but not including) '\n'.
 *
 * This is similar to strlen(), but:
 * - It safely handles NULL input (returns 0).
 * - It stops counting at '\n' so lines returned by get_line()/file reads can be
 *   measured without including the newline character.
 *
 * @param s Input string (may be NULL).
 *
 * @return Number of characters before '\n' or '\0'.
 */
int	row_len(char *s)
{
	int	i;

	i = 0;
	while (s && s[i] && s[i] != '\n')
		i++;
	return (i);
}

/**
 * @brief Free a partially allocated grid of rows, then free the grid pointer.
 *
 * This helper is intended for cleanup when grid allocation fails mid-way.
 * It frees rows from index (y - 1) down to 0, then frees the @p grid array.
 *
 * Expected usage:
 * - @p y is the count of successfully allocated rows so far.
 * - Only grid[0..y-1] are assumed valid allocated pointers.
 *
 * @param grid The grid array to free.
 * @param y Number of allocated rows currently stored in @p grid.
 *
 * @return None.
 */
void	free_partial_grid(char **grid, int y)
{
	while (y > 0)
	{
		y--;
		free(grid[y]);
	}
	free(grid);
}
