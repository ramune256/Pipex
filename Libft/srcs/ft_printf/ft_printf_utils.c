/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 20:52:23 by shunwata          #+#    #+#             */
/*   Updated: 2025/06/13 15:02:07 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putchar(char c)
{
	int	result;

	result = write(1, &c, 1);
	if (result == -1)
		return (-1);
	return (result);
}

int	ft_putstr(char *str)
{
	int	count;
	int	result;

	count = 0;
	if (!str)
		return (ft_putstr("(null)"));
	while (str[count])
	{
		result = ft_putchar(str[count]);
		if (result == -1)
			return (-1);
		count++;
	}
	return (count);
}

int	ft_putnbr_base(unsigned long n, int base, int uppercase)
{
	char	*digits;
	int		count;
	int		result;

	if (base < 2 || base > 16)
		return (-1);
	if (uppercase)
		digits = "0123456789ABCDEF";
	else
		digits = "0123456789abcdef";
	count = 0;
	if (n >= (unsigned long)base)
	{
		result = ft_putnbr_base(n / base, base, uppercase);
		if (result == -1)
			return (-1);
		count += result;
	}
	result = ft_putchar(digits[n % base]);
	if (result == -1)
		return (-1);
	return (count + result);
}

int	ft_putnbr(int n)
{
	int	count;
	int	result;

	count = 0;
	if (n == -2147483648)
	{
		result = ft_putstr("-2147483648");
		if (result == -1)
			return (-1);
		return (result);
	}
	if (n < 0)
	{
		result = ft_putchar('-');
		if (result == -1)
			return (-1);
		count += result;
		n = -n;
	}
	result = ft_putnbr_base(n, 10, 0);
	if (result == -1)
		return (-1);
	return (count + result);
}

int	put_pointer(void *ptr)
{
	int	count;
	int	result;

	count = 0;
	if (ptr == NULL)
		return (ft_putstr("(nil)"));
	result = ft_putstr("0x");
	if (result == -1)
		return (-1);
	count += result;
	result = ft_putnbr_base((unsigned long)ptr, 16, 0);
	if (result == -1)
		return (-1);
	count += result;
	return (count);
}
