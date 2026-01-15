/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgueon <jgueon@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 22:00:17 by jgueon            #+#    #+#             */
/*   Updated: 2026/01/14 17:09:11 by jgueon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief Check whether a character belongs to the allowed map character set.
 *
 * This helper defines which characters are valid inside the parsed map grid.
 * Accepted characters are:
 * - '0' (floor)
 * - '1' (wall)
 * - 'N', 'S', 'E', 'W' (player start + orientation)
 * - ' ' (space / void padding)
 *
 * It is commonly used by parsing/validation code to reject invalid symbols
 * inside map lines.
 *
 * @param c The character to test.
 *
 * @return 1 if @p c is allowed in the map, 0 otherwise.
 */
int	is_map_charset(char c)
{
	if (c == '0' || c == '1')
		return (1);
	if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
		return (1);
	if (c == ' ')
		return (1);
	return (0);
}

int	is_wall(t_game *game, double x, double y)
{
	int	mx;
	int	my;

	mx = (int)x;
	my = (int)y;
	if (mx < 0 || my < 0 || my >= game->map_h || mx >= game->map_w)
		return (1);
	if (game->map[my][mx] == '1')
		return (1);
	if (game->map[my][mx] == ' ')
		return (1);
	return (0);
}

int	can_move_to(t_game *game, double nx, double ny)
{
	if (is_wall(game, nx - PLAYER_RADIUS, ny))
		return (0);
	if (is_wall(game, nx + PLAYER_RADIUS, ny))
		return (0);
	if (is_wall(game, nx, ny - PLAYER_RADIUS))
		return (0);
	if (is_wall(game, nx, ny + PLAYER_RADIUS))
		return (0);
	return (1);
}

void	init_mouse(t_game *game)
{
	mlx_set_cursor_mode(game->mlx, MLX_MOUSE_DISABLED);
	mlx_cursor_hook(game->mlx, mouse_hook, game);
}
