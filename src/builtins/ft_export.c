/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohhusse <mohhusse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 11:32:06 by mohhusse          #+#    #+#             */
/*   Updated: 2025/01/14 18:10:42 by mohhusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	print_export(t_env *xenv)
{
	t_env	*curr;

	curr = xenv;
	while (curr)
	{
		printf("declare -x %s", curr->key);
		if (curr->equal == true)
			printf("=\"%s\"", curr->value);
		printf("\n");
		curr = curr->next;
	}
	return ;
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

int		update_existing(t_shell *shell, t_env *env, bool equal, bool xenv)
{
	t_env	*curr;

	curr = shell->env;
	if (xenv)
		curr = shell->xenv;
	while (curr)
	{
		if (!ft_strcmp(curr->key, env->key))
		{
			if (equal == true)
				curr->value = env->value;
			return (1);
		}
		curr = curr->next;
	}
	return (0);
}

void	addexport(t_shell *shell, t_env *env)
{
	t_env	*curr;
	t_env	*next;

	if (!shell->xenv)
	{
		shell->xenv = env;
		return ;
	}
	curr = shell->xenv;
	next = curr->next;
	while (next)
	{
		if (ft_strcmp(env->key, next->key) > 0)
		{
			curr->next = env;
			env->next= next;
			return ;
		}
		curr = curr->next;
		next= next->next;
	}
	curr->next = env;
	env->next = next;
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

void	export_not_equal(char *key, t_shell *shell)
{
	t_env	*new_env;

	if (!valid_key(key))
	{
		printf("bash: export: '%s': not a valid identifier\n", key);
		return ;
	}
	new_env = (t_env *)malloc(sizeof(t_env));
	if (!new_env)
		return ;
	new_env->key = key;
	new_env->value = NULL;
	new_env->equal = false;
	new_env->next = NULL;
	if (update_existing(shell, new_env, false, true))
		return ;
	addexport(shell, new_env);
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
	new_env = (t_env *)malloc(sizeof(t_env));
	if (!new_env)
		return ;
	new_env->key = key;
	new_env->value = value;
	new_env->equal = true;
	new_env->next = NULL;
	if (!update_existing(shell, new_env, true, false))
		addtoenv(shell, new_env);
}

int		export_equal(char *assign, t_shell *shell)
{
	char	*key;
	char	*value;
	t_env	*new_env;

	key = ft_strtok(assign, "=");
	value = ft_strtok(NULL, "");
	if (!valid_key(key))
	{
		printf("bash: export: '%s': not a valid identifier\n", assign);
		return (0);
	}
	new_env = (t_env *)malloc(sizeof(t_env));
	if (!new_env)
		return (0);
	new_env->key = key;
	new_env->value = value;
	new_env->equal = true;
	new_env->next = NULL;
	if (update_existing(shell, new_env, true, true))
		return (1);
	addexport(shell, new_env);
	return (1);
}

void	export_arg(char *assign, t_shell *shell)
{
	char	*equal;
	char	*dup;
	int		xflag;

	if (!ft_isalpha(*assign) && assign[0] != 95)
	{
		printf("bash: export: '%s': not a valid identifier\n", assign);
		return ;
	}
	equal = ft_strchr(assign, '=');
	if (!equal)
		export_not_equal(assign, shell);
	else
	{
		dup = ft_strdup(assign);
		xflag = export_equal(assign, shell);
		if (xflag)
			export_env(dup, shell);
	}
}

void	ft_export(t_cmd *cmd, t_shell *shell)
{
	int		i;

	if (!cmd->args[1])
	{
		print_export(shell->xenv);
		return ;
	}

	i = 1;
	while (cmd->args[i])
	{
		export_arg(cmd->args[i], shell);
		i++;
	}
	shell->xenv = merge_sort(shell->xenv);
}
