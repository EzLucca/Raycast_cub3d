/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dda.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edlucca <edlucca@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 16:15:29 by edlucca           #+#    #+#             */
/*   Updated: 2026/01/15 09:43:54 by edlucca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief Initializes a t_line structure for line drawing.
 *
 * This function prepares a t_line structure used for drawing a line
 * from the center of the minimap (MM_CENTER, MM_CENTER) to the target
 * point (x1, y1). It computes the deltas, step directions, and initial
 * error value required for a Bresenham-style line drawing algorithm.
 *
 * @param x1 The x-coordinate of the destination point.
 * @param y1 The y-coordinate of the destination point.
 *
 * @return A fully initialized t_line structure containing the starting
 *         position, directional steps, deltas, and initial error term.
 */
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

/**
 * @brief Draws a line from the minimap center to a target point.
 *
 * This function draws a line on the given MLX image from the minimap
 * center (MM_CENTER, MM_CENTER) to the specified endpoint (x1, y1),
 * using a Bresenham line drawing algorithm. Pixels are drawn only if
 * they fall within the minimap boundaries.
 *
 * @param img   Pointer to the MLX image where the line will be drawn.
 * @param x1    The x-coordinate of the destination point.
 * @param y1    The y-coordinate of the destination point.
 * @param color The color used to draw the line.
 */
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
