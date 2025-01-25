/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processing_quotes.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fel-ghaz <fel-ghaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 17:02:42 by fel-ghaz          #+#    #+#             */
/*   Updated: 2025/01/20 15:05:44 by fel-ghaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int count_tokens(char *str)
{
	int i = 0;
	int count = 0;

	if (input_has_unmatched_quotes(str))
	{
		printf("Error\n");
		return -1;
	}

	while (str[i] != '\0')
	{
		while (str[i] != '\0' && is_space(str[i]))
			i++;
		if (str[i] != '\0')
		{
			process_and_count_token(str, &i, NULL);
			count++;
		}
	}
	return count;
}
int handle_quoted_sequences(char *str, int *i, char *token)
{
	static int in_quotes = 0;
	static char quote_char = 0;
	int j = 0;

	if ((str[*i] == '\'' || str[*i] == '\"') && in_quotes == 0)
	{
		in_quotes = 1;
		quote_char = str[*i];
	}
	else if (str[*i] == quote_char && in_quotes)
	{
		in_quotes = 0;
		quote_char = 0;
	}
	else
		token[j++] = str[*i];
	(*i)++;
	return j;
}

int process_quotes(const char *str, int *i, int *state, char *token)
{
	static int in_quotes;
	static char quote_char;

	in_quotes = 0;
	quote_char = 0;
	if ((str[*i] == '\'' || str[*i] == '\"') && in_quotes == 0)
	{
		in_quotes = 1;
		quote_char = str[*i];
	}
	else if (str[*i] == quote_char && in_quotes)
	{
		in_quotes = 0;
		quote_char = 0;
	}
	else if (token)
		token[(*state)++] = str[*i];
	else
		(*state)++;
	(*i)++;
	return (*state);
}
int handle_quoted_sequence(char *str, int *i, char *quote_char)
{
	if ((str[*i] == '\'' || str[*i] == '\"') && *quote_char == 0)
		*quote_char = str[*i];
	else if (str[*i] == *quote_char)
		*quote_char = 0;
	(*i)++;
	return (1);
}
int handle_delimiter(char *str, int *i, char *token, int *j)
{
    if ((str[*i] == '<' && str[*i + 1] == '<') || (str[*i] == '>' && str[*i + 1] == '>'))
    {
        if (token)
        {
            token[(*j)++] = str[*i];
            token[(*j)++] = str[*i + 1];
            token[*j] = '\0';
        }
        (*i) += 2;
        return 2;
    }
    if (str[*i] == '>' || str[*i] == '<' || str[*i] == '|')
    {
        if (token)
        {
            token[(*j)++] = str[*i];
            token[*j] = '\0';
        }
        (*i)++;
        return 1;
    }
    return 0;
}

