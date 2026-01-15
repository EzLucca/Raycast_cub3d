/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgueon <jgueon@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 16:15:24 by edlucca           #+#    #+#             */
/*   Updated: 2026/01/14 18:59:30 by jgueon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	get_tex_x(t_game *game, mlx_texture_t *tex, double dist)
{
	double	wall_x;
	int		tex_x;

	if (game->ray.side == 0)
		wall_x = game->player.y + dist * game->ray.ray_dir_y;
	else
		wall_x = game->player.x + dist * game->ray.ray_dir_x;
	wall_x -= floor(wall_x);
	tex_x = (int)(wall_x * tex->width);
	if ((game->ray.side == 0 && game->ray.step_x < 0)
		|| (game->ray.side == 1 && game->ray.step_y > 0))
		tex_x = tex->width - tex_x - 1;
	return (tex_x);
}

static	uint32_t	get_tex_color(mlx_texture_t *tex, int x, int y)
{
	uint8_t	*p;

	p = &tex->pixels[(y * tex->width + x) * tex->bytes_per_pixel];
	return ((p[0] << 24) | (p[1] << 16) | (p[2] << 8) | p[3]);
}

static void	draw_textured_column(t_game *game, int x, double dist)
{
	mlx_texture_t	*tex;
	int				tex_x;
	uint32_t		tex_y;
	int				y;
	int				wall_h;

	tex = get_wall_texture(game);
	tex_x = get_tex_x(game, tex, dist);
	wall_h = game->tex.wall_bottom - game->tex.wall_top + 1;
	y = game->tex.wall_top;
	while (y <= (int)game->tex.wall_bottom)
	{
		if (y >= 0 && y < (int)game->img_3d->height)
		{
			tex_y = ((y - game->tex.wall_top) * tex->height) / wall_h;
			if (tex_y >= tex->height)
				tex_y = tex->height - 1;
			mlx_put_pixel(game->img_3d, x, y,
				get_tex_color(tex, tex_x, tex_y));
		}
		y++;
	}
}

void	draw_map3d(t_game *game)
{
	uint32_t	x;
	double		ray_angle;
	double		dist;

	x = 0;
	while (x < game->img_3d->width)
	{
		ray_angle = game->player.da - FOV * 0.5
			+ ((double)x / game->img_3d->width) * FOV;
		cast_ray(ray_angle, game);
		dist = game->ray.dist * cos(ray_angle - game->player.da);
		if (dist < 0.1)
			dist = 0.1;
		game->tex.wall_height = (int)(game->img_3d->height / dist);
		if (game->tex.wall_height > game->img_3d->height)
			game->tex.wall_height = game->img_3d->height;
		game->tex.wall_top = (game->img_3d->height
				- game->tex.wall_height) * 0.5;
		game->tex.wall_bottom = game->tex.wall_top + game->tex.wall_height;
		draw_textured_column(game, x, dist);
		x++;
	}
}

void	resize_callback(int new_width, int new_height, void *param)
{
	t_game	*game;

	game = param;
	if (new_width <= 0 || new_height <= 0)
		return ;
	game->window_width = new_width;
	game->window_height = new_height;
	create_img(game, &game->img_3d, game->window_width, game->window_height);
	create_img(game, &game->img_map, MINIMAP_SIZE, MINIMAP_SIZE);
}
