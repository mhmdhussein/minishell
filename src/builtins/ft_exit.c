/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohhusse <mohhusse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 11:11:22 by mohhusse          #+#    #+#             */
/*   Updated: 2025/01/11 18:14:23 by mohhusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_isnum(char *str)
{
	int		i;

	i = 0;
	if (str[i] == 43 || str[i] == 45)
		i++;
	while (str[i])
	{
		if (str[i] < 48 || str[i] > 57)
			return (0);
		i++;
	}
	return (1);
}

void	ft_exit(t_cmd *cmd, t_shell *shell)
{
	int		exit_code;

	exit_code = 0;
	if (cmd->args[1])
	{
		if (!ft_isnum(cmd->args[1]))
		{
			printf("bash: exit: %s: numeric argument required", cmd->args[1]);
			exit_code = 255;
		}
		else if (cmd->args[2])
		{
			printf("bash: exit: too many arguments\n");
			return ;
		}
		else
			exit_code = ft_atoi(cmd->args[1]);
	}
	shell->running = false;
	// clean all
	exit(exit_code);
}
