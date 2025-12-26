#include "../includes/game.h"

// Handle key events
void ft_hook(void* param)
{
	t_game *game;

	game = (t_game *)param;

	if (mlx_is_key_down(game->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(game->mlx);

	if (mlx_is_key_down(game->mlx, MLX_KEY_UP)) 
	{
		if (game->map[(int)(game->player->y - PLAYER_SPEED) * MAPX + (int)(game->player->x)] == 0)
			game->player->y -= PLAYER_SPEED; // Move up
	}
	if (mlx_is_key_down(game->mlx, MLX_KEY_DOWN)) 
	{
		if (game->map[(int)(game->player->y + PLAYER_SPEED) * MAPX + (int)(game->player->x)] == 0)
			game->player->y += PLAYER_SPEED; // Move down
	}
	if (mlx_is_key_down(game->mlx, MLX_KEY_LEFT)) 
	{
		if (game->map[(int)(game->player->y) * MAPX + (int)(game->player->x - PLAYER_SPEED)] == 0)
			game->player->x -= PLAYER_SPEED; // Move left
	}
	if (mlx_is_key_down(game->mlx, MLX_KEY_RIGHT)) 
	{
		if (game->map[(int)(game->player->y) * MAPX + (int)(game->player->x + PLAYER_SPEED)] == 0)
			game->player->x += PLAYER_SPEED; // Move right
	}

	// Rotation
	if(mlx_is_key_down(game->mlx, MLX_KEY_A))
	{
		game->player->da -= 0.05f;
	}
	if(mlx_is_key_down(game->mlx, MLX_KEY_D))
	{
		game->player->da += 0.05f;
	}
	// if(mlx_is_key_down(game->mlx, MLX_KEY_W))
	// {
	// 	game->player->x += game->player->dx; game->player->y += game->player->dy;
	// }
	// if(mlx_is_key_down(game->mlx, MLX_KEY_S))
	// {
	// 	game->player->x -= game->player->dx; game->player->y -= game->player->dy;
	// }
}

void clear_image(mlx_image_t *img, uint32_t color, int width, int height)
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            mlx_put_pixel(img, x, y, color);
        }
    }
}

// Update the game state
void update(void* param)
{
	t_game *game;

	game = (t_game *)param;

// Clear 3D and 2D map images
    clear_image(game->img_3d, 0x000000FF, MAP_W, MAP_H);
    clear_image(game->img_map, 0x000000FF, MAP_W, MAP_H);

	ft_hook(game);    // Handle input
	drawMap2D(game);       
	drawPlayer(game);
	drawray(game);
	drawMap3D(game);       
}
