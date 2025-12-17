/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lst_env_sort.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgazur <bgazur@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 15:11:27 by bgazur            #+#    #+#             */
/*   Updated: 2025/08/22 15:13:17 by bgazur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lst_env_sort(t_env **arr, size_t size)
{
	size_t	i;
	size_t	j;
	t_env	*temp;

	i = 0;
	j = size - 1;
	while (j > 0)
	{
		while (i < (size - 1))
		{
			if (ft_strcmp(arr[i + 1]->key, arr[i]->key) < 0)
			{
				temp = arr[i];
				arr[i] = arr[i + 1];
				arr[i + 1] = temp;
			}
			i++;
		}
		i = 0;
		j--;
	}
}
