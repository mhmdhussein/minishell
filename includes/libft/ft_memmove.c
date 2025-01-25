/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohhusse <mohhusse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 16:06:26 by mohhusse          #+#    #+#             */
/*   Updated: 2024/06/18 14:10:19 by mohhusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	overlap1(unsigned char *d, const unsigned char *s, size_t n)
{
	while (n > 0)
	{
		n--;
		*(d + n) = *(s + n);
	}
}

static void	overlap2(unsigned char *d, const unsigned char *s, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		*(d + i) = *(s + i);
		i++;
	}	
}

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	const unsigned char	*s;
	unsigned char		*d;

	s = (const unsigned char *) src;
	d = (unsigned char *) dest;
	if (d == s || !n)
		return (dest);
	if (d > s && d < s + n)
	{
		overlap1(d, s, n);
	}
	else
	{
		overlap2(d, s, n);
	}
	return (dest);
}
