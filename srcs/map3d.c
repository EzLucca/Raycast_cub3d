#include "cub3d.h"

/**
 * @brief Calculates the horizontal texture coordinate for a wall hit.
 *
 * This function determines the x-coordinate on the texture corresponding
 * to the exact point where a ray intersects a wall. It accounts for
 * wall orientation (vertical or horizontal) and the ray's direction to
 * ensure the texture is mapped correctly.
 *
 * @param game Pointer to the game structure containing ray and player info.
 * @param tex  Pointer to the wall texture.
 * @param dist Distance from the player to the wall along the ray.
 *
 * @return The x-coordinate on the texture corresponding to the wall hit.
 */
static int	get_tex_x(t_game *game, mlx_texture_t *tex, double dist)
{
	double	wall_x;
	int		tex_x;

	if (game->ray.side == 0)
		wall_x = game->player.y + dist * game->ray.ray_dir_y;
	else
		wall_x = game->player.x + dist * game->ray.ray_dir_x;
	wall_x -= floor(wall_x);
	tex_x = (int)(wall_x * tex->width);
	if ((game->ray.side == 0 && game->ray.step_x < 0)
		|| (game->ray.side == 1 && game->ray.step_y > 0))
		tex_x = tex->width - tex_x - 1;
	return (tex_x);
}

/**
 * @brief Retrieves a pixel color from a texture.
 *
 * This function returns the color of the pixel at coordinates (x, y)
 * from the given texture. The color is packed into a 32-bit unsigned
 * integer in RGBA format.
 *
 * @param tex Pointer to the texture.
 * @param x   X-coordinate of the pixel.
 * @param y   Y-coordinate of the pixel.
 *
 * @return The 32-bit color value of the specified pixel.
 */
static	uint32_t	get_tex_color(mlx_texture_t *tex, int x, int y)
{
	uint8_t	*p;

	p = &tex->pixels[(y * tex->width + x) * tex->bytes_per_pixel];
	return ((p[0] << 24) | (p[1] << 16) | (p[2] << 8) | p[3]);
}

/**
 * @brief Draws a vertical textured wall slice.
 *
 * This function renders a single column of a wall in the 3D view. It
 * maps the appropriate portion of the wall texture based on the distance
 * from the player and the wall's vertical extent, then draws each pixel
 * in the 3D image buffer.
 *
 * @param game Pointer to the game structure containing textures and 3D image.
 * @param x    X-coordinate of the column on the screen.
 * @param dist Distance from the player to the wall along the ray.
 */
static void	draw_textured_column(t_game *game, int x, double dist)
{
	mlx_texture_t	*tex;
	int				tex_x;
	uint32_t		tex_y;
	int				y;
	int				wall_h;

	tex = get_wall_texture(game);
	tex_x = get_tex_x(game, tex, dist);
	wall_h = game->tex.wall_bottom - game->tex.wall_top + 1;
	y = game->tex.wall_top;
	while (y <= (int)game->tex.wall_bottom)
	{
		if (y >= 0 && y < (int)game->img_3d->height)
		{
			tex_y = ((y - game->tex.wall_top) * tex->height) / wall_h;
			if (tex_y >= tex->height)
				tex_y = tex->height - 1;
			mlx_put_pixel(game->img_3d, x, y,
				get_tex_color(tex, tex_x, tex_y));
		}
		y++;
	}
}

/**
 * @brief Renders the 3D view of the map.
 *
 * This function casts rays for each vertical column of the 3D image to
 * determine wall distances and heights. It calculates the correct wall
 * slice size and position for each column, performs perspective
 * correction, and draws textured wall columns using the associated
 * wall textures.
 *
 * @param game Pointer to the game structure containing the player,
 *             rays, textures, and 3D image.
 */
void	draw_map3d(t_game *game)
{
	uint32_t	x;
	double		ray_angle;
	double		dist;

	x = 0;
	while (x < game->img_3d->width)
	{
		ray_angle = game->player.da - FOV * 0.5
			+ ((double)x / game->img_3d->width) * FOV;
		cast_ray(ray_angle, game);
		dist = game->ray.dist * cos(ray_angle - game->player.da);
		if (dist < 0.1)
			dist = 0.1;
		game->tex.wall_height = (int)(game->img_3d->height / dist);
		if (game->tex.wall_height > game->img_3d->height)
			game->tex.wall_height = game->img_3d->height;
		game->tex.wall_top = (game->img_3d->height
				- game->tex.wall_height) * 0.5;
		game->tex.wall_bottom = game->tex.wall_top + game->tex.wall_height;
		draw_textured_column(game, x, dist);
		x++;
	}
}
