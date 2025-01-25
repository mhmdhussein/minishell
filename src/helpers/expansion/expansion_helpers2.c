/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_helpers2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fel-ghaz <fel-ghaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 08:45:10 by fel-ghaz          #+#    #+#             */
/*   Updated: 2025/01/21 08:47:03 by fel-ghaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	ft_isspecial(int c)
{
	return (c == 33 || c == 35 || c == 37 || c == 38
		|| (c >= 40 && c <= 47) || c == 58 || (c == 59)
			|| c == 61 || c == 63 || c == 64 || (c >= 91 && c <= 94)
				|| c == 96 || c == 123 || c == 125 || c == 126);
}
char *ft_strjoincustom(char *s1, char *s2)
{
	char			*join;
	unsigned int	len1;
	unsigned int	len2;

	if (!s1 && !s2)
		return (ft_strdup(""));
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	join = (char *)malloc(len1 + len2 + 1);
	if (!join)
		return (NULL);
	ft_memcpy(join, s1, len1);
	ft_memcpy(join + len1, s2, len2);
	join[len1 + len2] = '\0';
	free(s1);
	return (join);
}
char *handle_env_variable(char *result, t_token *token, t_shell *shell, int *i)
{
	char *key;
	char *env;
	int start;

	(*i)++;
	start = *i;
	while (ft_isalnum(token->value[*i]) || token->value[*i] == '_')
		(*i)++;
	key = ft_substr(token->value, start, *i - start);
	env = envget(shell->env, key);
	if (env)
		result = ft_strjoincustom(result, env);
	free(key);
	(*i)--;
	return result;
}

void expand(t_token *token, t_shell *shell)
{
	char *result = ft_strdup("");
	int i = 0;

	while (token->value[i])
	{
		if (token->value[i] == '$' && (ft_isalpha(token->value[i + 1]) || token->value[i + 1] == '_'))
			result = handle_env_variable(result, token, shell, &i);
		else
			result = appendchar(result, token->value[i]);
		i++;
	}
	free(token->value);
	token->value = result;
}
void expand_number(t_token *token)
{
	char *result = ft_strdup("");
	int i = 0;

	while (token->value[i])
	{
		if (token->value[i] == '$' && ft_isdigit(token->value[i + 1]))
			i++;
		else
			result = appendchar(result, token->value[i]);
		i++;
	}
	free(token->value);
	token->value = result;
}
