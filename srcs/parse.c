/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgueon <jgueon@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 18:19:14 by jgueon            #+#    #+#             */
/*   Updated: 2026/01/14 00:38:52 by jgueon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief Write a string to standard error (stderr).
 *
 * This helper prints the given null-terminated string character-by-character
 * to file descriptor 2 using write().
 *
 * @param s The message to print (must be a valid C-string).
 *
 * @return None.
 */
void	ft_putstr_err(const char *s)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
	{
		write(2, &s[i], 1);
		i++;
	}
}

/**
 * @brief Print a standardized error message and return an error code.
 *
 * This function prints:
 * - "Error\n"
 * - followed by the provided @p msg
 *
 * It then returns 1 so it can be used directly in error-return paths.
 *
 * @param msg The error message to print after "Error\n".
 *
 * @return Always returns 1.
 */
int	ft_error(const char *msg)
{
	ft_putstr_err("Error\n");
	ft_putstr_err(msg);
	return (1);
}

/**
 * @brief Check that a filename has the expected map extension.
 *
 * This function verifies that @p file ends with MAP_EXTENSION (e.g, ".cub").
 * It compares the last 4 characters of @p file with
 * MAP_EXTENSION[0..3].
 *
 * Note: this implementation assumes the extension length is exactly 4.
 *
 * @param file The filename/path to validate.
 *
 * @return 1 if the extension matches, 0 otherwise.
 */
static int	check_extension(const char *file)
{
	int	len;
	int	ext_len;

	len = ft_strlen(file);
	ext_len = ft_strlen(MAP_EXTENSION);
	if (len < ext_len)
		return (0);
	if (file[len - 4] != MAP_EXTENSION[0])
		return (0);
	if (file[len - 3] != MAP_EXTENSION[1])
		return (0);
	if (file[len - 2] != MAP_EXTENSION[2])
		return (0);
	if (file[len - 1] != MAP_EXTENSION[3])
		return (0);
	return (1);
}

/**
 * @brief Validate command-line arguments for the program.
 *
 * Rules enforced:
 * - Exactly one argument is expected: the map path.
 * - If no map argument is provided: prints NO_MAP.
 * - If more than one argument is provided: prints INVALID_ARG_MSG.
 * - If the map filename does not have the expected extension: prints
 * 	FILE_EXT_MSG.
 *
 * @param argc Argument count from main().
 * @param argv Argument vector from main().
 *
 * @return 0 if arguments are valid, non-zero on error (prints via ft_error).
 */
int	check_args(int argc, char **argv)
{
	if (argc < 2)
		return (ft_error(NO_MAP));
	if (argc > 2)
		return (ft_error(INVALID_ARG_MSG));
	if (!check_extension(argv[1]))
		return (ft_error(FILE_EXT_MSG));
	return (0);
}
