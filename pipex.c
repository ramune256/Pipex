/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 19:27:03 by shunwata          #+#    #+#             */
/*   Updated: 2025/07/14 20:26:06 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

void	error_exit(const char *message)
{
	perror(message);
	exit(1);
}

void	close_fd(int fd1, int fd2, int fd3)
{
	if (fd1 != NULL)
		close(fd1);
	if (fd2 != NULL)
		close(fd2);
	if (fd3 != NULL)
		close(fd3);
}

void	free_2d_array(char **array)
{
	size_t	i;

	if (!array)
		return;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

char	*find_envp_path(char **envp)
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

char	*join_path(char *bin_dir, char *cmd_name)
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

char	*get_fullpath(char *cmd_name, char **envp)
{
	char	**bin_dir;
	char	*envp_path;
	char	*fullpath;
	size_t	i;

	envp_path = find_envp_path(envp);
	if (!envp_path)
		return (NULL);
	bin_dir = ft_split(envp_path, ':');
	if (!bin_dir)
		return (NULL);
	i = 0;
	while (bin_dir[i])
	{
		fullpath = join_path(bin_dir[i], cmd_name);
		if (!fullpath)
			return (free_2d_array(bin_dir), NULL);
		if (access(fullpath, X_OK) == 0)
			return (free_2d_array(bin_dir), fullpath);
		free(fullpath);
		i++;
	}
	return (free_2d_array(bin_dir), NULL);
}

void	execute_first_command(char *infile, char *cmd1, char **envp, int *pipe_fd)
{
	int		infile_fd;
	char	**cmd_args;
	char	*cmd_fullpath;

	infile_fd = open(infile, O_RDONLY);
	if (infile_fd == -1)
		error_exit("open infile");
	if (dup2(infile_fd, STDIN_FILENO) == -1)
		error_exit("dup2 stdin");
	if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
		error_exit("dup2 stdout");
	close_fd(infile_fd, pipe_fd[0], pipe_fd[1]);
	cmd_args = ft_split(cmd1, ' ');
	if (!cmd_args)
		error_exit("ft_split");
	cmd_fullpath = get_fullpath(cmd_args[0], envp);
	if (!cmd_fullpath)
		error_exit("couldn't get path");
	execve(cmd_fullpath, cmd_args, envp);
	free(cmd_fullpath);
	free_2d_array(cmd_args);
	error_exit("execve cmd1");
}

void	execute_second_command(char *outfile, char *cmd2, char **envp, int *pipe_fd)
{
	int		outfile_fd;
	char	**cmd_args;
	char	*cmd_fullpath;

	outfile_fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (outfile_fd == -1)
		error_exit("open outfile");
	if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
		error_exit("dup2 stdin");
	if (dup2(outfile_fd, STDOUT_FILENO) == -1)
		error_exit("dup2 stdout");
	close_fd(outfile_fd, pipe_fd[0], pipe_fd[1]);
	cmd_args = ft_split(cmd2, ' ');
	if (!cmd_args)
		error_exit("ft_split");
	cmd_fullpath = get_fullpath(cmd_args[0], envp);
	if (!cmd_fullpath)
		error_exit("couldn't get path");
	execve(cmd_fullpath, cmd_args, envp);
	free(cmd_fullpath);
	free_2d_array(cmd_args);
	error_exit("execve cmd2");
}

int	main(int argc, char **argv, char **envp)
{
	int		pipe_fd[2];
	pid_t	pid1;
	pid_t	pid2;
	int		status;

	if (argc != 5)
		return (write(2, "Usage: ./pipex infile cmd1 cmd2 outfile\n", 41), 1);
	if (pipe(pipe_fd) == -1)
		error_exit("pipe");
	pid1 = fork();
	if (pid1 == -1)
		error_exit("fork");
	if (pid1 == 0)
		execute_first_command(argv[1], argv[2], envp, pipe_fd);
	pid2 = fork();
	if (pid2 == -1)
		error_exit("fork");
	if (pid2 == 0)
		execute_second_command(argv[4], argv[3], envp, pipe_fd);
	close_fd(pipe_fd[0], pipe_fd[1], NULL);
	waitpid(pid1, &status, 0);
	waitpid(pid2, &status, 0);
	return (0);
}
