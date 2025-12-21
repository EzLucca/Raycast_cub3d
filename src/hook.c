#include "../includes/game.h"

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
