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

	int j = 0;
	printf("%i\n", j++);
	curr = tokens;
	printf("%i\n", j++);
	args = (char **)malloc(sizeof(char *) * (counttokens(tokens) + 1));
	printf("%i\n", j++);
	i = 0;
	printf("%i\n", j++);
	while (curr)
	{
		printf("%ia\n", j++);
		args[i] = curr->value;
		printf("%ia\n", j++);
		curr = curr->next;
		printf("%ia\n", j++);
		i++;
		printf("%ia\n", j++);
		printf("restart\n");
	}
	printf("end_loop\n");
	args[i] = NULL;
	printf("0\n");
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
	cmds = (t_cmd *)malloc(sizeof(t_cmd));
	if (!cmds)
		return ;
	cmds->input_fd = -1;
	cmds->output_fd = -1;
	redirections(shell, cmds);
	printf ("redirections out\n");
	// return ;
	args = detokenize(shell->tokens);
	printf ("111\n");
	cmds->args = args;
	printf ("222\n");
	cmds->next = NULL;
	printf("333\n");
	if (is_builtin(cmds->args[0]))
	{
		printf("before_builtin\n");
		exec_builtin(cmds, shell);
		printf("after_builtin\n");
	}
	else
	{
		printf("before_exec\n");
		execute_command(cmds, shell);
		printf("after_exec\n");
	}
}

char	*displaymessage(t_shell *shell)
{
	char	*path;
	char	*s1;
	char	*s2;

	path = envget(shell->env, "PWD");
	if (path)
	{
		s1 = ft_strjoin("\033[37;41;1mmicroshell😭:\033[0m\033[31m", path);
		if (!s1)
			return (NULL);
		s2 = ft_strjoin(s1, "> \033[0m");
		free(s1);
		if (!s2)
			return (NULL);
		return (s2);
	}
	else
	{
		s1 = ft_strdup("\033[37;41;1mmicroshell😭\033[0m\033[31m >\033[0m ");
		if (!s1)
			return (NULL);
		return (s1);
	}
}

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	char	*display;
	t_shell	shell;

	(void)argc;
	(void)argv;
	init_shell(&shell, envp);
	while (shell.running)
	{
		display = displaymessage(&shell);
		if (!display)
			break ;
		input = readline(display);
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
