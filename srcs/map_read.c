/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_read.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgueon <jgueon@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 20:24:31 by jgueon            #+#    #+#             */
/*   Updated: 2026/01/14 14:20:02 by jgueon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief Append one line pointer to a NULL-terminated array of lines.
 *
 * This function grows the dynamic array @p *arr by 1 element, stores @p line
 * at the end, and keeps the array NULL-terminated.
 *
 * Memory behavior:
 * - Allocates a new array of size (*n + 2).
 * - Copies the existing pointers (does NOT duplicate the strings).
 * - Frees the old array pointer (*arr), then updates *arr to the new array.
 * - Increments *n.
 *
 * @param arr Address of the current NULL-terminated array of strings (may be
 * 	NULL).
 * @param n Address of the current number of stored lines (updated on success).
 * @param line The line pointer to add (ownership is kept by the array after
 * 	push).
 *
 * @return 0 on success, 1 on allocation failure.
 */
static int	push_line(char ***arr, int *n, char *line)
{
	char	**new_arr;
	int		i;

	new_arr = (char **)malloc(sizeof(char *) * (size_t)(*n + 2));
	if (!new_arr)
		return (1);
	i = 0;
	while (i < *n)
	{
		new_arr[i] = (*arr)[i];
		i++;
	}
	new_arr[*n] = line;
	new_arr[*n + 1] = NULL;
	free(*arr);
	*arr = new_arr;
	*n += 1;
	return (0);
}

/**
 * @brief Build a padded rectangular map grid (game->map) from raw map lines.
 *
 * This function allocates game->map with height game->map_h, then for each row:
 * - Allocates a string of length game->map_w.
 * - Fills it with spaces ' ' (padding).
 * - Copies the raw line content into the beginning of the row.
 *
 * This produces a rectangular map where shorter input lines are right-padded
 * with spaces, which is useful for later validation (flood fill, borders, etc).
 *
 * On allocation failure mid-way, it frees the partially built grid and sets
 * game->map to NULL.
 *
 * @param game Game context where map_h/map_w are already computed and map is
 * 	stored.
 * @param lines Raw map lines (NULL-terminated), typically gathered by
 * 	read_map().
 *
 * @return 0 on success, 1 on allocation failure.
 */
static int	build_grid(t_game *game, char **lines)
{
	int		y;
	char	*row;

	game->map = (char **)malloc(sizeof(char *) * (size_t)(game->map_h + 1));
	if (!game->map)
		return (1);
	game->map[game->map_h] = NULL;
	y = 0;
	while (y < game->map_h)
	{
		row = (char *)malloc((size_t)game->map_w + 1);
		if (!row)
		{
			free_partial_grid(game->map, y);
			game->map = NULL;
			return (1);
		}
		ft_memset(row, ' ', (size_t)game->map_w);
		row[game->map_w] = '\0';
		ft_memcpy(row, lines[y], (size_t)row_len(lines[y]));
		game->map[y] = row;
		y++;
	}
	return (0);
}

/**
 * @brief Validate one map line while reading the map section ("map mode").
 *
 * Rules enforced:
 * - A line containing only spaces/tabs is forbidden (empty line inside map).
 * - Every character must belong to the allowed map charset(via is_map_charset),
 *   except '\r' which is ignored (Windows CRLF handling).
 *
 * @param line A raw line from the map section (as returned by get_line()).
 *
 * @return 0 if valid, non-zero if invalid (prints an error via ft_error).
 */
static int	check_map_mode_line(char *line)
{
	int	i;

	if (*skip_spaces(line) == '\0')
		return (ft_error(EMPTY_LINE_IN_MAP));
	i = 0;
	while (line[i])
	{
		if (line[i] == '\r')
		{
			i++;
			continue ;
		}
		if (!is_map_charset(line[i]))
			return (ft_error(INVALID_MAP_CHAR_MSG));
		i++;
	}
	return (0);
}

/**
 * @brief Read all remaining map lines from fd and collect them into @p *lines.
 *
 * This helper is used by read_map() to keep the function small for Norm.
 * For each read line:
 * - Validate it with check_map_mode_line().
 * - Update game->map_w if the line is wider than the current max width.
 * - Push it into the dynamic @p *lines array, updating game->map_h.
 *
 * On error, it frees the current line and the already collected lines.
 *
 * @param fd The file descriptor positioned at the first map line after
 * 	first_line.
 * @param game Game context updated with final map_h and map_w.
 * @param lines Address of the collected raw lines array (NULL-terminated).
 *
 * @return 0 on success, 1 on error (validation or allocation).
 */
static int	read_map_lines(int fd, t_game *game, char ***lines)
{
	char	*line;
	int		i;

	line = get_line(fd);
	while (line)
	{
		if (check_map_mode_line(line))
			return (free(line), free_map(*lines), 1);
		i = row_len(line);
		if (i > game->map_w)
			game->map_w = i;
		if (push_line(lines, &game->map_h, line))
			return (free(line), free_map(*lines), ft_error("Malloc failed\n"));
		line = get_line(fd);
	}
	return (0);
}

/**
 * @brief Read and build the final padded map grid from the .cub file.
 *
 * This function assumes the parser already detected the start of the map and
 * provides the first map line in @p first_line. It then:
 * - Initializes game->map_h, game->map_w, and game->map.
 * - Validates the first line under map rules.
 * - Collects all map lines (including first_line) into a temporary array.
 * - Builds a padded rectangular grid into game->map.
 * - Frees the temporary raw lines array.
 *
 * @param fd The file descriptor for the .cub file (positioned after
 * 	first_line).
 * @param game Game context receiving map_h/map_w and the allocated game->map.
 * @param first_line The first map line already read (ownership is consumed
 * 	here).
 *
 * @return 0 on success, non-zero on error (prints error via ft_error).
 */
int	read_map(int fd, t_game *game, char *first_line)
{
	char	**lines;

	lines = NULL;
	game->map = NULL;
	game->map_h = 0;
	game->map_w = row_len(first_line);
	if (check_map_mode_line(first_line))
		return (free(first_line), 1);
	if (push_line(&lines, &game->map_h, first_line))
		return (free(first_line), ft_error("Malloc failed\n"));
	if (read_map_lines(fd, game, &lines))
		return (1);
	if (build_grid(game, lines))
		return (free_map(lines), ft_error("Malloc failed\n"));
	free_map(lines);
	return (0);
}
