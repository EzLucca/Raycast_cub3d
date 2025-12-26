#include "../includes/game.h"

// Convert RGB to 32-bit integer
int32_t ft_pixel(int32_t r, int32_t g, int32_t b, int32_t a) // use uint8_t
{
	return (r << 24 | g << 16 | b << 8 | a);
}

// Draw the map
void drawMap2D(void *param)
{
	t_game *game;

	game = (t_game *)param;
	int x, y;
	int tile_size = TILE_SIZE;

	for (y = 0; y < MAPY; ++y)
	{
		for (x = 0; x < MAPX; ++x)
		{
			int tile = game->map[y * MAPX + x];
			int color = (tile == 1) ? ft_pixel(WHITE) : ft_pixel(BLACK);

			// Draw the tile at the correct position (x, y)
			for (int i = 0; i < tile_size; ++i)
			{
				for (int j = 0; j < tile_size; ++j)
				{
					mlx_put_pixel(game->img_map, x * tile_size + i, y * tile_size + j, color);
				}
			}
			int border_color = (tile == 1) ? ft_pixel(BLACK) : ft_pixel(WHITE);

			for (int i = 0; i < tile_size; ++i) 
			{
				mlx_put_pixel(game->img_map, x * tile_size + i, y * tile_size, border_color);
				mlx_put_pixel(game->img_map, x * tile_size + i, (y + 1) * tile_size - 1, border_color);
			}
			// Left and right borders
			for (int j = 0; j < tile_size; ++j) 
			{
				mlx_put_pixel(game->img_map, x * tile_size, y * tile_size + j, border_color);
				mlx_put_pixel(game->img_map, (x + 1) * tile_size - 1, y * tile_size + j, border_color);
			}
		}
	}
}

double cast_ray(double ray_angle, t_game *g)
{
	int map_x = (int)g->player->x;
	int map_y = (int)g->player->y;

	double ray_dir_x = cos(ray_angle);
	double ray_dir_y = sin(ray_angle);

	double delta_dist_x = fabs(1 / ray_dir_x);
	double delta_dist_y = fabs(1 / ray_dir_y);

	int step_x = ray_dir_x < 0 ? -1 : 1;
	int step_y = ray_dir_y < 0 ? -1 : 1;

	double side_dist_x = (step_x < 0 ? g->player->x - map_x : map_x + 1.0 - g->player->x) * delta_dist_x;
	double side_dist_y = (step_y < 0 ? g->player->y - map_y : map_y + 1.0 - g->player->y) * delta_dist_y;

	int hit = 0;
	int side;

	while (!hit)
	{
		if (side_dist_x < side_dist_y)
		{
			side_dist_x += delta_dist_x;
			map_x += step_x;
			side = 0;
		}
		else
		{
			side_dist_y += delta_dist_y;
			map_y += step_y;
			side = 1;
		}

		if (map_y < 0 || map_y >= MAPY || map_x < 0 || map_x >= MAPX)
			return 1000; // far away to prevent crash

		if (g->map[map_y * MAPX + map_x] == 1)
			hit = 1;
	}

	if (side == 0)
		return (map_x - g->player->x + (1 - step_x) / 2) / ray_dir_x;
	else
		return (map_y - g->player->y + (1 - step_y) / 2) / ray_dir_y;
}


void draw_column(mlx_image_t *img, int x, int top, int bottom, uint32_t color)
{
	for (int y = top; y <= bottom; y++)
		mlx_put_pixel(img, x, y, color);
}

void drawMap3D(void *param)
{
	t_game	*game;
	double	dist;
	int		wall_height;
	int		wall_top;
	int		wall_bottom;

	game = (t_game *)param;

	for (int x = 0; x < MAP_W; x++)
	{
		double ray_angle = game->player->da - (double)FOV / 2
			+ x * (FOV / MAP_W);

		dist = cast_ray(ray_angle, game);

		// fisheye correction
		dist *= cos(ray_angle - game->player->da);

		// prevent extreme wall heights
		if (dist < 0.01)
			dist = 0.01;

		wall_height = (int)(MAP_H / dist);
		wall_top = (MAP_H / 2) - (wall_height / 2);
		wall_bottom = (MAP_H / 2) + (wall_height / 2);

		// clamp to screen
		if (wall_top < 0) wall_top = 0;
		if (wall_bottom >= MAP_H) wall_bottom = MAP_H - 1;

		draw_column(game->img_3d, x, wall_top, wall_bottom, 0x0000FFFF);
	}
}
