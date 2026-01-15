/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edlucca <edlucca@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 16:15:35 by edlucca           #+#    #+#             */
/*   Updated: 2026/01/15 09:48:13 by edlucca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief Handles keyboard input events.
 *
 * Pressing the M key toggles the minimap display, while
 * pressing the TAB key toggles mouse control.
 *
 * @param keydata Structure containing key event information.
 * @param param   Pointer to the game structure.
 */
void	key_hook(mlx_key_data_t keydata, void *param)
{
	t_game	*game;

	game = param;
	if (keydata.key == MLX_KEY_M && keydata.action == MLX_PRESS)
		game->show_map = !game->show_map;
	if (keydata.key == MLX_KEY_TAB && keydata.action == MLX_PRESS)
		game->mouse = !game->mouse;
}

/**
 * @brief Main loop hook handling input and game updates.
 *
 * This function is called continuously by the MLX loop. It checks for
 * exit input and updates the player state by handling movement and
 * rotation based on current input.
 *
 * @param param Pointer to the game structure.
 */
void	ft_hook(void *param)
{
	t_game	*game;

	game = (t_game *)param;
	if (mlx_is_key_down(game->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(game->mlx);
	move_up(game);
	move_down(game);
	move_left(game);
	move_right(game);
	horizontal_rotation(game);
}

/**
 * @brief Fills an image with a single color.
 *
 * This function clears the given MLX image by setting every pixel
 * to the specified color.
 *
 * @param img   Pointer to the MLX image to be cleared.
 * @param color Color value used to fill the image.
 */
void	clear_image(mlx_image_t *img, uint32_t color)
{
	int	*p;
	int	count;
	int	i;

	p = (int *)img->pixels;
	count = img->width * img->height;
	i = 0;
	while (i < count)
	{
		p[i] = color;
		i++;
	}
}

/**
 * @brief Main game rendering loop.
 *
 * This function is called each frame by the MLX loop. It clears the
 * rendering images, draws the background, processes input and player
 * updates, and renders both the 3D view and the minimap. Mouse handling
 * is updated if enabled.
 *
 * @param param Pointer to the game structure.
 */
void	game_loop(void *param)
{
	t_game	*game;

	game = param;
	if (game->img_3d)
		clear_image(game->img_3d, 0x000000FF);
	if (game->img_map)
		clear_image(game->img_map, 0x000000FF);
	render_background(game, game->window_width, game->window_height);
	ft_hook(game);
	draw_map3d(game);
	draw_minimap(game);
	if (game->mouse == true)
		init_mouse(game);
}

/**
 * @brief Handles mouse movement for player rotation.
 *
 * This function updates the player's viewing angle based on horizontal
 * mouse movement. The angle is adjusted using a sensitivity factor,
 * normalized within the range [0, 2π], and the player's direction
 * vector is recalculated accordingly. The mouse cursor is then reset
 * to the center of the window.
 *
 * @param xpos  Current x-coordinate of the mouse cursor.
 * @param ypos  Current y-coordinate of the mouse cursor (unused).
 * @param param Pointer to the game structure.
 */
void	mouse_hook(double xpos, double ypos, void *param)
{
	t_game		*game;
	double		delta_x;
	int			center_x;
	int			center_y;

	(void)ypos;
	game = (t_game *)param;
	center_x = game->window_width / 2;
	center_y = game->window_height / 2;
	delta_x = xpos - center_x;
	game->player.da += delta_x * MOUSE_SENSITIVITY;
	if (game->player.da < 0)
		game->player.da += 2 * M_PI;
	else if (game->player.da > 2 * M_PI)
		game->player.da -= 2 * M_PI;
	game->player.dx = cos(game->player.da);
	game->player.dy = sin(game->player.da);
	mlx_set_mouse_pos(game->mlx, center_x, center_y);
}
