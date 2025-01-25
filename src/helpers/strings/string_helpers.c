/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fel-ghaz <fel-ghaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 12:26:00 by fel-ghaz          #+#    #+#             */
/*   Updated: 2025/01/15 13:09:22 by fel-ghaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int ft_strcmp(char *str1, char *str2)
{
	int i;

    i = 0;
    while (str1[i] && str2[i])
    {
        if (str1[i] != str2[i])
            return (str1[i] - str2[i]);
        i++;
    }
    return (str1[i] - str2[i]);
}
char *ft_strncpy(char *dest, char *src, int n)
{
    int i = 0;

    while (i < n && src[i] != '\0')
    {
        dest[i] = src[i];
        i++;
    }
    while (i < n)
    {
        dest[i] = '\0';
        i++;
    }
    return dest;
}
int check_character(char c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c == '_'))
		return 1;
	return 0;
}
void append_char(char **result, char c)
{
    char temp[2];

    temp[0] = c;
    temp[1] = '\0';
    *result = ft_strjoin(*result, temp);
}
char *ft_strndup(char *s, size_t n)
{
    char *dup = (char *)malloc(n + 1);
    if (!dup)
        return NULL;
    ft_memcpy(dup, s, n);
    dup[n] = '\0';
    return dup;
}
