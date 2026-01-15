/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edlucca <edlucca@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 16:16:35 by edlucca           #+#    #+#             */
/*   Updated: 2026/01/14 16:16:36 by edlucca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	render_background(t_game *game, int32_t new_width, int32_t new_height)
{
	int		x;
	int		y;
	int		floor_color;
	int		ceiling_color;

	x = 0;
	floor_color = ft_pixel(game->floor);
	ceiling_color = ft_pixel(game->ceiling);
	if (!game->img_3d)
		return ;
	while (x < new_width)
	{
		y = 0;
		while (y < new_height)
		{
			if (y < (new_height / 2))
				mlx_put_pixel(game->img_3d, x, y, floor_color);
			else
				mlx_put_pixel(game->img_3d, x, y, ceiling_color);
			y++;
		}
		x++;
	}
}

void	load_textures(t_game *game)
{
	game->tex.n_wall = mlx_load_png(game->tex.no);
	game->tex.s_wall = mlx_load_png(game->tex.so);
	game->tex.w_wall = mlx_load_png(game->tex.we);
	game->tex.e_wall = mlx_load_png(game->tex.ea);
	if (!game->tex.n_wall || !game->tex.s_wall
		|| !game->tex.w_wall || !game->tex.e_wall)
	{
		fprintf(stderr, "Failed to load textures!\n");
		exit(EXIT_FAILURE);
	}
	game->tex.wall_height = 0;
	game->tex.wall_top = 0;
	game->tex.wall_bottom = 0;
}

mlx_texture_t	*get_wall_texture(t_game *game)
{
	if (game->ray.side == 0)
	{
		if (game->ray.step_x > 0)
			return (game->tex.e_wall);
		else
			return (game->tex.w_wall);
	}
	else
	{
		if (game->ray.step_y > 0)
			return (game->tex.s_wall);
		else
			return (game->tex.n_wall);
	}
}
