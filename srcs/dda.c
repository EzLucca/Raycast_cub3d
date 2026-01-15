/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dda.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edlucca <edlucca@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 16:15:29 by edlucca           #+#    #+#             */
/*   Updated: 2026/01/14 16:39:15 by edlucca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static t_line	init_line(int x1, int y1)
{
	t_line	line;

	line.x = MM_CENTER;
	line.y = MM_CENTER;
	line.dx = abs(x1 - line.x);
	line.dy = abs(y1 - line.y);
	line.sx = (line.x < x1) - (line.x > x1);
	line.sy = (line.y < y1) - (line.y > y1);
	line.err = line.dx - line.dy;
	return (line);
}

void	draw_line(mlx_image_t *img, int x1, int y1, uint32_t color)
{
	t_line	line;
	int		e2;

	line = init_line(x1, y1);
	while (1)
	{
		if (line.x >= 0 && line.x < MINIMAP_SIZE
			&& line.y >= 0 && line.y < MINIMAP_SIZE)
			mlx_put_pixel(img, line.x, line.y, color);
		if (line.x == x1 && line.y == y1)
			break ;
		e2 = line.err * 2;
		if (e2 > -line.dy)
		{
			line.err -= line.dy;
			line.x += line.sx;
		}
		if (e2 < line.dx)
		{
			line.err += line.dx;
			line.y += line.sy;
		}
	}
}
