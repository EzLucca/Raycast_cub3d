/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgueon <jgueon@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 15:12:15 by bgazur            #+#    #+#             */
/*   Updated: 2026/01/12 19:41:56 by jgueon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// int	ft_atoi(const char *nptr, t_data *data)
// {
// 	int			digit;
// 	int			sign;
// 	long long	result;
// 	size_t		i;

// 	sign = 1;
// 	result = 0;
// 	i = 0;
// 	if (nptr[i] == '+' || nptr[i] == '-')
// 	{
// 		if (nptr[i] == '-')
// 			sign = -1;
// 		i++;
// 	}
// 	while (nptr[i] != '\0' && ft_isdigit(nptr[i]))
// 	{
// 		digit = nptr[i] - '0';
// 		if (sign == 1 && (result > (LLONG_MAX - digit) / 10))
// 			exit_atoi_overflow(data);
// 		if (sign == -1 && (-result < (LLONG_MIN + digit) / 10))
// 			exit_atoi_overflow(data);
// 		result = result * 10 + digit;
// 		i++;
// 	}
// 	return ((int)((result * sign) % 256));
// }

/**
 * This function converts the initial portion of the string pointed to by str
 * to an integer representation. It skips all white-space characters at the
 * beginning, takes an optional plus or minus sign followed by as many digits
 * as possible, and interprets them as a numerical value.
 * ft_atoi - Converts a string to an integer
 *
 * @param str: The string to be converted
 * @return The converted integer value
 */
int	ft_atoi(const char *str)
{
	int		sign;
	int		result;

	sign = 1;
	result = 0;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + (*str - '0');
		str++;
	}
	return (result * sign);
}
