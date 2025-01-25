/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohhusse <mohhusse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 12:09:25 by fel-ghaz          #+#    #+#             */
/*   Updated: 2025/01/11 18:04:51 by mohhusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void ft_pwd(char **args)
{
	int		i;

	i = 1;
	while (args[i])
	{
		if (args[i][0] == '-')
		{
			printf("bash: pwd: %s: invalid option\n", args[i]);
			return;
		}
		i++;
	}
	char cwd[1024];
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		printf("%s\n", cwd);
	else
		perror("getcwd() error");
}
