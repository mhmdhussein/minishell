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

t_token	*extract_tokens(t_token *tokens, int i)
{
	t_token	*curr;
	t_token	*new_token;
	t_token	*head;

	curr = tokens;
	head = NULL;
	while (curr)
	{
		while (i)
		{
			if (curr->type == PIPE)
				i--;
			curr = curr->next;
		}
		while (curr && curr->type != PIPE)
		{
			new_token = create_token(curr->value);
			if (!new_token)
				return (NULL);
			add_token(&head, &new_token);
			curr = curr->next;
		}
		if (curr && curr->type == PIPE)
			break ;
		if (curr)
			curr = curr->next;
	}
	return (head);
}

void	decrement_shlvl(t_shell *shell)
{
	char	*shlvl;
	int		lvl;

	shlvl = envget(shell->env, "SHLVL");
	if (!shlvl)
		lvl = 0;
	else
	{
		lvl = ft_atoi(shlvl) - 1;
		if (lvl < 0)
			lvl = 0;
	}
	envset(shell->env, "SHLVL", ft_itoa(lvl));
}

int	has_heredoc(t_shell *shell, int i)
{
	t_token	*curr;

	if (!shell->tokens)
		return (0);
	curr = shell->tokens;
	while (i)
	{
		if (curr->type == PIPE)
			i--;
		curr = curr->next;
	}
	while (curr && curr->type != PIPE)
	{
		if (curr->type == HEREDOC)
			return (1);
		curr = curr->next;
	}
	return (0);
}

void	handle_pipes(t_shell *shell)
{
	t_cmd	*cmd;
	int		pipe_fd[2];
	pid_t	pid;
	int		prev_fd;
	int		i;
	t_token	*cmd_tokens;

	cmd = shell->cmds;
	prev_fd = -1;
	i = 0;
	while (cmd)
	{
		if (cmd->next && pipe(pipe_fd) == -1)
			return ;
		pid = fork();
		if (pid == -1)
			return ;
		else if (pid == 0)
		{
			decrement_shlvl(shell);
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
			cmd_tokens = extract_tokens(shell->tokens, i);
			if (!redirections(shell, cmd, &cmd_tokens))
				exit(1);
			cmd->args = detokenize(cmd_tokens);
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
			if (has_heredoc(shell, i))
				wait(&shell->last_exit_status);
			cmd = cmd->next;
		}
		i++;
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

// void	parse_commands(t_token	*tokens, t_shell *shell)
// {
// 	t_cmd	*cmd;
// 	t_cmd	*curr;

// 	while (tokens)
// 	{
// 		cmd = create_cmd();
// 		if (!cmd)
// 			return ;
// 		// cmd->args = fill_args(&tokens);
// 		if (!shell->cmds)
// 			shell->cmds = cmd;
// 		else
// 			curr->next = cmd;
// 		curr = cmd;
// 		if (tokens && tokens->type == PIPE)
// 			tokens = tokens->next;
// 	}
// }

void	parse_commands(t_token *tokens, t_shell *shell)
{
	t_token	*curr;
	t_cmd	*curr_cmd;
	t_cmd	*cmd;
	int		i;

	curr = tokens;
	i = 1;
	while (curr)
	{
		if (curr->type == PIPE)
			i++;
		curr = curr->next;
	}
	while (i--)
	{
		cmd = create_cmd();
		if (!cmd)
			return ;
		if (!shell->cmds)
			shell->cmds = cmd;
		else
			curr_cmd->next = cmd;
		curr_cmd = cmd;
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
