/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isexpandable.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgazur <bgazur@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 15:07:35 by bgazur            #+#    #+#             */
/*   Updated: 2025/07/30 09:25:04 by bgazur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

bool	ft_isexpandable(int c, char *quote)
{
	if (*quote == '\0' && ft_isquote(c))
	{
		*quote = c;
		return (false);
	}
	if (c == *quote)
	{
		*quote = '\0';
		return (false);
	}
	if (c == '$' && *quote != '\'')
		return (true);
	return (false);
}
