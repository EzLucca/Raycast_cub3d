/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map2d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgueon <jgueon@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 16:15:52 by edlucca           #+#    #+#             */
/*   Updated: 2026/01/15 09:52:20 by edlucca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief Prepares minimap coordinates for a single tile.
 *
 * This function calculates the scaled screen coordinates of a given
 * map tile relative to the player's position. It sets the start and
 * end positions for rendering the tile on the minimap and stores the
 * tile type.
 *
 * @param x    X-coordinate of the tile in the map grid.
 * @param y    Y-coordinate of the tile in the map grid.
 * @param game Pointer to the game structure.
 */
static void	minimap_prepare_tile(int x, int y, t_game *game)
{
	double	cam_x;
	double	cam_y;
	double	rel_x;
	double	rel_y;

	cam_x = game->player.x;
	cam_y = game->player.y;
	game->map2d.tile = game->map[y][x];
	rel_x = (double)x - cam_x;
	rel_y = (double)y - cam_y;
	game->map2d.start_x = (int)(rel_x * game->map2d.scale_x + MM_CENTER);
	game->map2d.start_y = (int)(rel_y * game->map2d.scale_y + MM_CENTER);
	game->map2d.end_x = (int)((rel_x + 1) * game->map2d.scale_x + MM_CENTER);
	game->map2d.end_y = (int)((rel_y + 1) * game->map2d.scale_y + MM_CENTER);
}

/**
 * @brief Draws a single tile on the minimap.
 *
 * This function renders a map tile onto the minimap image using its
 * precomputed screen coordinates. Tiles that are completely outside
 * the minimap boundaries are skipped. The color of the tile is
 * determined by the `tile_color` function.
 *
 * @param game Pointer to the game structure.
 */
static void	minimap_draw_tile(t_game *game)
{
	int	color;
	int	py;
	int	px;

	if (game->map2d.end_x < 0 || game->map2d.start_x >= MINIMAP_SIZE
		|| game->map2d.end_y < 0 || game->map2d.start_y >= MINIMAP_SIZE)
		return ;
	color = tile_color(game);
	py = game->map2d.start_y;
	while (py < game->map2d.end_y)
	{
		if (py >= 0 && py < MINIMAP_SIZE)
		{
			px = game->map2d.start_x;
			while (px < game->map2d.end_x)
			{
				if (px >= 0 && px < MINIMAP_SIZE)
					mlx_put_pixel(game->img_map, px, py, color);
				px++;
			}
		}
		py++;
	}
}

/**
 * @brief Draws the player's field of view rays on the minimap.
 *
 * This function casts multiple rays corresponding to the player's
 * current viewing angle and field of view. Each ray is projected onto
 * the minimap using scaled coordinates, and a line is drawn from the
 * minimap center to the ray endpoint. The ray color is set to red.
 *
 * @param g Pointer to the game structure.
 */
static void	minimap_draw_ray(t_game *g)
{
	int		r;
	double	angle;
	int		x;
	int		y;
	int		color;

	r = 0;
	color = set_color(g, RED, TRANSPARENT);
	while (r < NUM_RAYS)
	{
		angle = g->player.da - (double)FOV * 0.5 + r * ANGULAR_STEP;
		cast_ray(angle, g);
		x = (int)(g->ray.ray_dir_x * g->ray.dist * g->map2d.scale_x
				+ MM_CENTER);
		y = (int)(g->ray.ray_dir_y * g->ray.dist * g->map2d.scale_y
				+ MM_CENTER);
		draw_line(g->img_map, x, y, color);
		r++;
	}
}

/**
 * @brief Renders the entire minimap.
 *
 * This function draws the minimap for the current frame. It sets the
 * scaling factors, renders the background, iterates over all map
 * tiles to prepare and draw them, draws the player position, projects
 * the player's field of view rays, and finally draws the minimap border.
 *
 * @param param Pointer to the game structure.
 */
void	draw_minimap(void *param)
{
	t_game	*game;
	int		y;
	int		x;

	game = (t_game *)param;
	game->map2d.scale_x = (double)MINIMAP_SIZE * 0.05;
	game->map2d.scale_y = (double)MINIMAP_SIZE * 0.05;
	draw_minimap_background(game);
	y = 0;
	while (y < game->map_h)
	{
		x = 0;
		while (x < game->map_w)
		{
			minimap_prepare_tile(x, y, game);
			minimap_draw_tile(game);
			x++;
		}
		y++;
	}
	draw_player(game);
	minimap_draw_ray(game);
	draw_minimap_border(game);
}
