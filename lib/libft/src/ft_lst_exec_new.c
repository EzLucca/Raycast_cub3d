/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lst_exec_new.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgazur <bgazur@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 10:28:38 by edlucca           #+#    #+#             */
/*   Updated: 2025/08/31 11:48:32 by bgazur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_exec	*ft_lst_exec_new(char **cmd_arg, char **in, char **out, char **app)
{
	t_exec	*new_node;

	new_node = ft_calloc(1, sizeof(t_exec));
	if (!new_node)
		return (NULL);
	new_node->cmd_arg = cmd_arg;
	new_node->in = in;
	new_node->out = out;
	new_node->app = app;
	new_node->next = NULL;
	return (new_node);
}
