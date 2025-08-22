/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 18:32:49 by shunwata          #+#    #+#             */
/*   Updated: 2025/08/22 18:54:36 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <sys/wait.h>
# include <fcntl.h>
# include "libft.h"

char	*get_fullpath(char **cmd_args, char **envp);
void	error_exit(const char *message);
void	close_fd(int fd1, int fd2, int fd3);
void	free_2d_array(char **array);
void	malloc_failed(void);
void	command_not_found(char **cmd_args);
void	permission_denied(char **cmd_args);
void	no_such_file_or_directory(char **cmd_args);
void	is_a_directory(char **cmd_args);
char	**split_improved(const char *s);

#endif
