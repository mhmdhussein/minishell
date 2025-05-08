/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohhusse <mohhusse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 12:09:25 by fel-ghaz          #+#    #+#             */
/*   Updated: 2025/03/17 13:17:49 by mohhusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_pwd(t_shell *shell)
{
	char	*cwd;

	cwd = envget(shell->env, "PWD");
	if (!cwd)
		cwd = shell->current_pwd;
	if (cwd)
		printf("%s\n", cwd);
	else
		printf("pwd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory\n");
}
