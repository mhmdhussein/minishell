/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohhusse <mohhusse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 18:06:09 by mohhusse          #+#    #+#             */
/*   Updated: 2025/01/30 13:30:40 by mohhusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	remove_env(t_shell *shell, char *key)
{
	t_env	*curr;
	t_env	*prev;

	curr = shell->env;
	prev = NULL;
	while (curr)
	{
		if (!ft_strcmp(curr->key, key))
		{
			if (prev)
				prev->next = curr->next;
			else
				shell->env = curr->next;
			free(curr->key);
			free(curr);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}

void	ft_unset(t_cmd *cmd, t_shell *shell)
{
	int		i;

	i = 1;
	if (!cmd->args[1])
		return ;
	while (cmd->args[i])
	{
		remove_env(shell, cmd->args[i]);
		i++;
	}
}
