/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 19:27:03 by shunwata          #+#    #+#             */
/*   Updated: 2025/08/23 01:59:44 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	exec_1st_command(char *infile, char *cmd1, char **envp, int *pipe_fd)
{
	int		infile_fd;
	char	**cmd_args;
	char	*cmd_fullpath;
	int		quote_err;

	infile_fd = open(infile, O_RDONLY);
	if (infile_fd == -1)
		error_exit("open infile");
	if (dup2(infile_fd, STDIN_FILENO) == -1)
		error_exit("dup2 stdin");
	if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
		error_exit("dup2 stdout");
	close_fd(infile_fd, pipe_fd[0], pipe_fd[1]);
	cmd_args = split_improved(cmd1, &quote_err);
	if (quote_err)
		syntax_error();
	if (!cmd_args)
		malloc_failed(NULL);
	cmd_fullpath = get_fullpath(cmd_args, envp);
	if (!cmd_fullpath)
		command_not_found(cmd_args);
	execve(cmd_fullpath, cmd_args, envp);
	free(cmd_fullpath);
	free_2d_array(cmd_args);
	error_exit("execve cmd1");
}

void	exec_2nd_command(char *outfile, char *cmd2, char **envp, int *pipe_fd)
{
	int		outfile_fd;
	char	**cmd_args;
	char	*cmd_fullpath;
	int		quote_err;

	outfile_fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (outfile_fd == -1)
		error_exit("open outfile");
	if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
		error_exit("dup2 stdin");
	if (dup2(outfile_fd, STDOUT_FILENO) == -1)
		error_exit("dup2 stdout");
	close_fd(outfile_fd, pipe_fd[0], pipe_fd[1]);
	cmd_args = split_improved(cmd2, &quote_err);
	if (quote_err)
		syntax_error();
	if (!cmd_args)
		malloc_failed(NULL);
	cmd_fullpath = get_fullpath(cmd_args, envp);
	if (!cmd_fullpath)
		command_not_found(cmd_args);
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
	int		status2;

	if (argc != 5)
		return (ft_putendl_fd("Usage: ./pipex infile cmd1 cmd2 outfile", 2), 1);
	if (pipe(pipe_fd) == -1)
		error_exit("pipe");
	pid1 = fork();
	if (pid1 == -1)
		error_exit("fork");
	if (pid1 == 0)
		exec_1st_command(argv[1], argv[2], envp, pipe_fd);
	pid2 = fork();
	if (pid2 == -1)
		error_exit("fork");
	if (pid2 == 0)
		exec_2nd_command(argv[4], argv[3], envp, pipe_fd);
	close_fd(pipe_fd[0], pipe_fd[1], -1);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, &status2, 0);
	if (WIFEXITED(status2))
		return (WEXITSTATUS(status2));
	return (1);
}
