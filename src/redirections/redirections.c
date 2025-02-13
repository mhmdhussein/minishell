/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohhusse <mohhusse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 13:30:19 by mohhusse          #+#    #+#             */
/*   Updated: 2025/02/10 14:00:34 by mohhusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
** handle_heredoc:
**   - Opens a pipe.
**   - Reads input lines with a "heredoc> " prompt until a line matching
**     the given delimiter is encountered.
**   - Writes all lines (with a newline) into the pipe.
**   - Closes the write end and returns the read end, which will serve as
**     the command’s STDIN.
*/
int	handle_heredoc(char *delim)
{
	int		pipe_fd[2];
	char	*line;

	if (pipe(pipe_fd) < 0)
	{
		perror("pipe");
		return (-1);
	}
	while (1)
	{
		line = readline("heredoc> ");
		if (!line)
			break ;
		if (ft_strcmp(line, delim) == 0)
		{
			free(line);
			break ;
		}
		write(pipe_fd[1], line, ft_strlen(line));
		write(pipe_fd[1], "\n", 1);
		free(line);
	}
	close(pipe_fd[1]);
	return (pipe_fd[0]);
}

/*
** set_input_redirection:
**   - If a heredoc delimiter is present in the command, call handle_heredoc().
**   - Otherwise, if an input file is specified, open it for reading.
**   - Returns 0 on success and -1 on error.
*/
int	set_input_redirection(t_cmd *cmd)
{
	if (cmd->delim)
	{
		cmd->input_fd = handle_heredoc(cmd->delim);
		if (cmd->input_fd < 0)
			return (-1);
	}
	else if (cmd->input_file)
	{
		cmd->input_fd = open(cmd->input_file, O_RDONLY);
		if (cmd->input_fd < 0)
		{
			perror("open input file");
			return (-1);
		}
	}
	return (0);
}

/*
** set_output_redirection:
**   - If an output file is specified, open it with the proper flags:
**       * ">"  uses O_TRUNC
**       * ">>" uses O_APPEND
**   - Returns 0 on success and -1 on error.
*/
int	set_output_redirection(t_cmd *cmd)
{
	int	flags;

	if (cmd->output_file)
	{
		if (cmd->append)
			flags = O_WRONLY | O_CREAT | O_APPEND;
		else
			flags = O_WRONLY | O_CREAT | O_TRUNC;
		cmd->output_fd = open(cmd->output_file, flags, 0644);
		if (cmd->output_fd < 0)
		{
			perror("open output file");
			return (-1);
		}
	}
	return (0);
}

/*
** apply_redirections:
**   - Calls set_input_redirection() and set_output_redirection() to obtain
**     the file descriptors.
**   - Then, if redirection is specified, duplicates the file descriptors onto
**     STDIN_FILENO and STDOUT_FILENO (using dup2) so that the upcoming exec call
**     will use the new I/O.
*/
int	apply_redirections(t_cmd *cmd)
{
	if (set_input_redirection(cmd) < 0)
		return (-1);
	if (set_output_redirection(cmd) < 0)
		return (-1);
	if (cmd->input_file || cmd->delim)
	{
		if (dup2(cmd->input_fd, STDIN_FILENO) < 0)
		{
			perror("dup2 input");
			return (-1);
		}
		close(cmd->input_fd);
	}
	if (cmd->output_file)
	{
		if (dup2(cmd->output_fd, STDOUT_FILENO) < 0)
		{
			perror("dup2 output");
			return (-1);
		}
		close(cmd->output_fd);
	}
	return (0);
}

/*
** remove_token:
**   - Helper function that removes a token from the doubly linked list and frees it.
*/
void	remove_token(t_token **head, t_token *token)
{
	if (!head || !(*head) || !token)
		return ;
	if (token->previous)
		token->previous->next = token->next;
	else
		*head = token->next;
	if (token->next)
		token->next->previous = token->previous;
	free(token->value);
	free(token);
}

/*
** process_redirections:
**   - Walks through the token list.
**   - When a redirection token (<, <<, >, >>) is found, its following token
**     (assumed to be a WORD) holds the file name or delimiter.
**   - Updates the t_cmd structure accordingly:
**         * For "<": sets cmd->input_file.
**         * For "<<": sets cmd->delim.
**         * For ">": sets cmd->output_file and cmd->append = false.
**         * For ">>": sets cmd->output_file and cmd->append = true.
**   - Also removes the redirection tokens (and the following WORD token) from
**     the token list so they aren’t passed as command arguments.
*/
void	process_redirections(t_cmd *cmd, t_token **tokens)
{
	t_token	*tmp;
	t_token	*next;

	tmp = *tokens;
	while (tmp)
	{
		if (tmp->type == IN && tmp->next)
		{
			cmd->input_file = ft_strdup(tmp->next->value);
			next = tmp->next;
			remove_token(tokens, tmp);
			remove_token(tokens, next);
			tmp = *tokens;
			continue ;
		}
		else if (tmp->type == HEREDOC && tmp->next)
		{
			cmd->delim = ft_strdup(tmp->next->value);
			next = tmp->next;
			remove_token(tokens, tmp);
			remove_token(tokens, next);
			tmp = *tokens;
			continue ;
		}
		else if (tmp->type == OUT && tmp->next)
		{
			cmd->output_file = ft_strdup(tmp->next->value);
			cmd->append = false;
			next = tmp->next;
			remove_token(tokens, tmp);
			remove_token(tokens, next);
			tmp = *tokens;
			continue ;
		}
		else if (tmp->type == APPEND && tmp->next)
		{
			cmd->output_file = ft_strdup(tmp->next->value);
			cmd->append = true;
			next = tmp->next;
			remove_token(tokens, tmp);
			remove_token(tokens, next);
			tmp = *tokens;
			continue ;
		}
		tmp = tmp->next;
	}
}
