/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtraoui <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 12:03:45 by fel-ghaz          #+#    #+#             */
/*   Updated: 2025/05/08 18:21:26 by rtraoui          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	set_old_pwd(t_env *env, char *old_pwd, t_shell *shell)
{
	envset(env, "OLDPWD", old_pwd);
	free(old_pwd);
	old_pwd = getcwd(NULL, 0);
	if (old_pwd)
	{
		envset(env, "PWD", old_pwd);
		free(shell->current_pwd);
		shell->current_pwd = ft_strdup(old_pwd);
		free(old_pwd);
	}
}

int	check_path(const char *path)
{
	struct stat	st;

	if (!path)
		return (0);
	else if (access(path, F_OK) != 0)
	{
		printf("-bash: cd: %s: No such file or directory\n", path);
		return (0);
	}
	else if (stat(path, &st) == 0 && !S_ISDIR(st.st_mode))
	{
		printf("-bash: cd: %s: Not a directory\n", path);
		return (0);
	}
	return (1);
}

char	*get_old_pwd(t_env *env, bool print)
{
	char		*path;

	path = envget(env, "OLDPWD");
	if (!path)
	{
		printf("cd: OLDPWD not set\n");
		return (NULL);
	}
	else if (!check_path(path))
		return (NULL);
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

char	*expand_home(t_env *env, char *path, t_shell *shell)
{
	char	*home;
	char	*expanded;

	home = envget(env, "HOME");
	if (!home)
		home = shell->original_home;
	expanded = ft_strjoin(home, (path + 1));
	return (expanded);
}

char	*slice_end(char *str)
{
	char	*end;
	char	*result;
	int		len;

	end = ft_strrchr(str, '/');
	if (!end)
		return (ft_strdup(str));
	len = ft_strlen(str) - ft_strlen(end);
	result = (char *)malloc(len + 1);
	if (!result)
		return (NULL);
	ft_memcpy(result, str, len);
	result[len] = '\0';
	return (result);
}

void	ft_cd(t_cmd *cmd, t_env *env, t_shell *shell)
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
	else if (!ft_strncmp((char *)path, "~", 1))
		path = expand_home(env, (char *)path, shell);
	else if (!ft_strcmp((char *)path, "-"))
		path = get_old_pwd(env, true);
	else if (!ft_strcmp((char *)path, "--"))
		path = get_old_pwd(env, false);
	if (!path || (cmd->args[1] && cmd->args[1][0] == '\0') || (path && path[0] == '\0')
		|| (path && !check_path(path)))
		return ;
	old_pwd = getcwd(NULL, 0);
	if (!old_pwd)
	{
		old_pwd = envget(env, "PWD");
		if (!ft_strcmp((char *)path, ".."))
		{
			envset(shell->env, "OLDPWD", old_pwd);
			old_pwd = slice_end(old_pwd);
			envset(shell->env, "PWD", old_pwd);
			free(shell->current_pwd);
			shell->current_pwd = ft_strdup(old_pwd);
			if (access(old_pwd, F_OK) == 0)
				{chdir(old_pwd); free(old_pwd);}
			else
				printf("chdir: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory\n");
			return ;
		}
	}
	if (chdir(path) == -1)
		printf("chdir: error while changing directory\n");
	else
		set_old_pwd(env, old_pwd, shell);
}
