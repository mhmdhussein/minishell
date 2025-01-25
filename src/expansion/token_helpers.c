/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohhusse <mohhusse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 16:22:25 by fel-ghaz          #+#    #+#             */
/*   Updated: 2025/01/25 15:52:30 by mohhusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token_type	check_variable(t_token *linked_list)
{
	while (linked_list != NULL)
	{
		if (linked_list->type == ENV_VAR)
			return (linked_list->type);
		linked_list = linked_list->next;
	}
	return (-1);
}

void	check_quotes(char c, int *quote)
{
	if (c == '"' && *quote != 1)
	{
		if (*quote == 2)
			*quote = 0;
		else
			*quote = 2;
	}
	else if (c == '\'' && *quote != 2)
	{
		if (*quote == 1)
			*quote = 0;
		else
			*quote = 1;
	}
}

char	*appendchar(char *str, char c)
{
	char	temp[2];
	char	*new_str;

	temp[1] = '\0';
	new_str = ft_strjoincustom(str, temp);
	return (new_str);
}

t_token	*expand_variable(t_token *tokens, t_shell *shell)
{
	t_token	*curr;
	char	*new_value;

	curr = tokens;
	while (curr)
	{
		expand_general(curr, shell);
		new_value = remove_quotes(curr->value, shell);
		free(curr->value);
		curr->value = new_value;
		curr = curr->next;
	}
	return (tokens);
}

/*
WRONG CASES:
echo $"$"
echo $'HO"ME'
*/
