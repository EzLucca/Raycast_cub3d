/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movements.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edlucca <edlucca@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 16:16:10 by edlucca           #+#    #+#             */
/*   Updated: 2026/01/15 09:59:52 by edlucca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief Moves the player forward.
 *
 * This function checks if the "W" key is pressed and attempts to move
 * the player forward in the direction they are currently facing. It
 * computes the new position based on the player's angle and speed,
 * and updates the player's coordinates only if the movement is allowed.
 *
 * @param game Pointer to the game structure containing player state and map.
 */
void	move_up(t_game *game)
{
	double	nx;
	double	ny;

	if (mlx_is_key_down(game->mlx, MLX_KEY_W))
	{
		game->player.move_x = cos(game->player.da) * PLAYER_SPEED;
		game->player.move_y = sin(game->player.da) * PLAYER_SPEED;
		nx = game->player.x + game->player.move_x;
		ny = game->player.y + game->player.move_y;
		if (can_move_to(game, nx, ny))
		{
			game->player.x = nx;
			game->player.y = ny;
		}
	}
}

/**
 * @brief Moves the player backward.
 *
 * This function checks if the "S" key is pressed and attempts to move
 * the player backward, opposite to the direction they are currently
 * facing. It calculates the new position using the player's angle and
 * speed, and updates the coordinates only if movement is allowed.
 *
 * @param game Pointer to the game structure containing player state and map.
 */
void	move_down(t_game *game)
{
	double	nx;
	double	ny;

	if (mlx_is_key_down(game->mlx, MLX_KEY_S))
	{
		game->player.move_x = cos(game->player.da) * PLAYER_SPEED;
		game->player.move_y = sin(game->player.da) * PLAYER_SPEED;
		nx = game->player.x - game->player.move_x;
		ny = game->player.y - game->player.move_y;
		if (can_move_to(game, nx, ny))
		{
			game->player.x = nx;
			game->player.y = ny;
		}
	}
}

/**
 * @brief Strafes the player to the left.
 *
 * This function checks if the "A" key is pressed and attempts to move
 * the player sideways to the left, perpendicular to the direction they
 * are currently facing. The new position is calculated using the player's
 * angle offset by 90 degrees and the movement speed, and updated only if
 * movement is allowed.
 *
 * @param game Pointer to the game structure containing player state and map.
 */
void	move_left(t_game *game)
{
	double	nx;
	double	ny;

	if (mlx_is_key_down(game->mlx, MLX_KEY_A))
	{
		game->player.move_x = cos(game->player.da - M_PI_2) * PLAYER_SPEED;
		game->player.move_y = sin(game->player.da - M_PI_2) * PLAYER_SPEED;
		nx = game->player.x + game->player.move_x;
		ny = game->player.y + game->player.move_y;
		if (can_move_to(game, nx, ny))
		{
			game->player.x = nx;
			game->player.y = ny;
		}
	}
}

/**
 * @brief Strafes the player to the right.
 *
 * This function checks if the "D" key is pressed and attempts to move
 * the player sideways to the right, perpendicular to the direction they
 * are currently facing. The new position is calculated using the player's
 * angle offset by 90 degrees and the movement speed, and updated only if
 * movement is allowed.
 *
 * @param game Pointer to the game structure containing player state and map.
 */
void	move_right(t_game *game)
{
	double	nx;
	double	ny;

	if (mlx_is_key_down(game->mlx, MLX_KEY_D))
	{
		game->player.move_x = cos(game->player.da + M_PI_2) * PLAYER_SPEED;
		game->player.move_y = sin(game->player.da + M_PI_2) * PLAYER_SPEED;
		nx = game->player.x + game->player.move_x;
		ny = game->player.y + game->player.move_y;
		if (can_move_to(game, nx, ny))
		{
			game->player.x = nx;
			game->player.y = ny;
		}
	}
}

/**
 * @brief Rotates the player horizontally.
 *
 * This function checks for left and right arrow key presses and adjusts
 * the player's viewing angle accordingly. Pressing the left arrow rotates
 * the player counterclockwise, while pressing the right arrow rotates
 * clockwise.
 *
 * @param game Pointer to the game structure containing player state.
 */
void	horizontal_rotation(t_game *game)
{
	if (mlx_is_key_down(game->mlx, MLX_KEY_LEFT))
	{
		game->player.da -= 0.05f;
	}
	if (mlx_is_key_down(game->mlx, MLX_KEY_RIGHT))
	{
		game->player.da += 0.05f;
	}
}
