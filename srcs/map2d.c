/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map2d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgueon <jgueon@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 16:15:52 by edlucca           #+#    #+#             */
/*   Updated: 2026/01/14 17:11:36 by jgueon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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
