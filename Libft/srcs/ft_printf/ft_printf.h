/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 20:46:07 by shunwata          #+#    #+#             */
/*   Updated: 2025/07/19 13:56:07 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <unistd.h>
# include <stdarg.h>

int	ft_putchar(char c);
int	ft_putstr(char *str);
int	ft_putnbr_base(unsigned long n, int base, int uppercase);
int	ft_putnbr(int n);
int	put_pointer(void *ptr);

int	process_format(va_list args, char format);
int	ft_printf(const char *format, ...);

#endif
