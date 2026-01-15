/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgueon <jgueon@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 17:47:47 by jgueon            #+#    #+#             */
/*   Updated: 2026/01/14 00:42:08 by jgueon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief Check whether a character is considered whitespace for texture IDs.
 *
 * This helper treats common whitespace characters as separators after the
 * 2-letter texture identifier (e.g., "NO ", "SO\t").
 *
 * @param c Character to test.
 *
 * @return 1 if @p c is a whitespace character, 0 otherwise.
 */
static int	is_ws(char c)
{
	return (c == ' ' || c == '\t' || c == '\r' || c == '\v' || c == '\f');
}

/**
 * @brief Check if a trimmed line starts with a specific 2-letter texture id.
 *
 * This function validates that:
 * - The first two characters match the given @p id (eg."NO", "SO", "WE", "EA").
 * - The third character is a whitespace separator.
 *
 * The strict whitespace rule prevents false matches like "NOO..." being
 * accepted as "NO".
 *
 * @param s Trimmed input line.
 * @param id Two-character identifier string (e.g., "NO").
 *
 * @return 1 if the line matches the identifier format, 0 otherwise.
 */
static int	is_id(char *s, char *id)
{
	if (!s || !id)
		return (0);
	if (s[0] != id[0] || s[1] != id[1])
		return (0);
	if (!is_ws(s[2]))
		return (0);
	return (1);
}

/**
 * @brief Check whether a texture path ends with the ".png" extension.
 *
 * This function performs a simple suffix check on the string.
 *
 * @param path Texture file path.
 *
 * @return 1 if @p path ends with ".png", 0 otherwise.
 */
static int	has_png_ext(char *path)
{
	int	len;

	if (!path)
		return (0);
	len = (int)ft_strlen(path);
	if (len < 4)
		return (0);
	if (path[len - 4] != '.' || path[len -3] != 'p')
		return (0);
	if (path[len - 2] != 'n' || path[len - 1] != 'g')
		return (0);
	return (1);
}

/**
 * @brief Validate and store one texture path into a destination pointer.
 *
 * Validation rules:
 * - Duplicate protection: if *dst is already set, it returns INVALID_DUP_TEX.
 * - The path must not be NULL or empty.
 * - The file extension must be ".png".
 * - The file must be openable (open(path, O_RDONLY) succeeds).
 *
 * Storage rule:
 * - The stored path is duplicated with ft_strdup so it remains valid even after
 *   the original line buffer is freed by the parser.
 *
 * @param dst Address of the destination pointer (e.g., &game->tex.no).
 * @param path Path string to validate and store.
 *
 * @return 0 on success, non-zero on failure (prints via ft_error).
 */
static int	store_texture(char **dst, char *path)
{
	int	fd;

	if (*dst != NULL)
		return (ft_error(INVALID_DUP_TEX));
	if (!path || *path == '\0')
		return (ft_error(PATH_MSG));
	if (!has_png_ext(path))
		return (ft_error(INVALID_TEX_EXT_MSG));
	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (ft_error(PATH_MSG));
	close(fd);
	*dst = ft_strdup(path);
	if (*dst == NULL)
		return (ft_error("Malloc failed\n"));
	return (0);
}

/**
 * @brief Handle one trimmed line if it defines a wall texture identifier.
 *
 * If @p trim starts with one of the valid texture identifiers:
 * - "NO", "SO", "WE", "EA"
 * this function parses the path part (skipping spaces after the id),
 * then validates/stores it into the matching field in game->tex.
 *
 * Return convention used:
 * - 1: the line was a texture line and was successfully handled.
 * - 0: the line is not a texture identifier line (no error).
 * - -1: the line was a texture line but invalid (error already printed).
 *
 * @param game Game context where texture paths are stored.
 * @param trim Trimmed input line.
 *
 * @return 1 if handled successfully, 0 if not a texture line, -1 on error.
 */
int	handle_texture_line(t_game *game, char *trim)
{
	char	*path;
	int		ret;

	if (is_id(trim, N))
		path = skip_spaces(trim + 2);
	else if (is_id(trim, S))
		path = skip_spaces(trim + 2);
	else if (is_id(trim, W))
		path = skip_spaces(trim + 2);
	else if (is_id(trim, E))
		path = skip_spaces(trim + 2);
	else
		return (0);
	if (is_id(trim, N))
		ret = store_texture(&game->tex.no, path);
	else if (is_id(trim, S))
		ret = store_texture(&game->tex.so, path);
	else if (is_id(trim, W))
		ret = store_texture(&game->tex.we, path);
	else
		ret = store_texture(&game->tex.ea, path);
	if (ret != 0)
		return (-1);
	return (1);
}
