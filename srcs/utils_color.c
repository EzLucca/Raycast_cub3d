/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_color.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgueon <jgueon@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 21:27:19 by jgueon            #+#    #+#             */
/*   Updated: 2026/01/14 00:45:49 by jgueon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief Count how many comma characters are present in a string.
 *
 * This helper is used when validating RGB lines to ensure the format contains
 * the expected number of commas (typically exactly 2 for "R,G,B").
 *
 * If @p line is NULL or empty, it returns 0.
 *
 * @param line Input string to scan.
 *
 * @return Number of commas found in @p line.
 */
int	get_nb_comma(char *line)
{
	int	n;

	n = 0;
	if (!line || !*line)
		return (n);
	while (*line)
	{
		if (*line == ',')
			n++;
		line++;
	}
	return (n);
}

/**
 * @brief Check whether an integer is within the valid RGB component range.
 *
 * Valid RGB component values are from 0 to 255 inclusive.
 *
 * @param i The integer value to validate.
 *
 * @return 1 if @p i is between 0 and 255 inclusive, 0 otherwise.
 */
int	check_rgb_range(int i)
{
	return ((i >= 0) && (i <= 255));
}

/**
 * @brief Check whether a t_color already has been assigned a valid RGB.
 *
 * In this project, an “unset” color is represented as (-1, -1, -1).
 * This function returns true only when all 3 components are not -1.
 *
 * @param c The color struct to test.
 *
 * @return 1 if the color is set, 0 if it is still unset.
 */
int	is_color_set(t_color c)
{
	return (c.r != -1 && c.g != -1 && c.b != -1);
}

/**
 * @brief Store a parsed RGB triplet into the game floor or ceiling color.
 *
 * If @p id is 'F', it stores the 3 values into game->floor.
 * Otherwise, it stores them into game->ceiling.
 *
 * This is the step that turns an “unset” color (-1,-1,-1) into an actual
 * parsed color used later by the renderer.
 *
 * @param game Game context containing floor/ceiling colors.
 * @param id Identifier selecting which color to store ('F' or 'C').
 * @param tmp Array of 3 integers containing {R, G, B}.
 *
 * @return None.
 */
void	store_color(t_game *game, char id, int *tmp)
{
	if (id == 'F')
	{
		game->floor.r = tmp[0];
		game->floor.g = tmp[1];
		game->floor.b = tmp[2];
	}
	else
	{
		game->ceiling.r = tmp[0];
		game->ceiling.g = tmp[1];
		game->ceiling.b = tmp[2];
	}
}
