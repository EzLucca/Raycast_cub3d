#include "../includes/game.h"

float playerSpeed = 0.1f; 

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

t_player player = {4.5f, 5.5f, 0, 0, 0.0f, 0.0f, 0.0f, 10}; // Initial player position

void *image = NULL;

// Convert RGB to 32-bit integer
int32_t ft_pixel(int32_t r, int32_t g, int32_t b, int32_t a) // use uint8_t
{
	return (r << 24 | g << 16 | b << 8 | a);
}

void draw_line(mlx_image_t *img, int x0, int y0, int x1, int y1, uint32_t color)
{
	int dx = abs(x1 - x0);
	int dy = abs(y1 - y0);

	int sx = x0 < x1 ? 1 : -1;
	int sy = y0 < y1 ? 1 : -1;

	int err = dx - dy;

	while (1)
	{
		mlx_put_pixel(img, x0, y0, color);
		if (x0 == x1 && y0 == y1)
			break;
		int e2 = 2 * err;
		if (e2 > -dy) { err -= dy; x0 += sx; }
		if (e2 <  dx) { err += dx; y0 += sy; }
	}
}

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

// Handle key events
void ft_hook(void* param)
{
	mlx_t* mlx = param;

	if (mlx_is_key_down(mlx, MLX_KEY_ESCAPE))
		mlx_close_window(mlx);
	// Move the player with smaller steps (fluid movement)
	if (mlx_is_key_down(mlx, MLX_KEY_UP)) 
	{
		if (map[(int)(player.y - playerSpeed) * mapY + (int)(player.x)] == 0) // Prevent moving through walls
			player.y -= playerSpeed; // Move up
	}
	if (mlx_is_key_down(mlx, MLX_KEY_DOWN)) 
	{
		if (map[(int)(player.y + playerSpeed) * mapX + (int)(player.x)] == 0) // Prevent moving through walls
			player.y += playerSpeed; // Move down
	}
	if (mlx_is_key_down(mlx, MLX_KEY_LEFT)) 
	{
		if (map[(int)(player.y) * mapX + (int)(player.x - playerSpeed)] == 0) // Prevent moving through walls
			player.x -= playerSpeed; // Move left
	}
	if (mlx_is_key_down(mlx, MLX_KEY_RIGHT)) 
	{
		if (map[(int)(player.y) * mapX + (int)(player.x + playerSpeed)] == 0) // Prevent moving through walls
			player.x += playerSpeed; // Move right
	}
	if(mlx_is_key_down(mlx, MLX_KEY_A))
	{
		player.da -= 0.05f;
	}
	if(mlx_is_key_down(mlx, MLX_KEY_D))
	{
		player.da += 0.05f;
	}
	if(mlx_is_key_down(mlx, MLX_KEY_W))
	{
		player.x += player.dx; player.y += player.dy;
	}
	if(mlx_is_key_down(mlx, MLX_KEY_S))
	{
		player.x -= player.dx; player.y -= player.dy;
	}
}

// Update the game state
void update(void* param)
{
	ft_hook(param);    // Handle input
	drawMap2D();       
	drawPlayer();
	drawray();
}

// Main function
int32_t main(void)
{
	mlx_t* mlx;

	// Initialize MLX
	if (!(mlx = mlx_init(WIDTH, HEIGHT, "MLX42", true))) {
		puts(mlx_strerror(mlx_errno));
		return EXIT_FAILURE;
	}
	// Create an image buffer
	if (!(image = mlx_new_image(mlx, mapX * TILE_SIZE, mapY * TILE_SIZE))) {
		mlx_close_window(mlx);
		puts(mlx_strerror(mlx_errno));
		return EXIT_FAILURE;
	}
	// Put the image to the window
	if (mlx_image_to_window(mlx, image, 0, 0) == -1) {
		mlx_close_window(mlx);
		puts(mlx_strerror(mlx_errno));
		return EXIT_FAILURE;
	}
	// Set up the update loop
	mlx_loop_hook(mlx, update, mlx);
	// Start the MLX event loop
	mlx_loop(mlx);
	// Cleanup
	mlx_delete_image(mlx, image);
	mlx_terminate(mlx);
	return EXIT_SUCCESS;
}

