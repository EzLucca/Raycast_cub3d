#include "../includes/game.h"

// Define the map
int map[] =
{
	1,1,1,1,1,1,1,1,
	1,0,0,1,0,0,0,1,
	1,0,0,1,0,0,0,1,
	1,0,0,1,1,0,0,1,
	1,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,1,
	1,1,1,1,1,1,1,1
};

// Draw the map
void drawMap2D(void)
{
	int x, y;
	int tile_size = TILE_SIZE;

	// Loop through the map array and draw it
	for (y = 0; y < mapY; ++y)
	{
		for (x = 0; x < mapX; ++x)
		{
			int tile = map[y * mapX + x];
			int color = (tile == 1) ? ft_pixel(WHITE) : ft_pixel(BLACK); // white for walls and black for other

			// Draw the tile at the correct position (x, y)
			for (int i = 0; i < tile_size; ++i)
			{
				for (int j = 0; j < tile_size; ++j)
				{
					mlx_put_pixel(image, x * tile_size + i, y * tile_size + j, color);
				}
			}
			// Draw the borders (delimitations) for each tile (thin black border around each tile)
			int border_color = (tile == 1) ? ft_pixel(BLACK) : ft_pixel(WHITE); // Black border for wall and white
																				// Top and bottom borders
			for (int i = 0; i < tile_size; ++i) 
			{
				mlx_put_pixel(image, x * tile_size + i, y * tile_size, border_color); // Top border
				mlx_put_pixel(image, x * tile_size + i, (y + 1) * tile_size - 1, border_color); // Bottom border
			}
			// Left and right borders
			for (int j = 0; j < tile_size; ++j) 
			{
				mlx_put_pixel(image, x * tile_size, y * tile_size + j, border_color); // Left border
				mlx_put_pixel(image, (x + 1) * tile_size - 1, y * tile_size + j, border_color); // Right border
			}
		}
	}
}
