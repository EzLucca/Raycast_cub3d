#include "../includes/game.h"

int	draw_horizontal(t_game *game);
int	draw_vertical(t_game *game);

static float	dist(float ax, float ay, float bx, float by)
{
	return (sqrtf((bx - ax) * (bx - ax) + (by - ay) * (by - ay)));
}

void drawray(void *param)
{
	t_game *game = param;
	float ray_angle;
	float angle_step;
	int   r;

	angle_step = FOV / NUM_RAYS;
	ray_angle = game->player->da - (FOV / 2);

	for (r = 0; r < NUM_RAYS; r++)
	{
		ft_memset(&game->ray, 0, sizeof(t_raycast));

		game->ray.center_x = game->player->xPos + PLAYER_SIZE / 2;
		game->ray.center_y = game->player->yPos + PLAYER_SIZE / 2;
		game->ray.ra = ray_angle;

		// normalize
		while (game->ray.ra < 0)
			game->ray.ra += 2 * M_PI;
		while (game->ray.ra >= 2 * M_PI)
			game->ray.ra -= 2 * M_PI;

		game->ray.dist_h = draw_horizontal(game);
		game->ray.dist_v = draw_vertical(game);

		if (game->ray.dist_h < game->ray.dist_v)
			draw_line(game->img_map,
					game->ray.center_x, game->ray.center_y,
					game->ray.h_rx, game->ray.h_ry,
					ft_pixel(RED));
		else
			draw_line(game->img_map,
					game->ray.center_x, game->ray.center_y,
					game->ray.v_rx, game->ray.v_ry,
					ft_pixel(RED));

		ray_angle += angle_step;
	}
}

int	draw_horizontal(t_game *game)
{
	// Horizontal 
	float dist_h;

	// looking straight left or right 0 or 180 degrees
	if (fabs(game->ray.ra) < 0.0001 || fabs(game->ray.ra - M_PI) < 0.0001 || fabs(game->ray.ra - 2 * M_PI) < 0.0001)
	{
		game->ray.h_rx = game->ray.center_x;
		game->ray.h_ry = game->ray.center_y;
		game->ray.dof = MAX_DOF;
	}
	else
	{
		float aTan = -1 / tan(game->ray.ra);
		// looking up
		if (game->ray.ra > M_PI)
		{
			game->ray.h_ry = ((game->ray.center_y / TILE_SIZE) * TILE_SIZE) - 0.0001;
			game->ray.h_rx = (game->ray.center_y - game->ray.h_ry) * aTan + game->ray.center_x;

			game->ray.yo = -TILE_SIZE;
			game->ray.xo = -game->ray.yo * aTan;
		}
		// looking down
		else if (game->ray.ra < M_PI)
		{
			game->ray.h_ry = ((game->ray.center_y / TILE_SIZE) * TILE_SIZE) + TILE_SIZE;
			game->ray.h_rx = (game->ray.center_y - game->ray.h_ry) * aTan + game->ray.center_x;

			game->ray.yo = TILE_SIZE;
			game->ray.xo = -game->ray.yo * aTan;
		}
	}

	while (game->ray.dof < MAX_DOF)
	{
		game->ray.mx = (int)(game->ray.h_rx / TILE_SIZE);

		if (game->ray.ra > M_PI)        // looking up
			game->ray.my = (int)((game->ray.h_ry - 1) / TILE_SIZE);
		else                // looking down
			game->ray.my = (int)(game->ray.h_ry / TILE_SIZE);

		if (game->ray.mx < 0 || game->ray.my < 0 || game->ray.mx >= MAPX || game->ray.my >= MAPY)
			break;
		game->ray.mp = game->ray.my * MAPX + game->ray.mx;
		if (game->map[game->ray.mp] == 1)
			break;
		game->ray.h_rx += game->ray.xo;
		game->ray.h_ry += game->ray.yo;

		game->ray.dof++;
	}

	dist_h = dist(
			game->ray.center_x,
			game->ray.center_y,
			game->ray.h_rx,
			game->ray.h_ry
			);
	return(dist_h);
}

int	draw_vertical(t_game *game)
{
	// vertical 

	float dist_v;

	game->ray.xo = 0;
	game->ray.yo = 0;
	game->ray.dof = 0;

	// vertical singularity dead-zone
	if ((game->ray.ra > M_PI_2 - 0.0001 && game->ray.ra < M_PI_2 + 0.0001) ||
			(game->ray.ra > 3*M_PI_2 - 0.0001 && game->ray.ra < 3*M_PI_2 + 0.0001))
	{
		game->ray.v_rx = game->ray.center_x;
		game->ray.v_ry = game->ray.center_y;
		game->ray.dof = MAX_DOF;
	}
	else
	{
		float nTan = -tan(game->ray.ra);

		if (fabs(nTan) > 1000)
		{
			game->ray.v_rx = game->ray.center_x;
			game->ray.v_ry = game->ray.center_y;
			game->ray.dof = MAX_DOF;
		}
		else if (game->ray.ra > M_PI_2 && game->ray.ra < 3*M_PI_2) // left
		{
			game->ray.v_rx = ((game->ray.center_x / TILE_SIZE) * TILE_SIZE) - 0.0001;
			game->ray.v_ry = (game->ray.center_x - game->ray.v_rx) * nTan + game->ray.center_y;
			game->ray.xo = -TILE_SIZE;
			game->ray.yo = -game->ray.xo * nTan;
		}
		else // right
		{
			game->ray.v_rx = ((game->ray.center_x / TILE_SIZE) * TILE_SIZE) + TILE_SIZE;
			game->ray.v_ry = (game->ray.center_x - game->ray.v_rx) * nTan + game->ray.center_y;
			game->ray.xo = TILE_SIZE;
			game->ray.yo = -game->ray.xo * nTan;
		}
	}
	while (game->ray.dof < MAX_DOF)
	{
		if (game->ray.ra > M_PI_2 && game->ray.ra < 3 * M_PI_2)   // left
			game->ray.mx = (int)((game->ray.v_rx - 1) / TILE_SIZE);
		else                               // right
			game->ray.mx = (int)(game->ray.v_rx / TILE_SIZE);

		game->ray.my = (int)(game->ray.v_ry / TILE_SIZE);

		if (game->ray.mx < 0 || game->ray.my < 0 || game->ray.mx >= MAPX || game->ray.my >= MAPY)
			break;

		game->ray.mp = game->ray.my * MAPX + game->ray.mx;
		if (game->map[game->ray.mp] == 1)
			break;

		game->ray.v_rx += game->ray.xo;
		game->ray.v_ry += game->ray.yo;
		game->ray.dof++;
	}
	// cast width/height to int for safe comparison
	// printf("end_x: %d game->ray.v_ry: %d\n", end_x, game->ray.v_ry); // DEBUG
	if (game->ray.v_rx < 0) game->ray.v_rx = 0;
	if (game->ray.v_rx >= (int)game->img_map->width) game->ray.v_rx = (int)game->img_map->width - 1;
	if (game->ray.v_ry < 0) game->ray.v_ry = 0;
	if (game->ray.v_ry >= (int)game->img_map->height) game->ray.v_ry = (int)game->img_map->height - 1;

	dist_v = dist(
			game->ray.center_x,
			game->ray.center_y,
			game->ray.v_rx,
			game->ray.v_ry
			);
	return(dist_v);
}
