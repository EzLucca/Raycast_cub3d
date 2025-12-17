/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lst_exec_clear.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgazur <bgazur@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 10:20:15 by edlucca           #+#    #+#             */
/*   Updated: 2025/09/01 09:24:56 by bgazur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lst_exec_clear(t_exec **lst)
{
	t_exec	*tmp;

	if (!lst)
		return ;
	while (*lst)
	{
		tmp = *lst;
		*lst = (*lst)->next;
		ft_free_array(tmp->cmd_arg);
		tmp->cmd_arg = NULL;
		ft_free_array(tmp->in);
		tmp->in = NULL;
		ft_free_array(tmp->out);
		tmp->out = NULL;
		free(tmp->app);
		tmp->app = NULL;
		free(tmp);
		tmp = NULL;
	}
	*lst = NULL;
}
