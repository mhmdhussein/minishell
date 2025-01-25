/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohhusse <mohhusse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 01:49:28 by fel-ghaz          #+#    #+#             */
/*   Updated: 2025/01/21 14:17:52 by fel-ghaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
cleanup_shell: free everything

NOTE: needs fixing (NOT USABLE)
*/
void cleanup_shell(t_shell *shell)
{
	t_env *tmp;
	t_cmd *cmd_tmp;
	int i;

	while (shell->env)
	{
		tmp = shell->env;
		shell->env = shell->env->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
	}
	while (shell->cmds)
	{
		cmd_tmp = shell->cmds;
		shell->cmds = shell->cmds->next;
		i = 0;
		while (cmd_tmp->args && cmd_tmp->args[i])
			free(cmd_tmp->args[i++]);
		free(cmd_tmp->args);
		free(cmd_tmp->args[0]);
		free(cmd_tmp);
	}
}

t_env *copy_env(t_env *env)
{
	t_env *new_list;
	t_env *new_node;
	t_env *curr;

	new_list = NULL;
	curr = env;
	while (curr)
	{
		new_node = (t_env *)malloc(sizeof(t_env));
		if (!new_node)
			return (NULL);
		new_node->key = ft_strdup(curr->key);
		if (curr->value)
			new_node->value = ft_strdup(curr->value);
		else
			new_node->value = NULL;
		new_node->equal = true;
		new_node->next = new_list;
		new_list = new_node;

		curr = curr->next;
	}
	return (new_list);
}

/*
init_env: imports env and transforms it into a linked list

NOTE: needs memory leak fix
*/
void init_env(t_shell *shell, char **envp)
{
	int i;
	char *key;
	char *value;
	t_env *new_env;

	i = 0;
	while (envp[i])
	{
		key = ft_strtok(envp[i], "=");
		value = ft_strtok(NULL, "");
		new_env = (t_env *)malloc(sizeof(t_env));
		new_env->key = ft_strdup(key);
		new_env->equal = true;
		if (value)
			new_env->value = ft_strdup(value);
		else
			new_env->value = NULL;
		new_env->next = shell->env;
		shell->env = new_env;
		i++;
	}
}

/*
init_shell: initializes shell and env
*/

void init_shell(t_shell *shell, char **envp)
{
	shell->env = NULL;
	shell->cmds = NULL;
	shell->running = true;
	shell->last_exit_status = 0;
	init_env(shell, envp);
	shell->xenv = copy_env(shell->env);
	shell->xenv = merge_sort(shell->xenv);
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
		args[i] = curr->value;
		curr = curr->next;
		i++;
	}
	args[i] = NULL;
	return (args);
}

void print_tokens(t_token *start)
{
	t_token *current = start;
	while(current)
	{
		printf("%s\n", current->value);
		current = current->next;
	}
}
void exec(t_shell *shell, char *input)
{
	char **args;
	t_cmd *cmds;

	t_token  *tokens = tokenize(input);
	if (!tokens)
		return ;
	t_token *expansion = expand_variable(tokens, shell);
	args = detokenize(expansion);
	cmds = (t_cmd *)malloc(sizeof(t_cmd));
	cmds->args = args;
	cmds->next = NULL;
	if (is_builtin(cmds->args[0]))
		exec_builtin(cmds, shell);
	else
		execute_command(cmds,shell);
}

int main(int argc, char **argv, char **envp)
{
	char *input;
	t_shell shell;

	(void)argc;
	(void)argv;
	init_shell(&shell, envp);
	// printf("HELLOO");
	// fflush(stdout);
	while (shell.running)
	{
		input = readline("\033[37;41;1mmicroshell😭>\033[0m ");
		if (!input)
		{
			printf("\n");
			break;
		}
		if (*input)
		{
			add_history(input);
			exec(&shell, input);
		}
	}
	// cleanup_shell(&shell);
	return (0);
}
