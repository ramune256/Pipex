/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_error.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 13:58:57 by shunwata          #+#    #+#             */
/*   Updated: 2025/08/23 23:43:45 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	malloc_failed(char **cmd_args)
{
	ft_putendl_fd("pipex: Cannot allocate memory", 2);
	free_2d_array(cmd_args);
	exit(1);
}

void	command_not_found(char **cmd_args)
{
	ft_putstr_fd("pipex: command not found: ", 2);
	if (cmd_args[0])
		ft_putendl_fd(cmd_args[0], 2);
	else
		write(2, "aaaa", 4);
		ft_putchar_fd('\n', 2);
	free_2d_array(cmd_args);
	exit(127);
}

void	permission_denied(char **cmd_args, char **bin_dir)
{
	ft_putstr_fd("pipex: permission denied: ", 2);
	ft_putendl_fd(cmd_args[0], 2);
	free_2d_array(cmd_args);
	free_2d_array(bin_dir);
	exit(126);
}

void	no_such_file_or_directory(char **cmd_args)
{
	ft_putstr_fd("pipex: no such file or directory: ", 2);
	ft_putendl_fd(cmd_args[0], 2);
	free_2d_array(cmd_args);
	exit(127);
}

void	syntax_error(void)
{
	ft_putendl_fd("pipex: unmatched \'", 2);
	exit(1);
}
