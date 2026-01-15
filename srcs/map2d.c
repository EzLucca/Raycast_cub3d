#include "cub3d.h"

/**
 * @brief Prepares minimap coordinates for a single tile.
 *
 * This function calculates the scaled screen coordinates of a given
 * map tile relative to the player's position. It sets the start and
 * end positions for rendering the tile on the minimap and stores the
 * tile type.
 *
 * @param x    X-coordinate of the tile in the map grid.
 * @param y    Y-coordinate of the tile in the map grid.
 * @param game Pointer to the game structure.
 */
static void minimap_prepare_tile(int x, int y, t_game *game)
{
	double cam_x = game->player.x;
	double cam_y = game->player.y;

	game->map2d.tile = game->map[y][x];

	int max_width = (game->draw_mode == DRAW_3D) ? MINIMAP_SIZE : game->window_width;
	int max_height = (game->draw_mode == DRAW_2D) ? game->window_height : MINIMAP_SIZE;
	
	double rel_x = (double)x - cam_x;
	double rel_y = (double)y - cam_y;
    int img_width = game->img_map->width * 0.5;
    int img_height = game->img_map->height * 0.5;

	if (game->draw_mode == DRAW_3D)
	{
		// double rel_x = (double)x - cam_x;
		// double rel_y = (double)y - cam_y;
		game->map2d.start_x = (int)(rel_x * game->map2d.scale_x + MM_CENTER);
		game->map2d.start_y = (int)(rel_y * game->map2d.scale_y + MM_CENTER);
		game->map2d.end_x = (int)((rel_x + 1) * game->map2d.scale_x + MM_CENTER);
		game->map2d.end_y = (int)((rel_y + 1) * game->map2d.scale_y + MM_CENTER);
	}
	else // DRAW_2D fullscreen
	{
		// double rel_x = (double)x - cam_x;
		// double rel_y = (double)y - cam_y;
		game->map2d.start_x = (int)(rel_x * game->map2d.scale_x + img_width);
		game->map2d.start_y = (int)(rel_y * game->map2d.scale_y + img_height);
		game->map2d.end_x = (int)((rel_x + 1) * game->map2d.scale_x + img_width);
		game->map2d.end_y = (int)((rel_y + 1) * game->map2d.scale_y + img_height);
	}

	// Clamp coordinates inside the image
	if (game->map2d.start_x < 0) game->map2d.start_x = 0;
	if (game->map2d.end_x > max_width) game->map2d.end_x = max_width;
	if (game->map2d.start_y < 0) game->map2d.start_y = 0;
	if (game->map2d.end_y > max_height) game->map2d.end_y = max_height;
}

/**
 * @brief Draws a single tile on the minimap.
 *
 * This function renders a map tile onto the minimap image using its
 * precomputed screen coordinates. Tiles that are completely outside
 * the minimap boundaries are skipped. The color of the tile is
 * determined by the `tile_color` function.
 *
 * @param game Pointer to the game structure.
 */
static void minimap_draw_tile_img(t_game *game, mlx_image_t *img)
{
	int color;
	int py;
	int px;
	int max_width;
	int max_height;

	// Determine drawing bounds based on mode
	if (game->draw_mode == DRAW_3D)
	{
		max_width = MINIMAP_SIZE;
		max_height = MINIMAP_SIZE;
	}
	else // DRAW_2D
	{
		max_width = game->window_width;
		max_height = game->window_height;
	}

	// Skip tiles completely outside the drawable area
	if (game->map2d.end_x < 0 || game->map2d.start_x >= max_width
			|| game->map2d.end_y < 0 || game->map2d.start_y >= max_height)
		return;

	color = tile_color(game);

	py = game->map2d.start_y;
	while (py < game->map2d.end_y)
	{
		if (py >= 0 && py < max_height)
		{
			px = game->map2d.start_x;
			while (px < game->map2d.end_x)
			{
				if (px >= 0 && px < max_width)
					mlx_put_pixel(img, px, py, color);
				px++;
			}
		}
		py++;
	}
}

