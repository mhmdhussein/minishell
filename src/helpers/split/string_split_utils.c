/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_split_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fel-ghaz <fel-ghaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 13:58:21 by fel-ghaz          #+#    #+#             */
/*   Updated: 2025/01/20 15:04:17 by fel-ghaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int is_space(char character)
{
	if (character == ' ' || character == '\t' || character == '\r')
		return (1);
	return (0);
}
int check_delimiter(char *str)
{
	if ((str[0] == '<' && str[1] == '<') || (str[0] == '>' && str[1] == '>'))
		return 1;
	if (str[0] == '>' || str[0] == '<' || str[0] == '|')
		return 1;
	if (is_space(str[0]))
		return 1;
	return 0;
}


bool input_has_unmatched_quotes(char *str)
{
	int i = 0;
	char current_quote = 0;

	while (str[i] != '\0')
	{
		if ((str[i] == '\'' || str[i] == '\"') && current_quote == 0)
			current_quote = str[i];
		else if (str[i] == current_quote)
			current_quote = 0;
		i++;
	}
	return (current_quote != 0);
}
int skip_quoted_sequence(char *str, int i)
{
	char quote_char;

	quote_char = str[i];
	i++;
	while (str[i] != '\0' && str[i] != quote_char)
		i++;
	if (str[i] == quote_char)
		i++;
	return i;
}
int skip_token(char *str, int i)
{
	while (str[i] != '\0' && !is_space(str[i]) && !check_delimiter((char[]){str[i], '\0'}))
	{
		if (str[i] == '\'' || str[i] == '\"')
			i = skip_quoted_sequence(str, i);
		else
			i++;
	}
	return i;
}

