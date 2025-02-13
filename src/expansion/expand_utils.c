/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohhusse <mohhusse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 14:59:25 by mohhusse          #+#    #+#             */
/*   Updated: 2025/02/11 14:32:54 by mohhusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
	char	char_str[2];
	char	*temp;

	if (!str)
		return (NULL);
	char_str[0] = c;
	char_str[1] = '\0';
	temp = str;
	str = ft_strjoin(str, char_str);
	free(temp);
	return (str);
}
