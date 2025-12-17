/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdelimiter.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgazur <bgazur@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 08:39:40 by bgazur            #+#    #+#             */
/*   Updated: 2025/08/07 10:09:40 by bgazur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

bool	ft_isdelimiter(char *s, size_t i, size_t *len)
{
	size_t	j;

	j = 0;
	while (DELIMITERS[j])
	{
		if (s[i] == DELIMITERS[j])
		{
			if (*len == 0)
			{
				*len = 1;
				if ((s[i] == '<' && s[i + 1] == '<')
					|| (s[i] == '>' && s[i + 1] == '>'))
					*len += 1;
			}
			return (true);
		}
		j++;
	}
	return (false);
}
