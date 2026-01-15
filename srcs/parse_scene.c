/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_scene.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgueon <jgueon@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/04 17:24:02 by jgueon            #+#    #+#             */
/*   Updated: 2026/01/14 00:35:41 by jgueon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief Initialize a t_game struct for the parsing phase (safe default state).
 *
 * This function prepares @p g so parsing can reliably detect missing
 * 	identifiers
 * and so cleanup can safely free fields that may still be NULL.
 *
 * What it does:
 * - Zeroes the full struct with ft_memset.
 * - Sets floor and ceiling RGB components to -1 to represent “unset”.
 * - Clears player-related fields (player_found, player_dir).
 * - Initializes default window dimensions (WIN_W / WIN_H).
 *
 * @param g Game struct to initialize.
 *
 * @return None.
 */
void	init_parser_game(t_game *g)
{
	if (!g)
		return ;
	ft_memset(g, 0, sizeof(t_game));
	g->floor.r = -1;
	g->floor.g = -1;
	g->floor.b = -1;
	g->ceiling.r = -1;
	g->ceiling.g = -1;
	g->ceiling.b = -1;
	g->player_found = 0;
	g->player_dir = '\0';
	g->window_width = WIN_W;
	g->window_height = WIN_H;
}

/**
 * @brief Free fields allocated during parsing and reset the game struct state.
 *
 * This cleanup function releases parsing-owned allocations and resets related
 * fields so the struct is left in a predictable empty state.
 *
 * Frees:
 * - Texture path strings (game->tex.no/so/we/ea).
 * - The padded map grid (game->map) if present (via free_map()).
 *
 * Resets:
 * - Texture pointers to NULL.
 * - Map pointer to NULL and dimensions to 0.
 * - Player fields back to defaults.
 *
 * @param game Game struct to clean.
 *
 * @return None.
 */
void	free_parser_game(t_game *game)
{
	if (!game)
		return ;
	free(game->tex.no);
	free(game->tex.so);
	free(game->tex.we);
	free(game->tex.ea);
	game->tex.no = NULL;
	game->tex.so = NULL;
	game->tex.we = NULL;
	game->tex.ea = NULL;
	if (game->map)
		free_map(game->map);
	game->map = NULL;
	game->map_w = 0;
	game->map_h = 0;
	game->player_found = 0;
	game->player_dir = '\0';
	game->player_x = 0;
	game->player_y = 0;
}

/**
 * @brief Parse a full .cub scene file (metadata + map) into a t_game struct.
 *
 * High-level flow:
 * - Initialize parser state in @p game (init_parser_game).
 * - Reset get_line() internal buffer state (get_line_reset).
 * - Open the file at @p path.
 * - Parse identifiers (textures/colors) until the first map line is reached.
 * - Read and build the padded rectangular map grid.
 * - Validate the resulting map (chars, player count, closed map).
 *
 * On any error:
 * - The file descriptor is closed (if opened).
 * - free_parser_game() is called to avoid leaks.
 * - A non-zero value is returned.
 *
 * On success:
 * - The file descriptor is closed.
 * - @p game contains parsed textures/colors/map/player info.
 *
 * @param path Path to the .cub scene file.
 * @param game Output game struct receiving all parsed data.
 *
 * @return 0 on success, non-zero on failure (prints an error message).
 */
int	parse_scene(const char *path, t_game *game)
{
	int		fd;
	char	*first_line;

	init_parser_game(game);
	get_line_reset();
	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (ft_error(CANT_OPEN_MAP));
	if (parse_identifiers_until_map(fd, game, &first_line))
		return (close(fd), free_parser_game(game), 1);
	if (read_map(fd, game, first_line))
		return (close(fd), free_parser_game(game), 1);
	if (validate_map(game))
		return (close(fd), free_parser_game(game), 1);
	close (fd);
	return (0);
}
