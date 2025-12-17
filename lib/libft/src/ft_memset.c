/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgazur <bgazur@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 18:30:59 by edlucca           #+#    #+#             */
/*   Updated: 2025/07/27 12:22:14 by bgazur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*temp_ptr;

	temp_ptr = (unsigned char *) s;
	while (n > 0)
	{
		(*temp_ptr++) = (unsigned char) c;
		n--;
	}
	return (s);
}
