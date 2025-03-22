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

void	ft_pwd()
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd)
		printf("%s\n", cwd);
	else
		perror("getcwd() error");
}
