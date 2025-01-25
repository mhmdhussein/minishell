/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expandd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fel-ghaz <fel-ghaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 10:46:27 by fel-ghaz          #+#    #+#             */
/*   Updated: 2025/01/15 14:28:16 by fel-ghaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

char *ft_strjoincustom(char *s1, char *s2)
{
	char            *join;
	unsigned int    len1;
	unsigned int    len2;

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
void check_quotes(char c, int *quote)
{
	if (c == '"')
		*quote = 2;
	if (c == '\'')
		*quote = 1;
	if ((c == '"' && *quote == 2) || (c == '\'' && *quote == 1))
		*quote = 0;
}
char *expand(t_shell *shell, char *key)
{
	t_env *current = shell->env;

	while (current != NULL)
	{
		if (strcmp(current->key, key) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}
/*1- awil shi bekhud l splitted, ye3ne m3 quotes m3 $
2- bs shouf quotes ye3ne check quotes, b alba check eza $ aw la:
2-a) eza $, b3edd kam wehde, eza odd, bektebun w betrouk wehde
w b3ml check_character w bshouf eza bl env, b3mella expansion,,,,, eza even, bektebun mtl ma henne,
b) eza msh $, bekteba mtl ma hiye*/
char **expand_variable(char **splitted, t_shell *shell)
{
	int i = 0;
	int j;
	int quote = 0;
	int dollar_count = 0;
	char *result = ft_strdup("");
	char *expanded;
	while (splitted[i])
	{
		j = 0;
		while (splitted[i][j])
		{
			check_quotes(splitted[i][j], &quote);
			if (splitted[i][j] == '$' && (quote == 2 || quote == 0))
			{
				dollar_count = 0;
				while (splitted[i][j + dollar_count] == '$')
					dollar_count++;
				if (dollar_count % 2 == 0)
					result = ft_strjoincustom(result, ft_strndup("00", dollar_count));
				else
				{
					if (check_character(splitted[i][j + dollar_count]))
					{
						result = ft_strjoincustom(result, ft_strndup("00", dollar_count - 1));
						expanded = expand(shell, &splitted[i][j + dollar_count]);
						if (expanded)
							result = ft_strjoincustom(result, expanded);
					}
					else //han ye3ne msh _ wala harf (ba3d l $ deghre), han bade 3 cases lli henne :eza number b3d l $, b2affi huwe wl $
					// eza msh quotes, w eza quotes
					{
						if (ft_isdigit(splitted[i][j]))
							j++;
						else if (splitted[i][j] != '\'' && splitted[i][j] != '"')
						{
							
						}
						result = ft_strjoincustom(result, ft_strndup("00", dollar_count));
					}
				}
				j += dollar_count;
			}
			else
			{
				char tmp[2] = {splitted[i][j], '\0'};
				result = ft_strjoincustom(result, tmp);
				j++;
			}
		}
		i++;
	}
	return (result);
}

	// 	{
	// 		check_quotes(splitted[i][j], &quote);
	// 		if (splitted[i][j] == '$' && (quote == 2 || quote == 0))
	// 		{
	// 			// i donno what to do with he case where i have: echo $$$9'HOME', it should give: 224691HOME
	// 			// count_$_signs
	// 			// if (even)
	// 			// 	write it as it is,
	// 			// else
	// 			// {
	// 			// 	han b3ayit la function btshufle b3d l $ shu fi
	// 			// 	b alba bhot 4 cases, wehde eza number, wehde eza _ aw harf, wehde eza msh quotes, wehde eza "" aw ''
	// 			// 	if (check_character(splitted[i][j+1]))
	// 			// 	{
	// 			// 		write (count_$ - 1) + expand

	// 			// 	}
	// 			// 	else
	// 			// 		write it as it is
	// 			}

	// 		}
	// 		else
	// 			// write it as it is
	// 	}
