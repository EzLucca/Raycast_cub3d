#include "../includes/game.h"

// Draw the player
void drawPlayer(void *param)
{

	t_game *game;

	game = (t_game *)param;
	// uint32_t playerColor = ft_pixel(255, 0, 0, 255);   // Player color
	uint32_t playerColor = ft_pixel(RED);   // Player color
	uint32_t dirColor    = ft_pixel(RED);   // Direction line color (green)

	game->player->xPos = game->player->x * TILE_SIZE;
	game->player->yPos = game->player->y * TILE_SIZE;

	// Draw player square
	for (int i = 0; i < game->player->Size; ++i)
	{
		for (int j = 0; j < game->player->Size; ++j)
		{
			mlx_put_pixel(game->image, game->player->xPos + i, game->player->yPos + j, playerColor);
		}
	}
	// --- Direction line ---
	int center_x = game->player->xPos + game->player->Size / 2;
	int center_y = game->player->yPos + game->player->Size / 2;

	game->player->dx = cos(game->player->da);
	game->player->dy = sin(game->player->da);

	int line_length = 50;

	for (int i = 0; i < line_length; ++i)
	{
		int lx = center_x + game->player->dx * i;
		int ly = center_y + game->player->dy * i;
		mlx_put_pixel(game->image, lx, ly, dirColor);
	}
}
