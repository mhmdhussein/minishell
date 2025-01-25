/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohhusse <mohhusse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 16:30:32 by mohhusse          #+#    #+#             */
/*   Updated: 2025/01/25 15:28:18 by mohhusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_env	*create_env(char *key, char *value)
{
	t_env	*new_node;

	new_node = (t_env *)malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->key = ft_strdup(key);
	if (!new_node->key)
	{
		free(new_node);
		return (NULL);
	}
	new_node->value = ft_strdup(value);
	if (!new_node->value)
	{
		free(new_node->key);
		free(new_node);
		return (NULL);
	}
	new_node->next = NULL;
	return (new_node);
}

int	checkenv(t_env *env, char *key)
{
	t_env	*curr;

	curr = env;
	while (curr)
	{
		if (!ft_strcmp(curr->key, key))
			return (1);
		curr = curr->next;
	}
	return (0);
}

void	addenv(t_env *env, char *key, char *value)
{
	t_env	*curr;

	if (!env)
	{
		env = create_env(key, value);
		return ;
	}
	curr = env;
	while (curr->next)
		curr = curr->next;
	curr->next = create_env(key, value);
}

char	*envget(t_env *env, char *key)
{
	t_env	*curr;

	curr = env;
	while (curr)
	{
		if (!ft_strcmp(curr->key, key) && curr->equal == true)
			return (curr->value);
		curr = curr->next;
	}
	return (NULL);
}

void	envset(t_env *env, char *key, char *value)
{
	t_env	*curr;

	curr = env;
	while (curr)
	{
		if (!ft_strcmp(curr->key, key))
		{
			curr->value = ft_strdup(value);
			return ;
		}
		curr = curr->next;
	}
	addenv(env, key, value);
}
