/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lst_tok_remove.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgazur <bgazur@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:01:18 by bgazur            #+#    #+#             */
/*   Updated: 2025/08/06 11:21:34 by bgazur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lst_tok_remove(t_token **lst, t_token *node)
{
	t_token	*current;
	t_token	*previous;

	if (!lst || !*lst || !node)
		return ;
	if (*lst == node)
	{
		*lst = node->next;
		free(node->content);
		free(node);
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
		free(current->content);
		free(current);
	}
}
