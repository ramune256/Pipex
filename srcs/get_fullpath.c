/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_fullpath.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 13:55:58 by shunwata          #+#    #+#             */
/*   Updated: 2025/08/22 14:01:31 by shunwata         ###   ########.fr       */
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

char	*get_fullpath(char **cmd_args, char **envp)
{
	char	**bin_dir;
	char	*fullpath;
	size_t	i;
	int		perm;

	if (ft_strchr(cmd_args[0], '/'))
		return (ft_strdup(cmd_args[0]));
	bin_dir = ft_split(find_envp_path(envp), ':');
	if (!bin_dir)
		return (NULL);
	i = 0;
	perm = 0;
	while (bin_dir[i])
	{
		fullpath = join_path(bin_dir[i], cmd_args[0]);
		if (!fullpath)
			return (free_2d_array(bin_dir), NULL);
		if (access(fullpath, F_OK) == 0)
		{
			if (access(fullpath, X_OK) == 0)
				return (free_2d_array(bin_dir), fullpath);
			perm = 1;
		}
		free(fullpath);
		i++;
	}
	if (perm)
		permission_denied(cmd_args);
	return (free_2d_array(bin_dir), NULL);
}
