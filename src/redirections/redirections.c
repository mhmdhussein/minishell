/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtraoui <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 13:30:19 by mohhusse          #+#    #+#             */
/*   Updated: 2025/03/25 10:52:13 by rtraoui          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	process_in(t_token *in, t_cmd *cmd)
{
	if (cmd->input_fd != -1)
		close(cmd->input_fd);
	cmd->input_fd = open(in->next->value, O_RDONLY);
	if (cmd->input_fd == -1)
		return; // handle error
	dup2(cmd->input_fd, STDIN_FILENO);
}

void	process_out(t_token *out, t_cmd *cmd)
{
	if (cmd->output_fd != -1)
		close(cmd->output_fd);
	cmd->output_fd = open(out->next->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (cmd->output_fd == -1)
		return; // handle error
	dup2(cmd->output_fd, STDOUT_FILENO);
}

void	process_append(t_token *app, t_cmd *cmd)
{
	if (cmd->output_fd != -1)
		close(cmd->output_fd);
	cmd->output_fd = open(app->next->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (cmd->output_fd == -1)
		return; // handle error
	dup2(cmd->output_fd, 1);
}

char	*create_tmp(void)
{
	char	*name;

	name = (char *)malloc(sizeof(char) * 2);
	if (!name)
		return (NULL);
	name[0] = 'p';
	name[1] = '\0';
	while (access(name, F_OK) == 0)
	{
		name = appendchar(name, 'p');
		if (!name)
			return (NULL);
	}
	return (name);
}

void	process_heredoc(t_token *heredoc, t_cmd *cmd, int std_out)
{
	char	*tmpfile;
	int		tmp_fd;
	char	*line;

	tmpfile = create_tmp();
	tmp_fd = open(tmpfile, O_WRONLY | O_CREAT, 0644);
	if (tmp_fd == -1)
		return ;
	cmd->delim = heredoc->next->value;
	while (1)
	{
		write(std_out, "> ", 2);
		line = readline("");
		if (!ft_strcmp(line, cmd->delim))
			break ;
		write(tmp_fd, line, ft_strlen(line));
		write(tmp_fd, "\n", 1);
	}
	if (cmd->input_fd != -1)
		close(cmd->input_fd);
	cmd->input_fd = open(tmpfile, O_RDONLY);
	if (cmd->input_fd == -1)
		return; // handle error
	dup2(cmd->input_fd, STDIN_FILENO);
	free(line);
	unlink(tmpfile);
	free(tmpfile);
	close(tmp_fd);
}

void	remove_redirection(t_token **tokens)
{
	t_token	*curr;
	t_token	*prev;

	if (!tokens || !(*tokens))
		return ;
	curr = *tokens;
	prev = NULL;
	while (curr && curr->next)
	{
		if (curr->type == IN || curr->type == OUT || curr->type == APPEND)
		{
			if (prev)
				prev->next = curr->next->next;
			else
				*tokens = curr->next->next;
			free(curr->next->value);
			free(curr->next);
			free(curr->value);
			free(curr);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}

void	process_redirections(t_shell *shell, t_cmd *cmd)
{
	t_token	*curr;

	curr = shell->tokens;
	while (curr)
	{
		if (curr->type == IN)
			process_in(curr, cmd);
		else if (curr->type == OUT)
			process_out(curr, cmd);
		else if (curr->type == APPEND)
			process_append(curr, cmd);
		if ((curr->type == IN || curr->type == OUT || curr->type == APPEND))
		{
			remove_redirection(&shell->tokens);
			curr = shell->tokens;
		}
		else
			curr = curr->next;
	}
	if (cmd->input_fd != -1)
		close(cmd->input_fd);
	if (cmd->output_fd != -1)
		close(cmd->output_fd);
}

void	remove_heredoc(t_token **tokens)
{
	t_token	*curr;
	t_token	*prev;

	if (!tokens || !(*tokens))
		return ;
	curr = *tokens;
	prev = NULL;
	while (curr && curr->next)
	{
		if (curr->type == HEREDOC)
		{
			if (prev)
				prev->next = curr->next->next;
			else
				*tokens = curr->next->next;
			free(curr->next->value);
			free(curr->next);
			free(curr->value);
			free(curr);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}

void	handle_heredoc(t_shell *shell, t_cmd *cmd)
{
	t_token	*curr;

	curr = shell->tokens;
	while (curr)
	{
		if (curr->type == HEREDOC)
		{
			process_heredoc(curr, cmd, shell->std_out);
			remove_heredoc(&shell->tokens);
			curr = shell->tokens;
		}
		else
			curr = curr->next;
	}
	if (cmd->input_fd != -1)
		close(cmd->input_fd);
}

int	redirections(t_shell *shell, t_cmd *cmd)
{
	t_token	*curr;

	curr = shell->tokens;
	while (curr)
	{
		if ((curr->type == IN || curr->type == OUT || curr->type == APPEND
				|| curr->type == HEREDOC) && (!curr->next || curr->next->type != WORD))
		{
			printf("Syntax error\n"); // free
			return (0);
		}
		curr = curr->next;
	}
	handle_heredoc(shell, cmd);
	process_redirections(shell, cmd);
	return (1);
}
