/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_helpers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fel-ghaz <fel-ghaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 10:46:27 by fel-ghaz          #+#    #+#             */
/*   Updated: 2025/01/21 08:44:35 by fel-ghaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

char *handle_dollar_digit_cases(char *result, char *value, int *i)
{
	if (value[*i + 1] == '$')
	{
		result = ft_strjoincustom(result, "42");
		(*i)++;
		return result;
	}
	else if (ft_isdigit(value[*i + 1]))
	{
		(*i)++;
		if (value[*i + 1])
		{
			while (value[*i + 1] && ft_isdigit(value[*i + 1]))
			{
				result = appendchar(result, value[*i + 1]);
				(*i)++;
			}
		}
	}
	return result;
}
char *expand_quote_cases(char *result, char *value, int *i, t_shell *shell)
{
	char *key = NULL;
	char *env = NULL;
	int start;
	if (value[*i + 1] == '$' || ft_isdigit(value[*i + 1]))
		return handle_dollar_digit_cases(result, value, i);
	else if (ft_isalpha(value[*i + 1]) || value[*i + 1] == '_')
	{
		(*i)++;
		start = *i;
		while (ft_isalnum(value[*i]) || value[*i] == '_')
			(*i)++;
		key = ft_substr(value, start, *i - start);
		env = envget(shell->env, key);
		if (env)
			result = ft_strjoincustom(result, env);
		free(key);
		(*i)--;
	}
	else
		result = appendchar(result, value[*i]);
	return result;
}
char *expand_dollar(t_token *token, t_shell *shell, int *i)
{
	char *result = ft_strdup("");
	char *key = NULL;
	char *env_value = NULL;
	int *start = i + 1;

	if (token->value[*start] == '$')
	{
		i = start;
		return ft_strdup("42");
	}
	while (ft_isalnum(token->value[*i + 1]) || token->value[*i + 1] == '_')
		(*i)++;
	key = ft_substr(token->value, (unsigned int)(*start), (unsigned int)(*i - *start + 1));

	env_value = expand2(shell, key);
	free(key);
	if (env_value)
		result = ft_strjoincustom(result, env_value);
	return result;
}
char *handle_dollar_quote_dollar(t_env *env)
{
	if (env->exp_value[3] == '"')
	{
		env->exp_result = appendchar(env->exp_result, '$');
		env->exp_index = 4;  // hay eza 3nde $"$"
	}
	else if (ft_isdigit(env->exp_value[3]))
	{
		env->exp_index = 4;
		while (env->exp_value[env->exp_index] && env->exp_value[env->exp_index] != '"')
		{
			env->exp_result = appendchar(env->exp_result, env->exp_value[env->exp_index]);
			env->exp_index++;
		}
		env->exp_index++;
	}
	return env->exp_result;
}
