/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtraoui <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 12:03:45 by fel-ghaz          #+#    #+#             */
/*   Updated: 2025/05/08 14:37:33 by rtraoui          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	set_old_pwd(t_env *env, char *old_pwd)
{
	envset(env, "OLDPWD", old_pwd);
	free(old_pwd);
	old_pwd = getcwd(NULL, 0);
	if (old_pwd)
	{
		envset(env, "PWD", old_pwd);
		free(old_pwd);
	}
}

char	*get_old_pwd(t_env *env, bool print)
{
	char	*path;

	path = envget(env, "OLDPWD");
	if (!path)
	{
		fprintf(stderr, "cd: OLDPWD not set\n");
		return (NULL);
	}
	if (print)
		printf("%s\n", path);
	return (path);
}

char	*get_home(t_env *env)
{
	t_env	*home;

	home = env;
	while (home && ft_strcmp(home->key, "HOME") != 0)
		home = home->next;
	if (home && home->value)
		return (home->value);
	else
	{
		fprintf(stderr, "cd: HOME not set\n");
		return (NULL);
	}
}

void	ft_cd(t_cmd *cmd, t_env *env)
{
	const char	*path;
	char		*old_pwd;

	if (cmd->args[0] && cmd->args[1] && cmd->args[2])
	{
		printf("bash: cd: too many arguments\n");
		return ;
	}
	path = cmd->args[1];
	if (!path)
		path = get_home(env);
	else if (!ft_strcmp((char *)path, "-"))
		path = get_old_pwd(env, true);
	else if (!ft_strcmp((char *)path, "--"))
		path = get_old_pwd(env, false);
	if (!path)
		return ;
	old_pwd = getcwd(NULL, 0);
	if (!old_pwd)
		return ;
	if (chdir(path) == -1)
		perror("cd");
	else
		set_old_pwd(env, old_pwd);
}
