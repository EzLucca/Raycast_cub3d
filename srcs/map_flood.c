/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_flood.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgueon <jgueon@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 20:08:41 by jgueon            #+#    #+#             */
/*   Updated: 2026/01/13 19:48:24 by jgueon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief Duplicate the padded map grid so it can be modified safely.
 *
 * This function allocates a new NULL-terminated array of strings with the same
 * height as g->map_h, then duplicates each row from g->map using ft_strdup.
 * The copy is meant to be used by flood fill so visited cells can be marked
 * without touching the original map.
 *
 * Implementation detail:
 * - It sets cpy[g->map_h] = NULL early so free_map(cpy) remains safe even if
 *   an allocation fails part-way through.
 *
 * @param g The game context containing the padded map (g->map), height,
 * 	and width.
 *
 * @return Newly allocated duplicated grid, or NULL on allocation failure.
 */
static char	**dup_grid(t_game *g)
{
	char	**cpy;
	int		y;

	cpy = (char **)malloc(sizeof(char *) * (size_t)(g->map_h + 1));
	if (!cpy)
		return (NULL);
	cpy[g->map_h] = NULL;
	y = 0;
	while (y < g->map_h)
	{
		cpy[y] = ft_strdup(g->map[y]);
		if (!cpy[y])
			return (free_map(cpy), NULL);
		y++;
	}
	return (cpy);
}

/**
 * @brief Determine whether a map cell is considered walkable for flood fill.
 *
 * Walkable cells are those the player can stand on / traverse:
 * - '0' is floor (walkable).
 * - 'N', 'S', 'E', 'W' represent the player start orientation (walkable).
 *
 * Non-walkable examples include:
 * - '1' walls.
 * - ' ' spaces (treated as void/outside).
 *
 * @param c The cell character to test.
 *
 * @return 1 if walkable, 0 otherwise.
 */
static int	is_walkable(char c)
{
	if (c == '0')
		return (1);
	if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
		return (1);
	return (0);
}

/**
 * @brief Check whether a given cell touches the void/outside area.
 *
 * A cell is considered adjacent to the void if:
 * - It lies on the border of the padded map (top row, bottom row, left column,
 *   right column), or
 * - One of its 4-neighbors (up/down/left/right) is a space ' '.
 *
 * This is used by the flood-fill to detect "open" maps (leaks to outside).
 *
 * @param m The modifiable map grid copy.
 * @param g The game context providing map dimensions.
 * @param y Y coordinate of the cell.
 * @param x X coordinate of the cell.
 *
 * @return 1 if the cell has a void neighbor (or is at border), 0 otherwise.
 */
static int	is_void_neighbor(char **m, t_game *g, int y, int x)
{
	if (y == 0 || x == 0 || y == g->map_h - 1 || x == g->map_w - 1)
		return (1);
	if (m[y - 1][x] == ' ' || m[y + 1][x] == ' ' || m[y][x - 1] == ' '
			|| m[y][x + 1] == ' ')
		return (1);
	return (0);
}

/**
 * @brief Recursive flood fill to detect if the map is open (not closed by
 * 	walls).
 *
 * Starting from a walkable cell, this function explores 4-directionally.
 * It marks visited walkable cells as 'V' to prevent infinite recursion.
 *
 * The map is considered invalid/open if the flood can reach:
 * - Out of bounds, or
 * - A cell adjacent to a void space ' ', or
 * - A space cell itself (treated as outside/void).
 *
 * Return convention:
 * - 1 means the map is OPEN (invalid).
 * - 0 means no opening was found along this explored region.
 *
 * @param m The modifiable duplicated grid used to mark visited cells.
 * @param g The game context providing map dimensions.
 * @param y Current Y coordinate in the flood.
 * @param x Current X coordinate in the flood.
 *
 * @return 1 if an opening is detected, 0 otherwise.
 */
static int	flood(char **m, t_game *g, int y, int x)
{
	if (y < 0 || x < 0 || y >= g->map_h || x >= g->map_w)
		return (1);
	if (!is_walkable(m[y][x]))
		return (0);
	if (is_void_neighbor(m, g, y, x))
		return (1);
	if (m[y][x] == ' ')
		return (1);
	if (m[y][x] == '1' || m[y][x] == 'V')
		return (0);
	m[y][x] = 'V';
	if (flood(m, g, y - 1, x))
		return (1);
	if (flood(m, g, y + 1, x))
		return (1);
	if (flood(m, g, y, x - 1))
		return (1);
	if (flood(m, g, y, x + 1))
		return (1);
	return (0);
}

/**
 * @brief Validate that the map is closed using flood fill from the player
 * 	start.
 *
 * This function duplicates the padded map grid, then runs flood fill starting
 * from the player coordinates (g->player_y, g->player_x). If the flood detects
 * any connection to the outside/void, the map is considered open and invalid.
 *
 * Memory handling:
 * - The duplicated grid is always freed before returning.
 *
 * @param g The game context containing the padded map and player position.
 *
 * @return 0 if the map is closed/valid, non-zero on error (prints an error
 * 	message).
 */
int	validate_map_closed(t_game *g)
{
	char	**cpy;
	int		open;

	cpy = dup_grid(g);
	if (!cpy)
		return (ft_error("Malloc failed\n"));
	open = flood(cpy, g, g->player_y, g->player_x);
	free_map(cpy);
	if (open)
		return (ft_error(MAP_OPEN_MSG));
	return (0);
}
