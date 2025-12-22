#include "../includes/game.h"

void drawray(void *param)
{
	t_game *game;

	game = (t_game *)param;

	int r, mx, my, mp, dof;
	float rx, ry, ra, xo, yo;
	uint32_t hColor = ft_pixel(GREEN);
	int center_x = game->player->xPos + game->player->Size / 2;
	int center_y = game->player->yPos + game->player->Size / 2;

	ra = game->player->da;
	xo = 0;
	yo = 0;

	// normalize angle
	while (ra < 0)
		ra += 2 * M_PI;
	while (ra >= 2 * M_PI)
		ra -= 2 * M_PI;

	float aTan = -1 / tan(ra);
	// Horizontal 
	for (r = 0; r < 1; r++)
	{
		dof = 0;

		// looking straight left or right
		if (fabs(ra) < 0.0001 || fabs(ra - M_PI) < 0.0001)
		{
			rx = center_x;
			ry = center_y;
			dof = MAX_DOF;
		}
		else
		{
			// looking up
			if (ra > M_PI)
			{
				ry = ((center_y / TILE_SIZE) * TILE_SIZE) - 0.0001;
				rx = (center_y - ry) * aTan + center_x;

				yo = -TILE_SIZE;
				xo = -yo * aTan;
			}
			// looking down
			else if (ra < M_PI)
			{
				ry = ((center_y / TILE_SIZE) * TILE_SIZE) + TILE_SIZE;
				rx = (center_y - ry) * aTan + center_x;

				yo = TILE_SIZE;
				xo = -yo * aTan;
			}
		}

		while (dof < MAX_DOF)
		{
			mx = (int)(rx / TILE_SIZE);

			if (ra > M_PI)        // looking up
				my = (int)((ry - 1) / TILE_SIZE);
			else                  // looking down
				my = (int)(ry / TILE_SIZE);
			// mx = ((int)(rx) / TILE_SIZE);
			// my = ((int)(ry) / TILE_SIZE);
			mp = my * mapX + mx;

			if (mx < 0 || my < 0 || mx >= mapX || my >= mapY)
				break;
			if (game->map[mp] == 1)
				break;
			rx += xo;
			ry += yo;

			dof++;
		}

		draw_line(game->image, (int)center_x, (int)center_y, (int)rx, (int)ry, hColor);
		// rx and ry need to check if is a wall.
	}
	// vertical 
	uint32_t vColor = ft_pixel(RED);
	float nTan = -tan(ra);
	// Horizontal 
	for (r = 0; r < 1; r++)
	{
		dof = 0;

		// looking straight left or right
		if (fabs(ra) < 0.0001 || fabs(ra - M_PI) < 0.0001)
		{
			rx = center_x;
			ry = center_y;
			dof = MAX_DOF;
		}
		// else
		// {
		// looking up
		if (ra > 2*M_PI && ra < 3*M_PI)
		{
			rx = ((center_x / TILE_SIZE) * TILE_SIZE) - 0.0001;
			ry = (center_x - rx) * nTan + center_y;

			xo = -TILE_SIZE;
			yo = -xo * nTan;
		}
		// looking down
		if (ra < 2*M_PI || ra > 3*M_PI)
		{
			rx = ((center_x / TILE_SIZE) * TILE_SIZE) + TILE_SIZE;
			ry = (center_x - rx) * nTan + center_y;

			xo = TILE_SIZE;
			yo = -xo * nTan;
		}
		// }

		while (dof < MAX_DOF)
		{
			mx = (int)(rx / TILE_SIZE);

			// if (ra > M_PI)        // looking up
			// 	my = (int)((ry - 1) / TILE_SIZE);
			// else                  // looking down
			my = (int)(ry / TILE_SIZE);
			mp = my * mapX + mx;

			if (mx < 0 || my < 0 || mx >= mapX || my >= mapY)
				break;
			if (game->map[mp] == 1)
				break;
			rx += xo;
			ry += yo;

			dof++;
		}

		draw_line(game->image, (int)center_x, (int)center_y, (int)rx, (int)ry, vColor);
		// rx and ry need to check if is a wall.
	}
}
