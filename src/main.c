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

// Main function
int32_t main(void)
{
	t_game game;
	ft_memset(&game, 0, sizeof(t_game));

	game.player = &(t_player){4.5f, 5.5f, 0, 0, 0.0f, 0.0f, 0.0f, 10, 0.1f}; // Initial player position
	size_t map_size = sizeof(map) / sizeof(map[0]);

	game.map = malloc(sizeof(int) * map_size);
	if (!game.map)
		exit(EXIT_FAILURE);

	ft_memcpy(game.map, map, sizeof(map));

	// Initialize MLX
	if (!(game.mlx = mlx_init(WIDTH, HEIGHT, "MLX42", true))) {
		puts(mlx_strerror(mlx_errno));
		return EXIT_FAILURE;
	}
	// Create an game.image buffer
	if (!(game.image = mlx_new_image(game.mlx, mapX * TILE_SIZE, mapY * TILE_SIZE))) {
		mlx_close_window(game.mlx);
		puts(mlx_strerror(mlx_errno));
		return EXIT_FAILURE;
	}
	// Put the game.image to the window
	if (mlx_image_to_window(game.mlx, game.image, 0, 0) == -1) {
		mlx_close_window(game.mlx);
		puts(mlx_strerror(mlx_errno));
		return EXIT_FAILURE;
	}
	// Set up the update loop
	mlx_loop_hook(game.mlx, update, &game);
	// Start the MLX event loop
	mlx_loop(game.mlx);
	// Cleanup
	mlx_delete_image(game.mlx, game.image);
	mlx_terminate(game.mlx);
	return EXIT_SUCCESS;
}

