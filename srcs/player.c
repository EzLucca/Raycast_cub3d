/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edlucca <edlucca@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 16:16:24 by edlucca           #+#    #+#             */
/*   Updated: 2026/01/15 10:01:23 by edlucca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief Converts a color structure to a 32-bit pixel value.
 *
 * This function takes a t_color structure with red, green, and blue
 * components (uint8_t) and packs it into a 32-bit integer in RGBA
 * format, setting the alpha channel to 255 (fully opaque).
 *
 * @param color The color structure containing r, g, b components.
 *
 * @return 32-bit unsigned integer representing the pixel color in RGBA.
 */
int	ft_pixel(t_color color) // use uint8_t
{
	return (color.r << 24 | color.g << 16 | color.b << 8 | 0xFF);
}

/**
 * @brief Sets the player's initial facing direction.
 *
 * This function initializes the player's viewing angle (`da`) based on
 * the character representing the starting direction ('N', 'S', 'E', 'W').
 *
 * @param game Pointer to the game structure containing player state.
 */
void	get_player_dir(t_game *game)
{
	if (game->player_dir == 'N')
		game->player.da = 3 * M_PI_2;
	else if (game->player_dir == 'S')
		game->player.da = M_PI_2;
	else if (game->player_dir == 'E')
		game->player.da = 0;
	else if (game->player_dir == 'W')
		game->player.da = M_PI;
}

/**
 * @brief Draws the player position on the minimap.
 *
 * This function renders a single pixel representing the player's
 * current position at the center of the minimap.
 *
 * @param game Pointer to the game structure containing the minimap image.
 */
void	draw_player(t_game *game)
{
	int	x;
	int	y;

	x = MM_CENTER;
	y = MM_CENTER;
	mlx_put_pixel(game->img_map, x, y, GREEN);
}

/**
 * @brief Chooses a color based on minimap visibility.
 *
 * This function returns one of two colors depending on whether the
 * minimap is currently shown. If `show_map` is true, the visible color
 * is returned; otherwise, the invisible color is used.
 *
 * @param game     Pointer to the game structure containing `show_map`.
 * @param visible  Color to use when the minimap is visible.
 * @param invisible Color to use when the minimap is hidden.
 *
 * @return The selected color.
 */
int	set_color(t_game *game, int visible, int invisible)
{
	int	color;

	if (game->show_map == true)
	{
		color = visible;
	}
	else
		color = invisible;
	return (color);
}
