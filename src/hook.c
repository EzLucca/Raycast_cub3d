#include "../includes/game.h"

// Handle key events
void ft_hook(void* param)
{
	t_game *game;

	game = (t_game *)param;

	if (mlx_is_key_down(game->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(game->mlx);
	// Move the player with smaller steps (fluid movement)
	if (mlx_is_key_down(game->mlx, MLX_KEY_UP)) 
	{
		if (game->map[(int)(game->player->y - game->player->Speed) * mapY + (int)(game->player->x)] == 0) // Prevent moving through walls
			game->player->y -= game->player->Speed; // Move up
	}
	if (mlx_is_key_down(game->mlx, MLX_KEY_DOWN)) 
	{
		if (game->map[(int)(game->player->y + game->player->Speed) * mapX + (int)(game->player->x)] == 0) // Prevent moving through walls
			game->player->y += game->player->Speed; // Move down
	}
	if (mlx_is_key_down(game->mlx, MLX_KEY_LEFT)) 
	{
		if (game->map[(int)(game->player->y) * mapX + (int)(game->player->x - game->player->Speed)] == 0) // Prevent moving through walls
			game->player->x -= game->player->Speed; // Move left
	}
	if (mlx_is_key_down(game->mlx, MLX_KEY_RIGHT)) 
	{
		if (game->map[(int)(game->player->y) * mapX + (int)(game->player->x + game->player->Speed)] == 0) // Prevent moving through walls
			game->player->x += game->player->Speed; // Move right
	}
	if(mlx_is_key_down(game->mlx, MLX_KEY_A))
	{
		game->player->da -= 0.05f;
	}
	if(mlx_is_key_down(game->mlx, MLX_KEY_D))
	{
		game->player->da += 0.05f;
	}
	if(mlx_is_key_down(game->mlx, MLX_KEY_W))
	{
		game->player->x += game->player->dx; game->player->y += game->player->dy;
	}
	if(mlx_is_key_down(game->mlx, MLX_KEY_S))
	{
		game->player->x -= game->player->dx; game->player->y -= game->player->dy;
	}
}

// Update the game state
void update(void* param)
{
	t_game *game;

	game = (t_game *)param;
	ft_hook(game);    // Handle input
	drawMap2D(game);       
	drawPlayer(game);
	drawray(game);
}
