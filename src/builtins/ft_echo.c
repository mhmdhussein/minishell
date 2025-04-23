/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fel-ghaz <fel-ghaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 15:53:42 by mohhusse          #+#    #+#             */
/*   Updated: 2025/01/21 14:31:21 by fel-ghaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_newline_option(char *str)
{
	int		i;

	i = 0;
	if (str[i++] != '-')
		return (0);
	while (str[i] && str[i] == 'n')
		i++;
	if (!str[i])
		return (1);
	return (0);
}

void	ft_echo(t_cmd *cmd)
{
	bool	newline;
	int		i;

	newline = true;
	i = 1;
	while (cmd->args[i] && is_newline_option(cmd->args[i]))
	{
		newline = false;
		i++;
	}
	while (cmd->args[i])
	{
		if (cmd->args[i][0] == '\0')
			printf(" ");
		else
			printf("%s", cmd->args[i]);
		if (cmd->args[i + 1] && cmd->args[i][0] != '\0')
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
}
