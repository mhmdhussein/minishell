/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fel-ghaz <fel-ghaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 13:58:18 by fel-ghaz          #+#    #+#             */
/*   Updated: 2025/01/20 15:05:10 by fel-ghaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int process_and_count_token(char *str, int *i, char *token)
{
    int len = 0, j = 0;
    char quote_char = 0;

    int delim_len = handle_delimiter(str, i, token, &j);
    if (delim_len > 0)
        return delim_len;
    while (str[*i] != '\0')
    {
        handle_quoted_sequence(str, i, &quote_char);
        if (token)
            token[j++] = str[*i - 1];
        len++;
        if (quote_char == 0 && (is_space(str[*i]) || check_delimiter((char[]){str[*i], '\0'})))
            break;
    }
    if (token)
        token[j] = '\0';
    return len;
}
char *copy_token(char *str, int start, int end)
{
    int len;
    int i;

    i = start;
    (void)end;
    len = process_and_count_token(str, &i, NULL);
    char *token = (char *)malloc(sizeof(char) * (len + 1));
    if (!token)
        return NULL;
    i = start;
    process_and_count_token(str, &i, token);
    return token;
}
void ft_split_helper1(char *str, int token_start, int *ij, char **tokens)
{
	int end = token_start;

	process_and_count_token(str, &end, NULL);
	tokens[ij[1]++] = copy_token(str, token_start, end);
	ij[0] = end;
}

void ft_split_helper0(char *str, int *ij, char **tokens)
{
	int token_start = ij[0];

	if ((str[ij[0]] == '<' && str[ij[0] + 1] != '\0' && str[ij[0] + 1] == '<') ||
		(str[ij[0]] == '>' && str[ij[0] + 1] != '\0' && str[ij[0] + 1] == '>'))
	{
		ij[0] += 2;
		tokens[ij[1]++] = copy_token(str, token_start, ij[0]);
	}
	else if (check_delimiter((char[]){str[ij[0]], '\0'}))
	{
		ij[0]++;
		tokens[ij[1]++] = copy_token(str, token_start, ij[0]);
	}
	else
		ft_split_helper1(str, token_start, ij, tokens);
}
char **ft_split(char *str)
{
	char **tokens;
	int ij[2];

	if (count_tokens(str) == -1)
		return NULL;
	tokens = (char **)malloc(sizeof(char *) * (count_tokens(str) + 1));
	if (!tokens)
		return NULL;
	ij[0] = 0;
	ij[1] = 0;
	while (str[ij[0]] != '\0')
	{
		while (str[ij[0]] != '\0' && is_space(str[ij[0]]))
			ij[0]++;
		if (str[ij[0]] != '\0')
			ft_split_helper0(str, ij, tokens);
	}
	tokens[ij[1]] = NULL;
	return tokens;
}