/**
 * @brief Draws the player's field of view rays on the minimap.
 *
 * This function casts multiple rays corresponding to the player's
 * current viewing angle and field of view. Each ray is projected onto
 * the minimap using scaled coordinates, and a line is drawn from the
 * minimap center to the ray endpoint. The ray color is set to red.
 *
 * @param g Pointer to the game structure.
 */
static void	minimap_draw_ray(t_game *g)
{
	int		r;
	double	angle;
	int		x;
	int		y;
	int		color;

	r = 0;
	color = set_color(g, RED, TRANSPARENT);
	while (r < NUM_RAYS)
	{
		angle = g->player.da - (double)FOV * 0.5 + r * ANGULAR_STEP;
		cast_ray(angle, g);
		x = (int)(g->ray.ray_dir_x * g->ray.dist * g->map2d.scale_x
				+ MM_CENTER);
		y = (int)(g->ray.ray_dir_y * g->ray.dist * g->map2d.scale_y
				+ MM_CENTER);
		draw_line(g->img_map, x, y, color);
		r++;
	}
}

/**
 * @brief Draws the player's field-of-view rays on the 2D fullscreen map.
 *
 * This function casts multiple rays originating from the player's position,
 * corresponding to the current viewing angle (`player.da`) and the field of
 * view (`FOV`). Each ray is projected onto the 2D map image (`img_map`) and
 * drawn as a line in world coordinates, scaled according to `map2d.scale_x`
 * and `map2d.scale_y`.
 *
 * The rays are only drawn in `DRAW_2D` mode; in other modes, the function
 * does nothing.
 *
 * @param g Pointer to the game structure containing player state, raycasting
 *          data, map scaling factors, and the image to draw on.
 */
void map2d_draw_rays(t_game *g)
{
	int r;
	double angle;
	int px, py, x, y;

	if (g->draw_mode != DRAW_2D)
		return;

	px = g->img_map->width * 0.5;
	py = g->img_map->height * 0.5;

	for (r = 0; r < NUM_RAYS; r++)
	{
		angle = g->player.da - FOV * 0.5 + r * ANGULAR_STEP;
		cast_ray(angle, g);

		x = (int)(px + g->ray.ray_dir_x * g->ray.dist * g->map2d.scale_x);
		y = (int)(py + g->ray.ray_dir_y * g->ray.dist * g->map2d.scale_y);

		draw_line_world(g->img_map, px, py, x, y, RED);
	}
}

/**
 * @brief Renders the entire minimap.
 *
 * This function draws the minimap for the current frame. It sets the
 * scaling factors, renders the background, iterates over all map
 * tiles to prepare and draw them, draws the player position, projects
 * the player's field of view rays, and finally draws the minimap border.
 *
 * @param param Pointer to the game structure.
 */
void draw_map2d(void *param)
{
	t_game *game = (t_game *)param;
	int y;
	int x;

	/* ---------- Set scale ---------- */
	if (game->draw_mode == DRAW_3D)
	{
		game->map2d.scale_x = (double)MINIMAP_SIZE / 20;
		game->map2d.scale_y = (double)MINIMAP_SIZE / 20;
	}
	else if (game->draw_mode == DRAW_2D)
	{
		game->map2d.scale_x = (double)game->window_width / 20;
		game->map2d.scale_y = (double)game->window_height / 20;
	}

	/* ---------- Background ---------- */
	draw_minimap_background(game);

	/* ---------- Tiles ---------- */
	y = 0;
	while (y < game->map_h)
	{
		x = 0;
		while (x < game->map_w)
		{
			minimap_prepare_tile(x, y, game);
			minimap_draw_tile_img(game, game->img_map);
			x++;
		}
		y++;
	}

	/* ---------- Player ---------- */
	draw_player(game);

	/* ---------- Rays & border ---------- */
	if (game->draw_mode == DRAW_3D)
	{
		minimap_draw_ray(game);      // player-centered rays
		draw_minimap_border(game);
	}
	else if (game->draw_mode == DRAW_2D)
	{
		map2d_draw_rays(game);       // world-space rays
	}
}
