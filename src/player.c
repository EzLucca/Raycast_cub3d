#include "../includes/game.h"

// Draw the player
void drawPlayer(void)
{
	// uint32_t playerColor = ft_pixel(255, 0, 0, 255);   // Player color
	uint32_t playerColor = ft_pixel(RED);   // Player color
	uint32_t dirColor    = ft_pixel(RED);   // Direction line color (green)

	player.xPos = player.x * TILE_SIZE;
	player.yPos = player.y * TILE_SIZE;

	// Draw player square
	for (int i = 0; i < player.Size; ++i)
	{
		for (int j = 0; j < player.Size; ++j)
		{
			mlx_put_pixel(image, player.xPos + i, player.yPos + j, playerColor);
		}
	}
	// --- Direction line ---
	int center_x = player.xPos + player.Size / 2;
	int center_y = player.yPos + player.Size / 2;

	player.dx = cos(player.da);
	player.dy = sin(player.da);

	int line_length = 50;

	for (int i = 0; i < line_length; ++i)
	{
		int lx = center_x + player.dx * i;
		int ly = center_y + player.dy * i;
		mlx_put_pixel(image, lx, ly, dirColor);
	}
}

