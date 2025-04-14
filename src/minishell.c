/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtraoui <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 14:33:59 by mohhusse          #+#    #+#             */
/*   Updated: 2025/03/25 10:55:02 by rtraoui          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
frees everything

NOTE: needs fixing (NOT USABLE)
*/
void	cleanup_shell(t_shell *shell)
{
	free_env(shell->env);
	free_cmds(shell->cmds);
	free_tokens(shell->tokens);
	free(shell);
}

/*
imports env and transforms it into a linked list

NOTE: needs memory leak fix
*/
void	init_env(t_shell *shell, char **envp)
{
	int		i;
	char	*key;
	char	*value;
	t_env	*new_env;

	i = 0;
	while (envp[i])
	{
		key = ft_strtok(envp[i], "=");
		value = ft_strtok(NULL, "");
		new_env = create_env(key, value);
		if (!new_env)
			return ; // free
		if (value)
			new_env->equal = true;
		else
			new_env->equal = false;
		new_env->next = shell->env;
		shell->env = new_env;
		i++;
	}
}

/*
initializes shell and env
*/
void	init_shell(t_shell *shell, char **envp)
{
	shell->env = NULL;
	shell->cmds = NULL;
	shell->tokens = NULL;
	shell->running = true;
	shell->last_exit_status = 0;
	shell->pipe_mode = false;
	init_env(shell, envp);
}

int	counttokens(t_token *tokens)
{
	int		i;
	t_token	*curr;

	curr = tokens;
	i = 0;
	while (curr)
	{
		i++;
		curr = curr->next;
	}
	return (i);
}

char	**detokenize(t_token *tokens)
{
	t_token	*curr;
	int		i;
	char	**args;

	curr = tokens;
	args = (char **)malloc(sizeof(char *) * (counttokens(tokens) + 1));
	i = 0;
	while (curr)
	{
		args[i] = ft_strdup(curr->value);
		curr = curr->next;
		i++;
	}
	args[i] = NULL;
	return (args);
}

void	print_tokens(t_token *start)
{
	t_token	*current;

	current = start;
	while (current)
	{
		printf("%s\ttype:%d\n", current->value, current->type);
		current = current->next;
	}
}

void	print_cmds(t_cmd *cmds)
{
	t_cmd	*curr;
	int		i;

	curr = cmds;
	i = 1;
	while (curr)
	{
		printf("Command %d:\n", i++);
		for (int j = 0; curr->args[j]; j++)
			printf("%s ", curr->args[j]);
		printf("\n\n");
		curr = curr->next;
	}
}

void	exec(t_shell *shell, char *input)
{
	char	**args;
	t_cmd	*cmds;
	t_token	*tokens;

	tokens = tokenize(input);
	if (!tokens)
		return ;
	shell->tokens = tokens;
	expand_variables(shell->tokens, shell);
	if (check_pipes(shell->tokens) == -1)
	{
		printf("bash: syntax error near unexpected token `|'\n");
		return ;
	}
	else if (check_pipes(shell->tokens) == 1)
	{
		shell->pipe_mode = true;
		parse_commands(shell->tokens, shell);
		shell->std_out = dup(STDOUT_FILENO);
		handle_pipes(shell);
		free_cmds(shell->cmds);
		shell->cmds = NULL;
	}
	else
	{
		cmds = (t_cmd *)malloc(sizeof(t_cmd));
		if (!cmds)
			return ;
		cmds->input_fd = -1;
		cmds->output_fd = -1;
		shell->std_in = dup(STDIN_FILENO);
		shell->std_out = dup(STDOUT_FILENO);
		shell->cmds = cmds;
		if (!redirections(shell, shell->cmds, &shell->tokens))
			return ;
		if (shell->tokens && shell->cmds->input_fd != -2)
		{
			args = detokenize(shell->tokens);
			cmds->args = args;
			cmds->next = NULL;
			if (is_builtin(cmds->args[0]))
				exec_builtin(cmds, shell);
			else
				execute_command(cmds, shell);
		}
		dup2(shell->std_out, STDOUT_FILENO);
		dup2(shell->std_in, STDIN_FILENO);
		close(shell->std_in);
		close(shell->std_out);
		free_cmds(shell->cmds);
		shell->cmds = NULL;
	}
}

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	t_shell	shell;

	(void)argc;
	(void)argv;
	init_shell(&shell, envp);
	while (shell.running)
	{
		input = readline("\033[37;41;1mmicroshell😭:\033[0m ");
		if (!input)
		{
			printf("\n");
			break ;
		}
		if (*input)
		{
			add_history(input);
			exec(&shell, input);
		}
		free(input);
	}
	return (0);
}
