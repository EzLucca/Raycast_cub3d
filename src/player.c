#include "../includes/game.h"

// Draw the player
void drawPlayer(void *param)
{
	t_game *game;
	uint32_t playerColor = ft_pixel(RED);   // Player color
	int i;
	int j;

	game = (t_game *)param;
	game->player->xPos = game->player->x * TILE_SIZE;
	game->player->yPos = game->player->y * TILE_SIZE;

	// Draw player square on 2dmap
	i = 0;
	while(i < PLAYER_SIZE)
	{
		j = 0;
		while(j < PLAYER_SIZE)
		{
			mlx_put_pixel(game->img_map, game->player->xPos + i, game->player->yPos + j, playerColor);
			++j;
		}
		++i;
	}
}
