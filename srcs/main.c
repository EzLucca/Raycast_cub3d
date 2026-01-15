/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgueon <jgueon@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 16:15:45 by edlucca           #+#    #+#             */
/*   Updated: 2026/01/15 09:49:57 by edlucca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief Initializes the player state.
 *
 * This function sets the player's initial position to the center of the
 * starting tile, resets movement values, initializes the direction
 * vector, and determines the initial facing direction.
 *
 * @param game Pointer to the game structure.
 */
static void	init_player(t_game *game)
{
	game->player.x = game->player_x + 0.5;
	game->player.y = game->player_y + 0.5;
	game->player.xpos = 0;
	game->player.ypos = 0;
	game->player.dx = cosf(0.0f);
	game->player.dy = sinf(0.0f);
	game->player.move_x = 0.0f;
	game->player.move_y = 0.0f;
	get_player_dir(game);
}

/**
 * @brief Creates and displays an MLX image.
 *
 * This function deletes any previously existing image, creates a new
 * MLX image with the specified dimensions, and attaches it to the
 * window. In case of failure, resources are cleaned up and the window
 * is closed.
 *
 * @param game   Pointer to the game structure.
 * @param image  Address of the image pointer to create or replace.
 * @param width  Width of the image in pixels.
 * @param height Height of the image in pixels.
 *
 * @return EXIT_SUCCESS on success, or EXIT_FAILURE on error.
 */
int	create_img(t_game *game, mlx_image_t **image, int width, int height)
{
	if (*image)
	{
		mlx_delete_image(game->mlx, *image);
		*image = NULL;
	}
	*image = mlx_new_image(game->mlx, width, height);
	if (!*image)
	{
		mlx_close_window(game->mlx);
		puts(mlx_strerror(mlx_errno));
		return (EXIT_FAILURE);
	}
	if (mlx_image_to_window(game->mlx, *image, 0, 0) == -1)
	{
		mlx_delete_image(game->mlx, *image);
		*image = NULL;
		mlx_close_window(game->mlx);
		puts(mlx_strerror(mlx_errno));
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

/**
 * @brief Initializes the MLX context and game images.
 *
 * This function initializes the MLX42 window, sets the window title,
 * and creates the images used for 3D rendering and the minimap.
 *
 * @param game Pointer to the game structure.
 *
 * @return EXIT_SUCCESS on success, or EXIT_FAILURE if MLX initialization fails.
 */
int	init_mlx(t_game *game)
{
	game->mlx = mlx_init(WIN_W, WIN_H, "MLX42", true);
	if (!game->mlx)
	{
		puts(mlx_strerror(mlx_errno));
		return (EXIT_FAILURE);
	}
	mlx_set_window_title(game->mlx, "title");
	create_img(game, &game->img_3d, game->window_width, game->window_height);
	create_img(game, &game->img_map, MINIMAP_SIZE, MINIMAP_SIZE);
	return (0);
}

/**
 * @brief Frees all MLX-related resources.
 *
 * This function deletes all images and textures associated with the
 * game and terminates the MLX context to release allocated resources.
 *
 * @param game Pointer to the game structure.
 */
void	free_mlx_assets(t_game *game)
{
	mlx_delete_image(game->mlx, game->img_3d);
	mlx_delete_image(game->mlx, game->img_map);
	mlx_terminate(game->mlx);
	mlx_delete_texture(game->tex.n_wall);
	mlx_delete_texture(game->tex.s_wall);
	mlx_delete_texture(game->tex.w_wall);
	mlx_delete_texture(game->tex.e_wall);
}

// Main function
int	main(int argc, char **argv)
{
	t_game	game;

	if (check_args(argc, argv))
		return (1);
	if (parse_scene(argv[1], &game) != 0)
		return (1);
	init_player(&game);
	set_spawn_tile_walkable(game);
	if (init_mlx(&game) == 1)
	{
		printf("error\n");
		free_parser_game(&game);
		return (EXIT_FAILURE);
	}
	load_textures(&game);
	mlx_key_hook(game.mlx, key_hook, &game);
	mlx_resize_hook(game.mlx, resize_callback, &game);
	mlx_loop_hook(game.mlx, game_loop, &game);
	mlx_loop(game.mlx);
	free_mlx_assets(&game);
	free_parser_game(&game);
	return (EXIT_SUCCESS);
}
