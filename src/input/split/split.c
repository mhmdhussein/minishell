/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohhusse <mohhusse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 14:41:54 by mohhusse          #+#    #+#             */
/*   Updated: 2025/02/11 14:15:23 by mohhusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	count_tokens(char *str)
{
	int		i;
	int		count;

	i = 0;
	count = 0;
	if (input_has_unmatched_quotes(str))
	{
		printf("Error: Input has unmatched quotes\n");
		return (-1);
	}
	while (str[i] != '\0')
	{
		while (str[i] != '\0' && ft_isspace(str[i]))
			i++;
		if (str[i] != '\0')
		{
			process_and_count_token(str, &i, NULL);
			count++;
		}
	}
	return (count);
}

void	process_delimiters(char *str, int *ij, char **tokens)
{
	int		token_start;

	token_start = ij[0];
	if ((str[ij[0]] == '<' && str[ij[0] + 1] != '\0' && str[ij[0] + 1] == '<')
		|| (str[ij[0]] == '>' && str[ij[0] + 1]
			!= '\0' && str[ij[0] + 1] == '>'))
	{
		ij[0] += 2;
		tokens[ij[1]++] = copy_token(str, token_start, ij[0]);
	}
	else if (check_delimiter((char []){str[ij[0]], '\0'}))
	{
		ij[0]++;
		tokens[ij[1]++] = copy_token(str, token_start, ij[0]);
	}
	else
		process_token(str, token_start, ij, tokens);
}

char	**split_input(char *str)
{
	char	**tokens;
	int		ij[2];

	if (count_tokens(str) == -1)
		return (NULL);
	tokens = (char **)malloc(sizeof(char *) * (count_tokens(str) + 1));
	if (!tokens)
		return (NULL);
	ij[0] = 0;
	ij[1] = 0;
	while (str[ij[0]] != '\0')
	{
		while (str[ij[0]] != '\0' && ft_isspace(str[ij[0]]))
			ij[0]++;
		if (str[ij[0]] != '\0')
			process_delimiters(str, ij, tokens);
	}
	tokens[ij[1]] = NULL;
	return (tokens);
}
