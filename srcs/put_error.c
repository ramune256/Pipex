/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_error.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 13:58:57 by shunwata          #+#    #+#             */
/*   Updated: 2025/08/22 18:54:05 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	malloc_failed(void)
{
	ft_putendl_fd("pipex: memory allocation failed", 2);
	exit(1);
}

void	command_not_found(char **cmd_args)
{
	ft_putstr_fd("pipex: ", 2);
	ft_putstr_fd(cmd_args[0], 2);
	ft_putendl_fd(": command not found", 2);
	free_2d_array(cmd_args);
	exit(127);
}

void	permission_denied(char **cmd_args)
{
	ft_putstr_fd("pipex: ", 2);
	ft_putstr_fd(cmd_args[0], 2);
	ft_putendl_fd(": permission denied", 2);
	free_2d_array(cmd_args);
	exit(126);
}

void	no_such_file_or_directory(char **cmd_args)
{
	ft_putstr_fd("pipex: ", 2);
	ft_putstr_fd(cmd_args[0], 2);
	ft_putendl_fd(": No such file or directory", 2);
	free_2d_array(cmd_args);
	exit(127);
}

void	is_a_directory(char **cmd_args)
{
	ft_putstr_fd("pipex: ", 2);
	ft_putstr_fd(cmd_args[0], 2);
	ft_putendl_fd(": is a directory", 2);
	free_2d_array(cmd_args);
	exit(126);
}
