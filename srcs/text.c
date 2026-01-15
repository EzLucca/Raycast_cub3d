/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edlucca <edlucca@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 16:16:35 by edlucca           #+#    #+#             */
/*   Updated: 2026/01/15 10:06:33 by edlucca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief Renders the ceiling and floor background in the 3D view.
 *
 * This function fills the 3D image buffer with ceiling and floor colors.
 * The top half of the image is set to the ceiling color, and the bottom
 * half is set to the floor color. It uses the current width and height
 * of the rendering area.
 *
 * @param game       Pointer to the game structure containing the 3D image
 *                   and ceiling/floor colors.
 * @param new_width  Width of the rendering area.
 * @param new_height Height of the rendering area.
 */
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

/**
 * @brief Loads wall textures from PNG files.
 *
 * This function loads the north, south, west, and east wall textures
 * into memory using the file paths stored in the game structure. If any
 * texture fails to load, the program prints an error message and exits.
 * It also initializes wall rendering parameters to zero.
 *
 * @param game Pointer to the game structure containing texture paths
 * and storage.
 */
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

/**
 * @brief Retrieves the appropriate wall texture for the current ray.
 *
 * This function returns the correct wall texture based on which side
 * of the wall the ray hit. For vertical walls (side 0), it selects
 * the east or west texture depending on the ray's X step. For horizontal
 * walls (side 1), it selects the south or north texture depending on
 * the ray's Y step.
 *
 * @param game Pointer to the game structure containing ray and texture info.
 *
 * @return Pointer to the mlx_texture_t representing the wall texture.
 */
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

/**
 * @brief Handles window resize events.
 *
 * This function updates the game’s stored window dimensions and
 * recreates the 3D and minimap images to match the new size. It
 * ensures that image buffers are valid for the resized window.
 *
 * @param new_width  The new width of the window.
 * @param new_height The new height of the window.
 * @param param      Pointer to the game structure.
 */
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
