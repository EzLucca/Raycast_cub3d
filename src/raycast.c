#include "../includes/game.h"

void drawray(void)
{
	int r, mx, my, mp, dof;
	float rx, ry, ra, xo, yo;
	uint32_t dirColor = ft_pixel(GREEN);
	int center_x = player.xPos + player.Size / 2;
	int center_y = player.yPos + player.Size / 2;

	ra = player.da;
	yo = 0;

	// normalize angle
	while (ra < 0)
		ra += 2 * M_PI;
	while (ra > 2 * M_PI)
		ra -= 2 * M_PI;

	float aTan = -1 / tan(ra);
	for (r = 0; r < 1; r++)
	{
		dof = 0;

		// looking straight left or right
		if (fabs(ra) < 0.0001 || fabs(ra - M_PI) < 0.0001)
		{
			rx = player.xPos;
			ry = player.yPos;
			dof = MAX_DOF;
		}
		else
		{
			// looking up
			if (ra > M_PI)
			{
				ry = (((int)player.yPos / TILE_SIZE) * TILE_SIZE) - 0.0001;
				rx = (player.yPos - ry) * aTan + player.xPos;

				yo = -TILE_SIZE;
				xo = -yo * aTan;
			}
			// looking down
			else if (ra < M_PI)
			{
				ry = (((int)player.yPos / TILE_SIZE) * TILE_SIZE) + TILE_SIZE;
				rx = (player.yPos - ry) * aTan + player.xPos;

				yo = TILE_SIZE;
				xo = -yo * aTan;
			}
		}
		while (dof < MAX_DOF)
		{
			mx = (int)(rx) >> 6;
			my = (int)(ry) >> 6;
			mp = my * mapX + mx;

			if (mx < 0 || my < 0 || mx >= mapX || my >= mapY)
				break;
			if (map[mp] == 1)
				break;
			rx += xo;
			ry += yo;

			dof++;
		}

		draw_line(image, (int)center_x, (int)center_y, (int)rx, (int)ry, dirColor);
		// rx and ry need to check if is a wall.
	}
}
