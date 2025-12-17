/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lst_env_remove.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgazur <bgazur@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:01:18 by bgazur            #+#    #+#             */
/*   Updated: 2025/08/23 13:27:11 by bgazur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	free_env_node(t_env *node);

void	ft_lst_env_remove(t_env **lst, t_env *node)
{
	t_env	*current;
	t_env	*previous;

	if (!lst || !*lst || !node)
		return ;
	if (*lst == node)
	{
		*lst = node->next;
		free_env_node(node);
		return ;
	}
	previous = NULL;
	current = *lst;
	while (current && current != node)
	{
		previous = current;
		current = current->next;
	}
	if (current == node)
	{
		previous->next = current->next;
		free_env_node(current);
	}
}

// Perfroms free() on all the content of the env node.
static void	free_env_node(t_env *node)
{
	free(node->key);
	free(node->value);
	free(node);
}
