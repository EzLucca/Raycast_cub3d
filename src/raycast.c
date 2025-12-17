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

t_player player = {3.0f, 3.0f, 0.0f, 0.0f, 0.0f}; // Initial player position

void *image = NULL;

// Convert RGB to 32-bit integer
int32_t ft_pixel(int32_t r, int32_t g, int32_t b, int32_t a)
{
	return (r << 24 | g << 16 | b << 8 | a);
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
			int color = (tile == 1) ? ft_pixel(255, 255, 255, 255) : ft_pixel(0, 0, 0, 255); // Red for walls, Blue for empty

			// Draw the tile at the correct position (x, y)
			for (int i = 0; i < tile_size; ++i)
			{
				for (int j = 0; j < tile_size; ++j)
				{
					mlx_put_pixel(image, x * tile_size + i, y * tile_size + j, color);
				}
			}
			// Draw the borders (delimitations) for each tile (thin black border around each tile)
			int border_color = (tile == 1) ? ft_pixel(0, 0, 0, 255) : ft_pixel(255, 255, 255, 255); // Black border
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
	uint32_t playerColor = ft_pixel(255, 0, 0, 255);   // Player color
	uint32_t dirColor    = ft_pixel(0, 255, 0, 255);   // Direction line color (green)

	int playerSize = 10;
	int xPos = player.x * TILE_SIZE;
	int yPos = player.y * TILE_SIZE;

	// Draw player square
	for (int i = 0; i < playerSize; ++i)
	{
		for (int j = 0; j < playerSize; ++j)
		{
			mlx_put_pixel(image, xPos + i, yPos + j, playerColor);
		}
	}
	// --- Direction line ---
	int center_x = xPos + playerSize / 2;
	int center_y = yPos + playerSize / 2;

	float dx = cos(player.da);
	float dy = sin(player.da);

	int line_length = 80;

	for (int i = 0; i < line_length; ++i)
	{
		int lx = center_x + dx * i;
		int ly = center_y + dy * i;
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
		if (map[(int)(player.y - playerSpeed) * mapX + (int)(player.x)] == 0) // Prevent moving through walls
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

