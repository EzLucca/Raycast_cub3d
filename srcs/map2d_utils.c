/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map2d_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edlucca <edlucca@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 16:16:02 by edlucca           #+#    #+#             */
/*   Updated: 2026/01/15 09:58:06 by edlucca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief Fills the minimap background with a solid color.
 *
 * This function clears the entire minimap image by setting every pixel
 * to the specified background color (typically black with transparency).
 *
 * @param game Pointer to the game structure containing the minimap image.
 */
void	draw_minimap_background(t_game *game)
{
	int	x;
	int	y;
	int	color;

	y = 0;
	color = set_color(game, BLACK, TRANSPARENT);
	while (y < MINIMAP_SIZE)
	{
		x = 0;
		while (x < MINIMAP_SIZE)
		{
			mlx_put_pixel(game->img_map, x, y, color);
			x++;
		}
		y++;
	}
}

/**
 * @brief Draws the horizontal borders of the minimap.
 *
 * This function renders the top and bottom borders of the minimap
 * with the specified color and thickness.
 *
 * @param game  Pointer to the game structure containing the minimap image.
 * @param color Color value used for the border.
 */
static void	draw_minimap_border_horizontal(t_game *game, int color)
{
	int	x;
	int	y;

	y = 0;
	while (y < MM_BORDER_THICKNESS)
	{
		x = 0;
		while (x < MINIMAP_SIZE)
		{
			mlx_put_pixel(game->img_map, x, y, color);
			mlx_put_pixel(game->img_map, x, MINIMAP_SIZE - 1 - y, color);
			x++;
		}
		y++;
	}
}

/**
 * @brief Draws the vertical borders of the minimap.
 *
 * This function renders the left and right borders of the minimap
 * with the specified color and thickness.
 *
 * @param game  Pointer to the game structure containing the minimap image.
 * @param color Color value used for the border.
 */
static void	draw_minimap_border_vertical(t_game *game, int color)
{
	int	x;
	int	y;

	x = 0;
	while (x < MM_BORDER_THICKNESS)
	{
		y = 0;
		while (y < MINIMAP_SIZE)
		{
			mlx_put_pixel(game->img_map, x, y, color);
			mlx_put_pixel(game->img_map, MINIMAP_SIZE - 1 - x, y, color);
			y++;
		}
		x++;
	}
}

/**
 * @brief Draws the full border around the minimap.
 *
 * This function renders both the horizontal and vertical borders of
 * the minimap using a solid color (typically black with transparency),
 * creating a complete framed outline.
 *
 * @param game Pointer to the game structure containing the minimap image.
 */
void	draw_minimap_border(t_game *game)
{
	int	color;

	color = set_color(game, BLACK, TRANSPARENT);
	draw_minimap_border_horizontal(game, color);
	draw_minimap_border_vertical(game, color);
}

/**
 * @brief Determines the color of a map tile for the minimap.
 *
 * This function returns the appropriate color for the current tile
 * based on its type and whether the minimap is visible. Wall tiles
 * ('1') are white, empty spaces are black, and the tile is transparent
 * if the minimap is hidden.
 *
 * @param game Pointer to the game structure containing map and minimap state.
 *
 * @return The 32-bit color value for the tile.
 */
int	tile_color(t_game *game)
{
	int	color;

	if (game->show_map == true)
	{
		if (game->map2d.tile == '1')
			color = WHITE;
		else
			color = BLACK;
	}
	else
		color = TRANSPARENT;
	return (color);
}
