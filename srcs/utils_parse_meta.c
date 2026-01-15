/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_parse_meta.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgueon <jgueon@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 22:02:22 by jgueon            #+#    #+#             */
/*   Updated: 2026/01/14 17:35:40 by jgueon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief Check whether a line qualifies as a “map line”.
 *
 * A map line is defined here as:
 * - Non-NULL and not empty.
 * - Contains only valid map characters (as defined by is_map_charset()).
 * - Contains at least one real “tile” character (so a line of only spaces is
 *   not considered a map line).
 *
 * Tile characters that count as “real content” include:
 * - '0', '1'
 * - Player start markers: 'N', 'S', 'E', 'W'
 *
 * This helper is used during metadata parsing to detect when the map section
 * begins in the .cub file.
 *
 * @param line The input line to test.
 *
 * @return 1 if @p line is a valid map line, 0 otherwise.
 */
int	is_map_line(char *line)
{
	int	i;
	int	has_tile;

	if (!line || !*line)
		return (0);
	i = 0;
	has_tile = 0;
	while (line[i])
	{
		if (!is_map_charset(line[i]))
			return (0);
		if (line[i] == '0' || line[i] == '1' || line[i] == 'N'
			|| line[i] == 'S' || line[i] == 'E'
			|| line[i] == 'W')
			has_tile = 1;
		i++;
	}
	return (has_tile);
}

/**
 * @brief Replace the player spawn marker with '0' (walkable floor) in the map.
 *
 * After parsing, the player spawn tile is represented by one of 'N', 'S', 'E',
 * 'W'. Some parts of the program may prefer that the map contains only '0'/'1'
 * (and spaces), so this helper turns the spawn cell into '0'.
 *
 * Safety checks performed:
 * - Returns immediately if game.map is NULL.
 * - Returns if player coordinates are negative.
 *
 * Important note:
 * - The parameter is passed by value (t_game game), so the struct fields are
 *   copied, but game.map still points to the same underlying allocated grid.
 *   Therefore, writing to game.map[y][x] still modifies the real map content.
 *
 * @param game Game struct containing the map pointer and player coordinates.
 *
 * @return None.
 */
void	set_spawn_tile_walkable(t_game game)
{
	if (!game.map)
		return ;
	if (game.player_y < 0 || game.player_x < 0)
		return ;
	game.map[game.player_y][game.player_x] = '0';
	game.tile_size = game.map_w / game.map_h;
}
