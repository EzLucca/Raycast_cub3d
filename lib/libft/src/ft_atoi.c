/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgazur <bgazur@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 15:12:15 by bgazur            #+#    #+#             */
/*   Updated: 2025/08/29 09:06:05 by bgazur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *nptr, t_data *data)
{
	int			digit;
	int			sign;
	long long	result;
	size_t		i;

	sign = 1;
	result = 0;
	i = 0;
	if (nptr[i] == '+' || nptr[i] == '-')
	{
		if (nptr[i] == '-')
			sign = -1;
		i++;
	}
	while (nptr[i] != '\0' && ft_isdigit(nptr[i]))
	{
		digit = nptr[i] - '0';
		if (sign == 1 && (result > (LLONG_MAX - digit) / 10))
			exit_atoi_overflow(data);
		if (sign == -1 && (-result < (LLONG_MIN + digit) / 10))
			exit_atoi_overflow(data);
		result = result * 10 + digit;
		i++;
	}
	return ((int)((result * sign) % 256));
}
