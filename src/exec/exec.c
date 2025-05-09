/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohhusse <mohhusse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 18:16:49 by mohhusse          #+#    #+#             */
/*   Updated: 2025/03/17 13:58:04 by mohhusse         ###   ########.fr       */
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
	dirs = ft_split(path, ':');
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

static void	execute_child_process(t_cmd *cmd, t_shell *shell, char *full_path)
{
	char	**env_array;

	env_array = env_to_array(shell->env);
	execve(full_path, cmd->args, env_array);
	perror("execve");
	free_double_array(env_array);
	exit(0);
}

static void	wait_for_child(pid_t pid, t_shell *shell)
{
	int		status;

	waitpid(pid, &status, 0);
	shell->last_exit_status = WEXITSTATUS(status);
}

char	*check_absolute_relative_path(char *cmd, int *path_flag)
{
	struct stat	st;

	if (!cmd)
		return (NULL);
	if (cmd[0] == '/' || cmd[0] == '.')
	{
		if (stat(cmd, &st) == 0 && S_ISDIR(st.st_mode))
		{
			*path_flag = 1;
			printf("-bash: %s: Is a directory\n", cmd);
		}
		else if (access(cmd, F_OK | X_OK) == 0)
			return (ft_strdup(cmd));
		else if (access(cmd, F_OK) == 0)
		{
			*path_flag = 1;
			printf("-bash: %s: Permission denied\n", cmd);
		}
		else
		{
			*path_flag = 1;
			printf("-bash: %s: No such file or directory\n", cmd);
		}
	}
	return (NULL);
}

void	execute_command(t_cmd *cmd, t_shell *shell)
{
	pid_t	pid;
	char	*full_path;
	int		path_flag;

	path_flag = 0;
	full_path = check_absolute_relative_path(cmd->args[0], &path_flag);
	if (path_flag == 1)
		return ;
	if (!full_path)
		full_path = find_executable_path(cmd->args[0], shell);
	if (!full_path || cmd->args[0][0] == '\0')
	{
		dup2(shell->std_out, STDOUT_FILENO);
		printf("bash: %s: command not found\n", cmd->args[0]);
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
