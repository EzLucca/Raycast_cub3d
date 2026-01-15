/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edlucca <edlucca@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 16:16:30 by edlucca           #+#    #+#             */
/*   Updated: 2026/01/15 10:06:19 by edlucca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief Determines the step direction for raycasting.
 *
 * This function returns the step value along an axis based on the
 * direction component. If the direction is negative, the step is -1;
 * otherwise, it is 1.
 *
 * @param dir The direction component (x or y) of the ray.
 *
 * @return The step value (-1 or 1) for moving along the axis.
 */
static int	get_step(double dir)
{
	if (dir < 0)
		return (-1);
	return (1);
}

/**
 * @brief Calculates the initial side distance for DDA raycasting.
 *
 * This function computes the distance from the player’s current
 * position to the first grid line along the X or Y axis, depending
 * on the `is_x` flag. The result is used in the DDA algorithm to
 * determine which side of the map grid the ray will intersect first.
 *
 * @param game Pointer to the game structure containing player and ray info.
 * @param is_x If true, calculates distance along the X axis; if false, along Y.
 *
 * @return The distance to the next side along the specified axis.
 */
static double	get_side_dist(t_game *game, bool is_x)
{
	double	ret;

	ret = 0;
	if (is_x == true)
	{
		if (game->ray.step_x < 0)
		{
			ret = (game->player.x - game->ray.map_x) * game->ray.delta_dist_x;
			return (ret);
		}
		ret = (game->ray.map_x + 1.0 - game->player.x) * game->ray.delta_dist_x;
		return (ret);
	}
	if (game->ray.step_y < 0)
	{
		ret = (game->player.y - game->ray.map_y) * game->ray.delta_dist_y;
		return (ret);
	}
	ret = (game->ray.map_y + 1.0 - game->player.y) * game->ray.delta_dist_y;
	return (ret);
}

/**
 * @brief Initializes the raycasting structure for a single ray.
 *
 * This function sets up the ray's starting position, direction, delta
 * distances, step values, and initial side distances based on the given
 * ray angle. It prepares the ray structure for use in the DDA algorithm.
 *
 * @param ray_angle The angle of the ray relative to the player's view.
 * @param g         Pointer to the game structure containing player and ray info.
 */
static void	init_ray_struct(double ray_angle, t_game *g)
{
	g->ray.map_x = (int)g->player.x;
	g->ray.map_y = (int)g->player.y;
	g->ray.ray_dir_x = cos(ray_angle);
	g->ray.ray_dir_y = sin(ray_angle);
	g->ray.delta_dist_x = fabs(1 / g->ray.ray_dir_x);
	g->ray.delta_dist_y = fabs(1 / g->ray.ray_dir_y);
	g->ray.step_x = get_step(g->ray.ray_dir_x);
	g->ray.step_y = get_step(g->ray.ray_dir_y);
	g->ray.side_dist_x = get_side_dist(g, true);
	g->ray.side_dist_y = get_side_dist(g, false);
}

/**
 * @brief Executes the DDA algorithm to find a wall hit.
 *
 * This function performs the Digital Differential Analyzer (DDA)
 * loop, stepping through the map grid along the ray until a wall
 * is encountered. It updates the ray’s map coordinates and side
 * distances at each step.
 *
 * @param game Pointer to the game structure containing the ray and map.
 *
 * @return An integer indicating which side was hit: 0 for a vertical wall,
 *         1 for a horizontal wall.
 */
static int	perform_dda(t_game *game)
{
	int	hit;
	int	side;

	hit = 0;
	while (!hit)
	{
		if (game->ray.side_dist_x < game->ray.side_dist_y)
		{
			game->ray.side_dist_x += game->ray.delta_dist_x;
			game->ray.map_x += game->ray.step_x;
			side = 0;
		}
		else
		{
			game->ray.side_dist_y += game->ray.delta_dist_y;
			game->ray.map_y += game->ray.step_y;
			side = 1;
		}
		hit = is_wall(game, game->ray.map_x, game->ray.map_y);
	}
	return (side);
}

/**
 * @brief Casts a single ray and calculates the distance to the first wall.
 *
 * This function initializes the ray structure, performs the DDA algorithm
 * to detect the first wall hit, and computes the distance from the player
 * to that wall. The distance is corrected based on whether a vertical or
 * horizontal wall was hit.
 *
 * @param ray_angle The angle of the ray relative to the player's viewing
 * direction.
 * @param game      Pointer to the game structure containing player, map,
 * and ray info.
 */
void	cast_ray(double ray_angle, t_game *game)
{
	int		side;
	double	h_ray;
	double	v_ray;
	double	p_to_grid_x;
	double	p_to_grid_y;

	init_ray_struct(ray_angle, game);
	side = perform_dda(game);
	game->ray.side = side;
	p_to_grid_x = game->ray.map_x - game->player.x;
	p_to_grid_y = game->ray.map_y - game->player.y;
	v_ray = (p_to_grid_x + (1 - game->ray.step_x) * 0.5) / game->ray.ray_dir_x;
	h_ray = (p_to_grid_y + (1 - game->ray.step_y) * 0.5) / game->ray.ray_dir_y;
	if (side == 0)
		game->ray.dist = v_ray;
	else
		game->ray.dist = h_ray;
}
