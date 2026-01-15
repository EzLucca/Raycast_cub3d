/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movements.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edlucca <edlucca@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 16:16:10 by edlucca           #+#    #+#             */
/*   Updated: 2026/01/14 16:16:16 by edlucca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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
