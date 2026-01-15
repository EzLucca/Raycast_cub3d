/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_validate.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgueon <jgueon@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/03 01:22:11 by jgueon            #+#    #+#             */
/*   Updated: 2026/01/13 19:59:28 by jgueon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief Check whether a character is a player start/orientation marker.
 *
 * Valid player markers are the four cardinal direction letters used in cub3D:
 * 'N', 'S', 'E', 'W'.
 *
 * @param c The character to test.
 *
 * @return 1 if @p c is one of 'N', 'S', 'E', 'W', otherwise 0.
 */
static int	is_player(char c)
{
	return (c == 'N' || c == 'S' || c == 'E' || c == 'W');
}

/**
 * @brief Check whether a character is allowed inside the parsed map grid.
 *
 * This validator accepts:
 * - '0' floor
 * - '1' wall
 * - ' ' space (void padding / outside)
 * - One of the player markers ('N', 'S', 'E', 'W')
 *
 * @param c The map cell character to validate.
 *
 * @return 1 if the character is valid, 0 otherwise.
 */
static int	is_valid_map_char(char c)
{
	if (c == '0' || c == '1' || c == ' ')
		return (1);
	if (is_player(c))
		return (1);
	return (0);
}

/**
 * @brief Store the player position and direction in the game state.
 *
 * This function enforces that only one player exists in the map:
 * - If a player was already stored (g->player_found is true), it raises an
 * 	error.
 * - Otherwise, it sets g->player_found, g->player_dir, and the (x, y)
 * 	coordinates.
 *
 * @param g Game context where player fields are stored.
 * @param y Player Y position in the map grid.
 * @param x Player X position in the map grid.
 * @param c The player direction character ('N', 'S', 'E', or 'W').
 *
 * @return 0 on success, non-zero on error (prints via ft_error).
 */
static int	store_player(t_game *g, int y, int x, char c)
{
	if (g->player_found)
		return (ft_error(PLAYER_COUNT_MSG));
	g->player_found = 1;
	g->player_dir = c;
	g->player_x = x;
	g->player_y = y;
	return (0);
}

/**
 * @brief Validate map characters and ensure exactly one player exists.
 *
 * This function iterates over the entire padded rectangular map(map_h x map_w):
 * - Rejects any character that is not allowed by is_valid_map_char().
 * - Detects player markers and stores the player info via store_player().
 *
 * At the end, it also rejects maps with no player found.
 *
 * Side effects:
 * - Resets g->player_found to 0 at the beginning.
 * - On success, fills g->player_found, g->player_dir, g->player_x, g->player_y.
 *
 * @param g Game context containing the padded map grid and its dimensions.
 *
 * @return 0 if valid, non-zero if invalid (prints via ft_error).
 */
static int	check_player_and_chars(t_game *g)
{
	int		y;
	int		x;
	char	c;

	g->player_found = 0;
	y = 0;
	while (y < g->map_h)
	{
		x = 0;
		while (x < g->map_w)
		{
			c = g->map[y][x];
			if (!is_valid_map_char(c))
				return (ft_error(INVALID_MAP_CHAR_MSG));
			if (is_player(c) && store_player(g, y, x, c))
				return (1);
			x++;
		}
		y++;
	}
	if (!g->player_found)
		return (ft_error(PLAYER_COUNT_MSG));
	return (0);
}

/**
 * @brief Validate the loaded map (basic integrity + closed walls).
 *
 * Validation performed:
 * - Ensures the map grid exists and dimensions are positive.
 * - Validates allowed characters and exactly one player start.
 * - Validates the map is closed using validate_map_closed() (flood fill).
 *
 * @param game Game context containing the map grid and receiving player info.
 *
 * @return 0 if the map is valid, non-zero if invalid (prints via ft_error).
 */
int	validate_map(t_game *game)
{
	if (!game->map || game->map_h <= 0 || game->map_w <= 0)
		return (ft_error(EMPTY_MSG));
	if (check_player_and_chars(game))
		return (1);
	if (validate_map_closed(game))
		return (1);
	return (0);
}
