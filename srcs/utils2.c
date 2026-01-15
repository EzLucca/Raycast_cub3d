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

/**
 * @brief Checks if a given map position is a wall or invalid.
 *
 * This function determines whether the specified coordinates correspond
 * to a wall or an invalid map space. It treats out-of-bounds positions,
 * map tiles with '1', and empty spaces (' ') as walls.
 *
 * @param game Pointer to the game structure containing the map and
 * its dimensions.
 * @param x    X-coordinate to check.
 * @param y    Y-coordinate to check.
 *
 * @return 1 if the position is a wall or invalid, 0 otherwise.
 */
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

/**
 * @brief Checks if the player can move to a specified position.
 *
 * This function determines whether the player can move to the
 * coordinates (`nx`, `ny`) without colliding with walls. It checks
 * the four cardinal points around the player's bounding radius to
 * ensure safe movement.
 *
 * @param game Pointer to the game structure containing the map.
 * @param nx   Proposed X-coordinate for the player.
 * @param ny   Proposed Y-coordinate for the player.
 *
 * @return 1 if the player can move to the position, 0 if blocked by a wall.
 */
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

/**
 * @brief Initializes mouse control for the game.
 *
 * This function disables the default cursor and sets up a mouse
 * callback to handle mouse movement for player rotation.
 *
 * @param game Pointer to the game structure containing the MLX instance.
 */
void	init_mouse(t_game *game)
{
	mlx_set_cursor_mode(game->mlx, MLX_MOUSE_DISABLED);
	mlx_cursor_hook(game->mlx, mouse_hook, game);
}
