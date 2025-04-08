/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohhusse <mohhusse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 15:01:36 by mohhusse          #+#    #+#             */
/*   Updated: 2025/04/05 15:01:36 by mohhusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_pipes(t_shell *shell)
{
	t_cmd	*cmd;
	int		pipe_fd[2];
	pid_t	pid;
	int		prev_fd;

	cmd = shell->cmds;
	prev_fd = -1;
	while (cmd)
	{
		if (cmd->next && pipe(pipe_fd) == -1)
			return ;
		pid = fork();
		if (pid == -1)
			return ;
		else if (pid == 0)
		{
			if (prev_fd != -1)
			{
				dup2(prev_fd, STDIN_FILENO);
				close(prev_fd);
			}
			if (cmd->next)
			{
				dup2(pipe_fd[1], STDOUT_FILENO);
				close(pipe_fd[1]);
			}
			close(pipe_fd[0]);
			if (is_builtin(cmd->args[0]))
				exec_builtin(cmd, shell);
			else
				execute_command(cmd, shell);
			exit(1);
		}
		else
		{
			if (prev_fd != -1)
				close(prev_fd);
			if (cmd->next)
				close(pipe_fd[1]);
			prev_fd = pipe_fd[0];
			cmd = cmd->next;
		}
	}
	while (wait(&shell->last_exit_status) > 0);
}

char	**fill_args(t_token	**tokens)
{
	int		num_tokens;
	t_token	*start;
	char	**args;
	int		i;

	num_tokens = 0;
	start = *tokens;
	while (*tokens && (*tokens)->type != PIPE)
	{
		num_tokens++;
		(*tokens) = (*tokens)->next;
	}
	args = (char **)malloc(sizeof(char *) * (num_tokens + 1));
	i = 0;
	while (start && start != *tokens)
	{
		args[i++] = ft_strdup(start->value);
		start = start->next;
	}
	args[i] = NULL;
	return (args);
}

t_cmd	*create_cmd()
{
	t_cmd	*cmd;

	cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->append = false;
	cmd->delim = NULL;
	cmd->input_fd = -1;
	cmd->output_fd = -1;
	cmd->next = NULL;
	return (cmd);
}

void	parse_commands(t_token	*tokens, t_shell *shell)
{
	t_cmd	*cmd;
	t_cmd	*curr;

	while (tokens)
	{
		cmd = create_cmd();
		if (!cmd)
			return ;
		cmd->args = fill_args(&tokens);
		if (!shell->cmds)
			shell->cmds = cmd;
		else
			curr->next = cmd;
		curr = cmd;
		if (tokens && tokens->type == PIPE)
			tokens = tokens->next;
	}
}

int	check_pipes(t_token *tokens)
{
	t_token	*curr;
	int		has_pipe;

	if (tokens->type == PIPE)
		return (-1);
	curr = tokens;
	has_pipe = 0;
	while (curr)
	{
		if (curr->type == PIPE && curr->next == NULL)
			return (-1);
		else if (curr->type == PIPE && curr->next->type == PIPE)
			return (-1);
		if (curr->type == PIPE && has_pipe != 1)
			has_pipe = 1;
		curr = curr->next;
	}
	return (has_pipe);
}
