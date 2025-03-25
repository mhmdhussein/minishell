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
	printf("outputfd = %i\n", cmd->output_fd);
	dup2(cmd->output_fd, STDOUT_FILENO);
	printf("f\n");
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

void	process_heredoc()
{
	printf("cry\n");
}

void	remove_redirection(t_token **curr)
{
	t_token	*temp;

	temp = NULL;
	if ((*curr)->next->next)
		temp = (*curr)->next->next;
	free((*curr)->next);
	free((*curr));
	*curr = temp;

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
		else if (curr->type == HEREDOC)
			process_heredoc();
		if ((curr->type == IN || curr->type == OUT || curr->type == APPEND
			|| curr->type == HEREDOC))
			remove_redirection(&curr);
		else if (curr && curr->next)
			curr = curr->next;
		else
			curr = NULL;
	}
	if (cmd->input_fd != -1)
		close(cmd->input_fd);
	if (cmd->output_fd != -1)
		close(cmd->output_fd);
}

int	redirections(t_shell *shell, t_cmd *cmd)
{
	t_token	*curr;

	curr = shell->tokens;
	while (curr)
	{
		if ((curr->type == IN || curr->type == OUT || curr->type == APPEND
				|| curr->type == HEREDOC) && curr->next->type != WORD)
		{
			printf("Syntax error\n"); // free
			return (0);
		}
		curr = curr->next;
	}
	process_redirections(shell, cmd);
	printf ("end\n");
	return (1);
}
