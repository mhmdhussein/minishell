/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohhusse <mohhusse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 11:32:06 by mohhusse          #+#    #+#             */
/*   Updated: 2025/01/30 14:30:32 by mohhusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_env	*copy_env(t_env *env)
{
	t_env	*new_list;
	t_env	*new_node;
	t_env	*curr;

	new_list = NULL;
	curr = env;
	while (curr)
	{
		new_node = (t_env *)malloc(sizeof(t_env));
		if (!new_node)
			return (NULL);
		new_node->key = ft_strdup(curr->key);
		if (curr->value)
		{
			new_node->value = ft_strdup(curr->value);
			new_node->equal = true;
		}
		else
		{
			new_node->value = NULL;
			new_node->equal = false;
		}
		new_node->next = new_list;
		new_list = new_node;
		curr = curr->next;
	}
	return (new_list);
}

void	print_export(t_env *env)
{
	t_env	*xenv;
	t_env	*curr;

	xenv = copy_env(env);
	xenv = merge_sort(xenv);
	curr = xenv;
	while (curr)
	{
		printf("declare -x %s", curr->key);
		if (curr->equal == true)
			printf("=\"%s\"", curr->value);
		printf("\n");
		curr = curr->next;
	}
}

int	valid_key(char *assign)
{
	int	i;

	i = 1;
	while (assign[i])
	{
		if (!ft_isalpha(assign[i]) && !ft_isdigit(assign[i]) && assign[i] != 95)
			return (0);
		i++;
	}
	return (1);
}

int	update_existing(t_shell *shell, t_env *env)
{
	t_env	*curr;

	curr = shell->env;
	while (curr)
	{
		if (!ft_strcmp(curr->key, env->key))
		{
			if (env->value)
			{
				curr->value = env->value;
				if (curr->equal == false)
					curr->equal = true;
			}
			return (1);
		}
		curr = curr->next;
	}
	return (0);
}

void	addtoenv(t_shell *shell, t_env *env)
{
	t_env	*curr;

	if (!shell->env)
		shell->env = env;
	curr = shell->env;
	while (curr->next)
		curr = curr->next;
	curr->next = env;
}

void	export_env(char *assign, t_shell *shell)
{
	char	*key;
	char	*value;
	t_env	*new_env;

	key = ft_strtok(assign, "=");
	value = ft_strtok(NULL, "");
	if (!valid_key(key))
	{
		printf("bash: export: '%s': not a valid identifier\n", assign);
		return ;
	}
	new_env = create_env(key, value);
	if (!new_env)
		return ;
	if (value)
		new_env->equal= true;
	else
		new_env->equal = false;
	if (!update_existing(shell, new_env))
		addtoenv(shell, new_env);
}

void	export_arg(char *assign, t_shell *shell)
{
	if (!ft_isalpha(*assign) && assign[0] != 95)
	{
		printf("bash: export: '%s': not a valid identifier\n", assign);
		return ;
	}
	export_env(assign, shell);
}

void	ft_export(t_cmd *cmd, t_shell *shell)
{
	int		i;

	if (!cmd->args[1])
	{
		print_export(shell->env);
		return ;
	}
	i = 1;
	while (cmd->args[i])
	{
		export_arg(cmd->args[i], shell);
		i++;
	}
}
