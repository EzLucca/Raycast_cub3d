#include "../includes/game.h"

void drawray(void *param)
{
	t_game *game;

	game = (t_game *)param;

	int mx, my, mp, dof;
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

	// Horizontal 

	dof = 0;

	// looking straight left or right 0 or 180 degrees
	if (fabs(ra) < 0.0001 || fabs(ra - M_PI) < 0.0001 || fabs(ra - 2 * M_PI) < 0.0001)
	{
		rx = center_x;
		ry = center_y;
		dof = MAX_DOF;
	}
	else
	{
		float aTan = -1 / tan(ra);
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

		if (mx < 0 || my < 0 || mx >= mapX || my >= mapY)
			break;
		mp = my * mapX + mx;
		if (game->map[mp] == 1)
			break;
		rx += xo;
		ry += yo;

		dof++;
	}

	draw_line(game->image, (int)center_x, (int)center_y, (int)rx, (int)ry, hColor);

	// vertical 

	xo = 0;
	yo = 0;

	uint32_t vColor = ft_pixel(RED);
	dof = 0;

	// vertical singularity dead-zone
	if ((ra > M_PI_2 - 0.0001 && ra < M_PI_2 + 0.0001) ||
			(ra > 3*M_PI_2 - 0.0001 && ra < 3*M_PI_2 + 0.0001))
	{
		rx = center_x;
		ry = center_y;
		dof = MAX_DOF;
	}
	else
	{
		float nTan = -tan(ra);

		if (fabs(nTan) > 1000)
		{
			rx = center_x;
			ry = center_y;
			dof = MAX_DOF;
		}
		else if (ra > M_PI_2 && ra < 3*M_PI_2) // left
		{
			rx = ((center_x / TILE_SIZE) * TILE_SIZE) - 0.0001;
			ry = (center_x - rx) * nTan + center_y;
			xo = -TILE_SIZE;
			yo = -xo * nTan;
		}
		else // right
		{
			rx = ((center_x / TILE_SIZE) * TILE_SIZE) + TILE_SIZE;
			ry = (center_x - rx) * nTan + center_y;
			xo = TILE_SIZE;
			yo = -xo * nTan;
		}
	}
	while (dof < MAX_DOF)
	{
		if (ra > M_PI_2 && ra < 3 * M_PI_2)   // left
			mx = (int)((rx - 1) / TILE_SIZE);
		else                                 // right
			mx = (int)(rx / TILE_SIZE);

		my = (int)(ry / TILE_SIZE);

		if (mx < 0 || my < 0 || mx >= mapX || my >= mapY)
			break;

		mp = my * mapX + mx;
		if (game->map[mp] == 1)
			break;

		rx += xo;
		ry += yo;
		dof++;
	}
	int end_x = (int)rx;
	int end_y = (int)ry;

	// cast width/height to int for safe comparison
	printf("end_x: %d end_y: %d\n", end_x, end_y); // DEBUG
	if (end_x < 0) end_x = 0;
	// if (end_x >= (int)game->image->width) end_x = (int)game->image->width - 1;
	//
	if (end_y < 0) end_y = 0;
	if (end_y >= (int)game->image->height) end_y = (int)game->image->height - 1;

	draw_line(game->image, center_x, center_y, end_x, end_y, vColor);
}
