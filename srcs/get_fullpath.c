/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_fullpath.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 13:55:58 by shunwata          #+#    #+#             */
/*   Updated: 2025/08/23 01:55:53 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static char	*find_envp_path(char **envp)
{
	size_t	i;

	if (!envp)
		return (NULL);
	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (!envp[i])
		return (NULL);
	return (envp[i] + 5);
}

static char	*join_path(char *bin_dir, char *cmd_name)
{
	size_t	total_len;
	char	*fullpath;

	total_len = ft_strlen(bin_dir) + ft_strlen(cmd_name) + 1;
	fullpath = malloc(sizeof(char) * (total_len + 1));
	if (!fullpath)
		return (NULL);
	ft_strlcpy(fullpath, bin_dir, total_len + 1);
	ft_strlcat(fullpath, "/", total_len + 1);
	ft_strlcat(fullpath, cmd_name, total_len + 1);
	return (fullpath);
}

static char	*check_path_and_perm(char **bin_dir, char **cmd_args, int *perm_err)
{
	char	*fullpath;
	size_t	i;

	i = 0;
	*perm_err = 0;
	while (bin_dir[i])
	{
		fullpath = join_path(bin_dir[i], cmd_args[0]);
		if (!fullpath)
		{
			free_2d_array(bin_dir);
			malloc_failed(cmd_args);
		}
		if (access(fullpath, F_OK) == 0)
		{
			if (access(fullpath, X_OK) == 0)
				return (fullpath);
			*perm_err = 1;
		}
		free(fullpath);
		i++;
	}
	return (NULL);
}

char	*get_fullpath(char **cmd_args, char **envp)
{
	char	**bin_dir;
	char	*fullpath;
	char	*envp_path;
	int		perm_err;

	if (ft_strchr(cmd_args[0], '/'))
	{
		if (access(cmd_args[0], F_OK) != 0)
			no_such_file_or_directory(cmd_args);
		if (access(cmd_args[0], X_OK) != 0)
			permission_denied(cmd_args, NULL);
		return (ft_strdup(cmd_args[0]));
	}
	envp_path = find_envp_path(envp);
	if (!envp_path)
		return (NULL);
	bin_dir = ft_split(envp_path, ':');
	if (!bin_dir)
		malloc_failed(cmd_args);
	fullpath = check_path_and_perm(bin_dir, cmd_args, &perm_err);
	if (fullpath)
		return (free_2d_array(bin_dir), fullpath);
	if (perm_err)
		permission_denied(cmd_args, bin_dir);
	return (free_2d_array(bin_dir), NULL);
}
