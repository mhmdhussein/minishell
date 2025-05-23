/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohhusse <mohhusse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 13:45:57 by mohhusse          #+#    #+#             */
/*   Updated: 2025/03/29 13:45:57 by mohhusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

void	process_heredoc(t_token *heredoc, t_cmd *cmd, int std_out, t_shell *shell)
{
	char	*tmpfile;
	char	*expanded_line;
	int		tmp_fd;
	char	*line;
	bool	expand;

	expand = true;
	tmpfile = create_tmp();
	tmp_fd = open(tmpfile, O_WRONLY | O_CREAT, 0644);
	if (tmp_fd == -1)
		return ;
	cmd->delim = heredoc->next->value;
	if (ft_strchr(cmd->delim, '\"') || ft_strchr(cmd->delim, '\''))
		expand = false;
	cmd->delim = remove_quotes(cmd->delim);
	dup2(shell->std_in, STDIN_FILENO);
	while (1)
	{
		write(std_out, "> ", 2);
		line = readline("");
		if (!*line)
		{
			write(std_out, "\n", 1);
			continue ;
		}
		if (!ft_strcmp(line, cmd->delim))
			break ;
		if (expand)
		{
			expanded_line = expand_token(line, shell);
			write(tmp_fd, expanded_line, ft_strlen(expanded_line));
			free(expanded_line);
		}
		else
			write(tmp_fd, line, ft_strlen(line));
		write(tmp_fd, "\n", 1);
		free(line);
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

void	handle_heredoc(t_shell *shell, t_cmd *cmd, t_token **tokens)
{
	t_token	*curr;

	curr = *tokens;
	while (curr)
	{
		if (curr->type == HEREDOC)
		{
			process_heredoc(curr, cmd, shell->std_out, shell);
			remove_heredoc(tokens);
			curr = *tokens;
		}
		else
			curr = curr->next;
	}
	if (cmd->input_fd != -1)
		close(cmd->input_fd);
}
