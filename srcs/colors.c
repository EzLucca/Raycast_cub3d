/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgueon <jgueon@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 22:08:33 by jgueon            #+#    #+#             */
/*   Updated: 2026/01/13 19:33:32 by jgueon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief Parse and validate RGB tokens split by comma.
 *
 * This function expects an array of 3 strings (typically produced by
 * ft_split(meta, ',')) where each token represents a numeric component
 * of an RGB color. It validates:
 * - There are exactly 3 tokens.
 * - Each token is a signed number string.
 * - Each parsed integer is within the valid RGB range (0..255).
 *
 * On success, it writes the 3 parsed integers into the @p rgb output array.
 *
 * @param color Array of strings containing the RGB components (size must be 3).
 * @param rgb Output array of 3 integers receiving R, G, B.
 *
 * @return 0 on success, non-zero on failure.
 */
static int	parse_rgb_values(char **color, int *rgb)
{
	int	i;
	int	n;

	if (!color || !rgb)
		return (1);
	if (arrlen(color) != 3)
		return (ft_error(INVALID_RGB_VALUE_MSG));
	i = 0;
	while (i < 3)
	{
		if (!is_signed_number(color[i]))
			return (ft_error(INVALID_RGB_CHAR_MSG));
		n = ft_atoi(color[i]);
		if (!check_rgb_range(n))
			return (ft_error(INVALID_RGB_RANGE_MSG));
		rgb[i] = n;
		i++;
	}
	return (0);
}

/**
 * @brief Trim whitespace around each token in a split RGB array.
 *
 * After splitting a color string like "220, 100, 0" by ',', tokens may contain
 * leading/trailing spaces (e.g., " 100"). This function replaces each token
 * with a newly allocated trimmed version (using ft_strtrim), freeing the old
 * token strings to avoid leaks.
 *
 * @param colors NULL-terminated array of strings to trim in-place.
 *
 * @return 0 on success, 1 on allocation/trim failure.
 */
static int	trim_tokens(char **colors)
{
	int		i;
	char	*trimmed;

	i = 0;
	while (colors[i])
	{
		trimmed = ft_strtrim(colors[i], " \t\n\r");
		if (!trimmed)
			return (1);
		free(colors[i]);
		colors[i] = trimmed;
		i++;
	}
	return (0);
}

/**
 * @brief Parse a full RGB line starting with a given identifier ('F' or 'C').
 *
 * This function parses lines like:
 * - "F 220, 100, 0"
 * - "C 225,30,0"
 *
 * Steps performed:
 * - Ensure @p line starts with @p identifier.
 * - Skip the identifier and any following spaces/tabs.
 * - Ensure there are exactly 2 commas in the remaining string.
 * - Split by ',' into 3 tokens.
 * - Trim whitespace on each token.
 * - Validate and convert tokens into 3 integers stored in @p rgb.
 *
 * @param identifier Expected identifier character ('F' for floor,
 * 		'C' for ceiling).
 * @param line Input line containing identifier + RGB values.
 * @param rgb Output array of 3 integers receiving parsed R, G, B.
 *
 * @return 0 on success, non-zero on failure (also reports errors via ft_error).
 */
int	parse_rgb_line(char identifier, char *line, int *rgb)
{
	char	*meta;
	char	**colors;
	int		i;

	if (!line || !rgb)
		return (ft_error("RGB: NULL input\n"));
	if (line[0] != identifier)
		return (ft_error("RGB: wrong identifier\n"));
	meta = line + 1;
	while (*meta == ' ' || *meta == '\t')
		meta++;
	if (get_nb_comma(meta) != 2)
		return (ft_error(INVALID_RGB_VALUE_MSG));
	colors = ft_split(meta, ',');
	if (!colors)
		return (ft_error("ft_split failed"));
	if (trim_tokens(colors))
	{
		free_split(colors);
		return (ft_error("ft_strtrim failed"));
	}
	i = parse_rgb_values(colors, rgb);
	free_split(colors);
	return (i);
}

/**
 * @brief Check whether a trimmed line contains a color value after its
 * 		identifier.
 *
 * This helper assumes @p trim starts with 'F' or 'C'. It checks that there is
 * something after the identifier (skipping spaces/tabs) and that the first
 * non-space character is a digit. This rejects lines like "F" or "C   ".
 *
 * @param trim A trimmed line beginning with a color identifier.
 *
 * @return 1 if it looks like a color line with values, 0 otherwise.
 */
static int	is_color_line(char *trim)
{
	int	i;

	i = 1;
	while (trim[i] == ' ' || trim[i] == '\t')
		i++;
	if (trim[i] == '\0')
		return (0);
	if (trim[i] < '0' || trim[i] > '9')
		return (0);
	return (1);
}

/**
 * @brief Handle one already-trimmed metadata line if it defines
 * 		floor/ceiling color.
 *
 * If the line begins with:
 * - 'F': validates floor color format, checks duplication, parses
 *   the RGB values, then stores it into game->floor via store_color.
 * - 'C': validates ceiling color format, checks duplication (already set),
 *   parses the RGB values, then stores it into game->ceiling via store_color.
 *
 * Return convention used here:
 * - 1 when a valid color line was handled (stored successfully).
 * - -1 on error (invalid format, duplicate, parse failure).
 *
 * @param game Main game struct where floor/ceiling colors are stored.
 * @param trim Trimmed input line starting with 'F' or 'C'.
 *
 * @return 1 if handled, -1 on error.
 */
int	handle_color_line(t_game *game, char *trim)
{
	int	tmp[3];

	if (trim[0] == 'F' && !is_color_line(trim))
		return (ft_error(INVALID_RGB_VALUE_MSG), -1);
	if (trim[0] == 'C' && !is_color_line(trim))
		return (ft_error("Invalid ceiling color format\n"), -1);
	if (trim[0] == 'F' && is_color_line(trim))
	{
		if (is_color_set(game->floor))
			return (ft_error(INVALID_DUP_FLOOR), -1);
		if (parse_rgb_line('F', trim, tmp))
			return (-1);
		store_color(game, 'F', tmp);
	}
	else if (trim[0] == 'C' && is_color_line(trim))
	{
		if (is_color_set(game->ceiling))
			return (ft_error(INVALID_DUP_CEIL), -1);
		if (parse_rgb_line('C', trim, tmp))
			return (-1);
		store_color(game, 'C', tmp);
	}
	return (1);
}
