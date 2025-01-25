/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohhusse <mohhusse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 18:16:49 by mohhusse          #+#    #+#             */
/*   Updated: 2025/01/14 16:41:58 by mohhusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*find_executable_path(char *cmd, t_shell *shell)
{
	char	**dirs;
	char	*path;
	char	*full_path;
	int		i;

	path = envget(shell->env, "PATH");
	if (!path)
		return (NULL);
	dirs = libft_split(path, ':');
	if (!dirs)
		return (NULL);
	i = 0;
	full_path = NULL;
	while (dirs[i])
	{
		full_path = ft_strjoin(ft_strjoin(dirs[i], "/"), cmd);
		if (access(full_path, X_OK) == 0)
			break ;
		free(full_path);
		full_path = NULL;
		i++;
	}
	free_double_array(dirs);
	return (full_path);
}

static int	count_env(t_env *env)
{
	int		count;
	t_env	*curr;

	count = 0;
	curr = env;
	while (curr)
	{
		if (curr->equal)
			count++;
		curr = curr->next;
	}
	return (count);
}

static char	*create_env_entry(t_env *env)
{
	char	*entry;
	char	*temp;

	if (!env->key || !env->value)
		return (NULL);
	temp = ft_strjoin(env->key, "=");
	if (!temp)
		return (NULL);
	entry = ft_strjoin(temp, env->value);
	free(temp);
	return (entry);
}

static char	**env_to_array(t_env *env)
{
	char	**env_array;
	int		env_count;
	int		i;

	env_count = count_env(env);
	env_array = (char **)malloc((env_count + 1) * sizeof(char *));
	if (!env_array)
		return (NULL);
	i = 0;
	while (env)
	{
		if (env->equal)
		{
			env_array[i] = create_env_entry(env);
			if (!env_array[i])
			{
				free_double_array(env_array);
				return (NULL);
			}
			i++;
		}
		env = env->next;
	}
	env_array[i] = NULL;
	return (env_array);
}

static void	execute_child_process(t_cmd *cmd, t_shell *shell, char *full_path)
{
	char	**env_array;

	env_array = env_to_array(shell->env);
	execve(full_path, cmd->args, env_array);
	perror("execve");
	free_double_array(env_array);
	exit(1);
}

static void	wait_for_child(pid_t pid, t_shell *shell)
{
	int		status;

	waitpid(pid, &status, 0);
	shell->last_exit_status = WEXITSTATUS(status);
}

void	execute_command(t_cmd *cmd, t_shell *shell)
{
	pid_t	pid;
	char	*full_path;

	full_path = find_executable_path(cmd->args[0], shell);
	if (!full_path)
	{
		printf("bash: command not found: %s\n", cmd->args[0]);
		shell->last_exit_status = 127;
		return ;
	}
	pid = fork();
	if (pid == 0)
		execute_child_process(cmd, shell, full_path);
	else if (pid > 0)
		wait_for_child(pid, shell);
	else
		perror("fork");
	free(full_path);
}
