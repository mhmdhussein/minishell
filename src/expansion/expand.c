/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohhusse <mohhusse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 13:23:07 by mohhusse          #+#    #+#             */
/*   Updated: 2025/03/17 12:34:44 by mohhusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_var_char(char c, int first)
{
	if (first)
		return (ft_isalpha(c) || c == '_');
	return (ft_isalnum(c) || c == '_');
}

char	*extract_variable_name(char *value, int *i)
{
	int		start;
	int		len;
	char	*var_name;

	start = *i;
	len = 0;
	if(!is_var_char(value[start], 1))
		return (NULL);
	while (value[start + len] && is_var_char(value[start + len], 0))
		len++;
	var_name = ft_substr(value, start, len);
	*i += len - 1;
	return (var_name);
}

char	*handle_variable(char *value, int *i, t_shell *shell, int quote)
{
	char	*var_name;
	char	*var_value;

	if ((value[(*i) + 1] == '\"' || value[(*i) + 1] == '\'') && quote == 0)
		return (ft_strdup(""));
	else if ((value[(*i) + 1] == '\"' || value[(*i) + 1] == '\'') && quote == 2)
		return (ft_strdup("$"));
	if (value[(*i) + 1] == ':' || value[(*i) + 1] == '=')
		return (ft_strdup("$"));
	(*i)++;
	if (value[*i] == '$')
		return (ft_strdup("42"));
	else if (value[*i] == '?')
		return (ft_itoa(shell->last_exit_status));
	else if (!is_var_char(value[*i], 1))
		return (ft_strdup(""));
	var_name = extract_variable_name(value, i);
	if (!var_name)
		return (ft_strdup("$")); //?
	var_value = envget(shell->env, var_name);
	free(var_name);
	if (var_value)
		return (ft_strdup(var_value));
	else
		return (ft_strdup(""));
}

char	*append_variable(char *result, char *expanded)
{
	char	*temp;

	temp = result;
	if (expanded)
	{
		result = ft_strjoin(result, expanded);
		free(temp);
	}
	return (result);
}

char	*expand_token(char *value, t_shell *shell)
{
	int		i;
	int		quote;
	char	*result;
	char	*expanded;

	i = 0;
	quote = 0;
	result = ft_strdup("");
	while (value[i])
	{
		check_quotes(value[i], &quote);
		if (value[i] == '$' && (quote == 0 || quote == 2 ) && value[i + 1] && value[i + 1] != ' ')
		{
			expanded = handle_variable(value, &i, shell, quote);
			result = append_variable(result, expanded);
			free(expanded);
		}
		else
			result = appendchar(result, value[i]);
		i++;
	}
	if (result[0] == '\0')
		return (NULL);
	return (result);
}

void	split_token(t_token **prev, t_token **curr, t_token **tokens)
{
	t_token	*new_list;
	t_token	*new_curr;

	if ((*curr)->value[0] == '\"' || (*curr)->value[0] == '\'')
		return ;
	new_list = tokenize((*curr)->value);
	if (!new_list)
		return ;
	if ((*prev))
		(*prev)->next = new_list;
	else
		*tokens = new_list;
	new_curr = new_list;
	while (new_curr->next)
		new_curr = new_curr->next;
	new_curr->next = (*curr)->next;
	free((*curr)->value);
	free((*curr));
	(*curr) = new_curr;
}

void	expand_variables(t_token *tokens, t_shell *shell)
{
	t_token	*curr;
	t_token	*prev;
	char	*expanded;
	char	*temp;

	curr = tokens;
	prev = NULL;
	while (curr)
	{
		// we need to re-code this part
		// first, if env_var, the process of of removing quotes and expanding should be parallel
		// and at the same time
		// but if type is word, remove quotes normally
		// ignore quotes of different types (quotes inside quotes)
		if (curr->type == ENV_VAR)
		{
			expanded = expand_token(curr->value, shell);
			free(curr->value);
			curr->value = expanded;
			curr->type = WORD;
			if ((!prev || prev->type != HEREDOC) && curr->value)
				split_token(&prev, &curr, &tokens);
		}
		else if ((!prev || prev->type != HEREDOC) && curr->value)
		{
			temp = curr->value;
			curr->value = remove_quotes(curr->value);
			free(temp);
		}
		prev = curr;
		curr = curr->next;
	}
}
