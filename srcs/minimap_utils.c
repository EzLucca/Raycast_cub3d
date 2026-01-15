/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edlucca <edlucca@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 16:16:02 by edlucca           #+#    #+#             */
/*   Updated: 2026/01/14 16:24:47 by edlucca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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

void	draw_minimap_border(t_game *game)
{
	int	color;

	color = set_color(game, BLACK, TRANSPARENT);
	draw_minimap_border_horizontal(game, color);
	draw_minimap_border_vertical(game, color);
}

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
