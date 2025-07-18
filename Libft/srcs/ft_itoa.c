/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 19:21:09 by shunwata          #+#    #+#             */
/*   Updated: 2025/06/04 20:49:04 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	get_n_len(int n)
{
	size_t		len;
	long long	nl;

	if (n == 0)
		return (1);
	len = 0;
	nl = (long long)n;
	if (nl < 0)
	{
		len++;
		nl = nl * -1;
	}
	while (nl != 0)
	{
		nl = nl / 10;
		len++;
	}
	return (len);
}

static void	parse_number(long long nl, char *str, size_t *i)
{
	if (nl > 9)
		parse_number(nl / 10, str, i);
	str[*i] = (nl % 10) + '0';
	(*i)++;
}

char	*ft_itoa(int n)
{
	char		*str;
	size_t		i;
	long long	nl;

	str = (char *)malloc(get_n_len(n) + 1);
	if (!str)
		return (NULL);
	i = 0;
	nl = n;
	if (nl < 0)
	{
		str[i] = '-';
		i++;
		nl = nl * -1;
	}
	parse_number(nl, str, &i);
	str[i] = '\0';
	return (str);
}
