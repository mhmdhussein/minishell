/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fel-ghaz <fel-ghaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 14:39:39 by fel-ghaz          #+#    #+#             */
/*   Updated: 2025/01/21 14:04:45 by fel-ghaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

char *handle_expansion_loop(t_env *env, t_shell *shell)
{
	check_quotes(env->exp_value[env->exp_index], &env->exp_quote);
	if (env->exp_value[env->exp_index] == '$' &&
		(env->exp_quote == 0 || env->exp_quote == 2))
	{
		if (env->exp_index > 0 || env->exp_value[env->exp_index + 1] == '$' ||
			ft_isdigit(env->exp_value[env->exp_index + 1]) ||
			ft_isalpha(env->exp_value[env->exp_index + 1]) ||
			env->exp_value[env->exp_index + 1] == '_')
			env->exp_result = expand_quote_cases(
				env->exp_result, env->exp_value, &env->exp_index, shell);
		else
			env->exp_result = appendchar(env->exp_result, env->exp_value[env->exp_index]);
	}
	else
		env->exp_result = appendchar(env->exp_result, env->exp_value[env->exp_index]);
	return env->exp_result;
}
void expand_general(t_token *token, t_shell *shell)
{
	t_env *env;

	env = shell->env;
	env->exp_quote = 0;
	env->exp_index = 0;
	env->exp_value = token->value;
	env->exp_result = ft_strdup("");
	if (env->exp_value[0] == '$' && env->exp_value[1] == '"' && env->exp_value[2] == '$')
		env->exp_result = handle_dollar_quote_dollar(env);
	while (env->exp_value[env->exp_index])
	{
		env->exp_result = handle_expansion_loop(env, shell);
		env->exp_index++;
	}
	free(token->value);
	token->value = env->exp_result;
	// env->exp_result = NULL; // chatgpt al hayl: Reset temporary variables in `t_env` to avoid polluting the state.
	// env->exp_value = NULL;
	// env->exp_index = 0;
	// env->exp_quote = 0;
}
char	*expand2(t_shell *shell, char *key)
{
	t_env *current = shell->env;

	while (current != NULL)
	{
		if (strcmp(current->key, key) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

void expand_cases(t_token *token, t_shell *shell, int *i)
{
	t_token *curr = token;
	if (ft_strchr(token->value, '\''))
        curr = curr->next;
	else if (ft_strchr(token->value, '\"'))
		expand_general(token, shell);
	else
	{
		expand_dollar(token, shell, i);
		expand_number(token);
		expand(token, shell);
	}
}
char *remove_quotes(char *value, t_shell *shell)
{
	(void) shell;
	char *result = ft_strdup("");
	int quote = 0;
	int i = 0;

	while (value[i])
	{
		if (value[i] == '\'' && quote == 0)
			quote = 1;
		else if (value[i] == '\'' && quote == 1)
			quote = 0;
		else if (value[i] == '\"' && quote == 0)
			quote = 2;
		else if (value[i] == '\"' && quote == 2)
			quote = 0;
		else
			result = appendchar(result, value[i]);
		i++;
	}
	return (result);
}
