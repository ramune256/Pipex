/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 13:57:47 by shunwata          #+#    #+#             */
/*   Updated: 2025/08/22 17:43:29 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	error_exit(const char *message)
{
	perror(message);
	exit(1);
}

void	close_fd(int fd1, int fd2, int fd3)
{
	if (fd1 != -1)
		close(fd1);
	if (fd2 != -1)
		close(fd2);
	if (fd3 != -1)
		close(fd3);
}

void	free_2d_array(char **array)
{
	size_t	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}
