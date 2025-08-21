/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 20:52:58 by shunwata          #+#    #+#             */
/*   Updated: 2025/08/18 15:49:35 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	process_format(va_list args, char format)
{
	int		result;

	if (format == 'c')
		result = ft_putchar(va_arg(args, int));
	else if (format == 's')
		result = ft_putstr(va_arg(args, char *));
	else if (format == 'p')
		result = put_pointer(va_arg(args, void *));
	else if (format == 'd' || format == 'i')
		result = ft_putnbr(va_arg(args, int));
	else if (format == 'u')
		result = ft_putnbr_base(va_arg(args, unsigned int), 10, 0);
	else if (format == 'x')
		result = ft_putnbr_base(va_arg(args, unsigned int), 16, 0);
	else if (format == 'X')
		result = ft_putnbr_base(va_arg(args, unsigned int), 16, 1);
	else if (format == '%')
		result = ft_putchar('%');
	else
		return (-1);
	return (result);
}

int	ft_printf(const char *format, ...)
{
	va_list	args;
	int		count;
	int		result;

	count = 0;
	if (!format)
		return (-1);
	va_start(args, format);
	while (*format)
	{
		if (*format == '%' && *(format + 1))
			result = process_format(args, *(++format));
		else
			result = ft_putchar(*format);
		if (result == -1)
		{
			va_end(args);
			return (-1);
		}
		count += result;
		format++;
	}
	va_end(args);
	return (count);
}
