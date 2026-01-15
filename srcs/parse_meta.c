/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_meta.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgueon <jgueon@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 13:40:25 by jgueon            #+#    #+#             */
/*   Updated: 2026/01/14 13:13:35 by jgueon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief Handle one trimmed metadata line (texture/color identifiers).
 *
 * This function tries to interpret a single trimmed line as a valid metadata
 * identifier:
 * - First attempts texture parsing via handle_texture_line().
 *   - If it returns 1: the line was handled (valid texture identifier).
 *   - If it returns -1: error occurred (already printed by callee).
 * - If not a texture line, it tries colors:
 *   - If the line starts with 'F' or 'C', it delegates to handle_color_line().
 * - If the trimmed line is empty, it is ignored.
 * - Otherwise it is treated as an unknown identifier and triggers WRONG_MSG.
 *
 * Return convention used:
 * - 1: line was successfully handled (texture or color line).
 * - 0: line was empty/ignored.
 * - -1: error.
 *
 * @param game Game context where textures/colors are stored.
 * @param trim Pointer to the trimmed line content(typically skip_spaces(line)).
 *
 * @return 1 if handled, 0 if ignored, -1 on error.
 */
static int	handle_one_meta_line(t_game *game, char *trim)
{
	int	ret;

	ret = handle_texture_line(game, trim);
	if (ret == -1)
		return (-1);
	if (ret == 1)
		return (1);
	if (trim[0] == 'F' || trim[0] == 'C')
		return (handle_color_line(game, trim));
	if (*trim == '\0')
		return (0);
	return (ft_error(WRONG_MSG), -1);
}

/**
 * @brief Validate that all required metadata identifiers were provided.
 *
 * Required identifiers:
 * - 4 wall textures: NO, SO, WE, EA (stored in game->tex.* pointers).
 * - 2 colors: floor and ceiling (stored in game->floor and game->ceiling).
 *
 * This function is a validator (not silent): when something is missing, it
 * prints a specific error message (INVALID_MISSING_TEX, BOTH_IDEN_MISSING,
 * INVALID_MISSING_FLOOR, INVALID_MISSING_CEIL).
 *
 * @param game Game context containing parsed metadata fields.
 *
 * @return 0 if all required metadata exists, non-zero otherwise.
 */
static int	check_meta_complete(t_game *game)
{
	if (!game->tex.no || !game->tex.so || !game->tex.we || !game->tex.ea)
		return (ft_error(INVALID_MISSING_TEX));
	if (game->floor.r == -1 && game->ceiling.r == -1)
		return (ft_error(BOTH_IDEN_MISSING));
	if (game->floor.r == -1)
		return (ft_error(INVALID_MISSING_FLOOR));
	if (game->ceiling.r == -1)
		return (ft_error(INVALID_MISSING_CEIL));
	return (0);
}

/**
 * @brief Detect whether a string contains at least one “map tile” character.
 *
 * This helper searches for characters that typically indicate map content:
 * '0', '1', or player start markers 'N', 'S', 'E', 'W'.
 *
 * It is used to improve error reporting: if metadata looks complete and a line
 * contains map tiles but is not a valid map line, the code returns an
 * INVALID_MAP_CHAR_MSG instead of treating it as “wrong metadata”.
 *
 * @param s Input string to scan (may be NULL).
 *
 * @return 1 if a map tile character is found, 0 otherwise.
 */
static int	has_map_tile(char *s)
{
	int	i;

	i = 0;
	while (s && s[i])
	{
		if (s[i] == '0' || s[i] == '1' || s[i] == 'N'
			|| s[i] == 'S' || s[i] == 'E' || s[i] == 'W')
			return (1);
		i++;
	}
	return (0);
}

/**
 * @brief Silent check: are all required metadata fields already set?
 *
 * Unlike check_meta_complete(), this function does not print errors.
 * It simply returns whether textures and both colors are present, so the parser
 * can decide how to interpret borderline lines without spamming “missing id”
 * messages during normal scanning.
 *
 * @param game Game context containing parsed metadata fields.
 *
 * @return 1 if metadata is complete, 0 otherwise.
 */
static int	meta_ready(t_game *game)
{
	if (!game->tex.no || !game->tex.so || !game->tex.we || !game->tex.ea)
		return (0);
	if (game->floor.r == -1 || game->ceiling.r == -1)
		return (0);
	return (1);
}

/**
 * @brief Parse identifiers line-by-line until the first map line is found.
 *
 * This function reads from @p fd using get_line() and processes lines in
 * “metadata mode” until it detects the first map line:
 * - When the first map line is found:
 *   - It validates metadata completeness via check_meta_complete().
 *   - On success, it sets *first_line to that map line and returns 0.
 *   - The file descriptor is left positioned *after* that first map line
 *     (because it was already read).
 * - For non-map lines:
 *   - It trims leading spaces (skip_spaces) and handles textures/colors.
 *   - Empty lines are ignored.
 *   - Unknown non-empty lines trigger WRONG_MSG.
 *
 * Special rule for better errors:
 * - If metadata is already complete and a non-empty line contains map tiles but
 *   is not a valid map line, it returns INVALID_MAP_CHAR_MSG.
 *
 * Output parameter:
 * - On success, *first_line is set to an allocated line returned by get_line().
 *   The caller is expected to “own” it and later free it (directly or through
 *   a container like the map lines array).
 *
 * @param fd Open file descriptor for the .cub file.
 * @param game Game context where parsed metadata is stored.
 * @param first_line Output pointer receiving the first map line on success.
 *
 * @return 0 on success, non-zero on error (also prints an error message).
 */
int	parse_identifiers_until_map(int fd, t_game *game, char **first_line)
{
	char	*line;
	char	*trim;

	*first_line = NULL;
	line = get_line(fd);
	while (line)
	{
		if (!*first_line && is_map_line(line))
		{
			if (check_meta_complete(game))
				return (free(line), 1);
			*first_line = line;
			return (0);
		}
		trim = skip_spaces(line);
		if (meta_ready(game) && *trim != '\0'
			&& has_map_tile(trim) && !is_map_line(line))
			return (free(line), ft_error(INVALID_MAP_CHAR_MSG));
		if (handle_one_meta_line(game, trim) == -1)
			return (free(line), 1);
		free(line);
		line = get_line(fd);
	}
	return (ft_error(EMPTY_MSG));
}
