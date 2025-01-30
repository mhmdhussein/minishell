/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanddd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohhusse <mohhusse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 14:39:39 by fel-ghaz          #+#    #+#             */
/*   Updated: 2025/01/30 11:29:47 by mohhusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token_type	check_variable(t_token *linked_list)
{
	while (linked_list != NULL)
	{
		if (linked_list->type == ENV_VAR)
			return linked_list->type;
		linked_list = linked_list->next;
	}
	return (-1);
}

void check_quotes(char c, int *quote)
{
	if (c == '"')
		*quote = 2;
	if (c == '\'')
		*quote = 1;
	if ((c == '"' && *quote == 2) || (c == '\'' && *quote == 1))
		*quote = 0;
}

int	ft_isspecial(int c)
{
	return (c == 33 || c == 35 || c == 37 || c == 38 || (c >= 40 && c <= 47) || c == 58 || (c == 59) || c == 61 || c == 63 || c == 64 || (c >= 91 && c <= 94) || c == 96 || c == 123 || c == 125 || c == 126);
}

char	*appendchar(char *str, char c)
{
	char temp[2];

	temp[0] = c;
	temp[1] = '\0';
	str = ft_strjoincustom(str, temp);
	return (str);
}

void	expand(t_token *token, t_shell *shell)
{
	char	*result;
	char	*env;
	char	*key;
	int		i;

	result = ft_strdup("");
	key = ft_strdup("");
	i = 0;
	while (token->value[i])
	{
		if (token->value[i] == '$' && (ft_isalpha(token->value[i + 1]) || token->value[i] == '_'))
		{
			i++;
			while (token->value[i] != '\0' && (ft_isalpha(token->value[i + 1]) || token->value[i] != '_' || ft_isdigit(token->value[i])))
				key = appendchar(key, token->value[i]);
			env = envget(shell->env, key);
			if (env)
				result = ft_strjoincustom(result, env);
		}
		else
			result = appendchar(result, token->value[i]);
		i++;
	}
	token->value= result;
}

void	expand_number(t_token *token)
{
	char	*result;
	int		i;

	result = ft_strdup("");
	i = 0;
	while (token->value[i])
	{
		if (token->value[i] == '$' && ft_isdigit(token->value[i + 1]))
			i++;
		else
			result = appendchar(result, token->value[i]);
		i++;
	}
	token->value = result;
}

void	expand_dollar(t_token *token)
{
	int		count;
	int		i;
	char	*result;

	count = 0;
	i = 0;
	result = ft_strdup("");
	while (token->value[i])
	{
		if (token->value[i] == '$' && token->value[i + 1] == '$')
		{
			result = ft_strjoincustom(result, ft_strdup("42"));
			i += 2;
		}
		else
			result = appendchar(result, token->value[i]);
		i++;
	}
	token->value = result;
}

char	*expand_quote_cases(char *result, char *value, int *i, t_shell *shell)
{
	char	*key;
	char	*env;

	if (value[++(*i)] == '$')
	{
		result = ft_strjoincustom(result, ft_strdup("42"));
		(*i)++;
	}
	else if (ft_isdigit(value[*i]))
		(*i)++;
	else if (ft_isalpha(value[*i]) || value[*i] == '_')
	{
		while (value[*i] && (ft_isalpha(value[*i]) || ft_isdigit(value[*i]) || value[*i] == '_'))
		{
			key = ft_strdup("");
			key = appendchar(key, value[*i]);
			(*i)++;
		}
		env = envget(shell->env, key);
		if (env)
			result = ft_strjoincustom(result, env);
	}
	else
	{
		--(*i);
		result = appendchar(result, value[*i]);
		i++;
	}
	return (result);

}

void	expand_quote(t_token *token, t_shell *shell)
{
	int		quote;
	char	*result;
	int		i;

	quote = 0;
	result = ft_strdup("");
	i = -1;
	while (token->value[++i])
	{
		check_quotes(token->value[i], &quote);
		if (token->value[i] == '$' && (quote == 0 || quote == 2))
			result = expand_quote_cases(result, token->value, &i, shell);
		else
			result = appendchar(result, token->value[i]);
	}
	token->value = result;
}

void	expand_cases(t_token *token, t_shell *shell)
{
	if (ft_strchr(token->value, '\"'))
	{
		expand_quote(token, shell);
		return ;
	}
	expand_dollar(token);
	expand_number(token);
	expand(token, shell);
}

t_token	*expand_variable(t_token *tokens, t_shell *shell)
{
	t_token *curr;

	curr = tokens;
	while (curr)
	{
		if (curr->type == ENV_VAR)
			expand_cases(curr, shell);
		curr = curr->next;
	}
	return (tokens);
}
