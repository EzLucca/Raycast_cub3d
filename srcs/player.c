/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edlucca <edlucca@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 16:16:24 by edlucca           #+#    #+#             */
/*   Updated: 2026/01/14 16:16:26 by edlucca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// Convert RGB to 32-bit integer
// int	ft_pixel(int r, int g, int b, int a) // use uint8_t
int	ft_pixel(t_color color) // use uint8_t
{
	return (color.r << 24 | color.g << 16 | color.b << 8 | 0xFF);
}

void	get_player_dir(t_game *game)
{
	if (game->player_dir == 'N')
		game->player.da = 3 * M_PI_2;
	else if (game->player_dir == 'S')
		game->player.da = M_PI_2;
	else if (game->player_dir == 'E')
		game->player.da = 0;
	else if (game->player_dir == 'W')
		game->player.da = M_PI;
}

void	draw_player(t_game *game)
{
	int	x;
	int	y;

	x = MM_CENTER;
	y = MM_CENTER;
	mlx_put_pixel(game->img_map, x, y, GREEN);
}

int	set_color(t_game *game, int visible, int invisible)
{
	int	color;

	if (game->show_map == true)
	{
		color = visible;
	}
	else
		color = invisible;
	return (color);
}
