/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   revert.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohhusse <mohhusse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 13:24:10 by mohhusse          #+#    #+#             */
/*   Updated: 2025/03/29 13:24:10 by mohhusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	revert_var(t_token	**tokens)
{
	t_token	*curr;

	if (!tokens || !*tokens)
		return ;
	curr = *tokens;
	while (curr && curr->next)
	{
		if (curr->type == HEREDOC && curr->next->type == ENV_VAR)
			curr->next->type = WORD;
		curr = curr->next;
	}
}
