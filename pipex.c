/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 19:27:03 by shunwata          #+#    #+#             */
/*   Updated: 2025/07/13 13:22:38 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

void error_exit(const char *message)
{
    perror(message);
    exit(1);
}

char **ft_strjoin_wraper(char *s1, char *s2)
{
    char    **result;

    result = ft_strjoin(*s1, *s2);
    if (!result)
        error_exit("ft_strjoin");
    return (result);
}

char *find_fullpath(char *cmd_name, char **envp)
{
    char    **bin_path;
    char    *fullpath;
    char    *tmp;

    if (!envp)
        exit(write(2, "NULL envp\n", 10));
    while (*envp && ft_strncmp(*envp, "PATH=", 5) != 0)
        envp++;
    if (!*envp)
        exit(write(2, "path not found\n", 15));
    bin_path = ft_split(*envp + 5, ':');
    if (!bin_path)
        error_exit("ft_split");
    while (*bin_path)
    {
        tmp = ft_strjoin_wraper(*bin_path, "/");
        fullpath = ft_strjoin_wraper(tmp, cmd_name);
        free(tmp);
        if (access(fullpath, X_OK) == 0)
            return (fullpath);
        free(fullpath);
        bin_path++;
    }
    exit(write(2, "command not found\n", 18));
}

void execute_first_command(char *infile, char *cmd1, char **envp, int *pipe_fd)
{
    int     infile_fd;
    char    **cmd_args;
    char    *cmd_fullpath;

    infile_fd = open(infile, O_RDONLY);
    if (infile_fd == -1)
        error_exit("open infile");
    if (dup2(infile_fd, STDIN_FILENO) == -1)
        error_exit("dup2 stdin");
    if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
        error_exit("dup2 stdout");
    close(infile_fd);
    close(pipe_fd[0]);
    close(pipe_fd[1]);
    cmd_args = ft_split(cmd1, ' ');
    if (!cmd_args)
        error_exit("ft_split");
    cmd_fullpath = find_fullpath(cmd_args[0], envp);
    execve(cmd_fullpath, cmd_args, envp);
    error_exit("execve cmd1");
}

void execute_second_command(char *outfile, char *cmd2, char **envp, int *pipe_fd)
{
    int outfile_fd;
    char    **cmd_args;
    char    *cmd_fullpath;

    outfile_fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (outfile_fd == -1)
        error_exit("open outfile");
    if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
        error_exit("dup2 stdin");
    if (dup2(outfile_fd, STDOUT_FILENO) == -1)
        error_exit("dup2 stdout");
    close(outfile_fd);
    close(pipe_fd[0]);
    close(pipe_fd[1]);
    cmd_args = ft_split(cmd2, ' ');
    if (!cmd_args)
        error_exit("ft_split");
    cmd_fullpath = find_fullpath(cmd_args[0], envp);
    execve(cmd_fullpath, cmd_args, envp);
    error_exit("execve cmd2");
}

int main(int argc, char **argv, char **envp)
{
    int pipe_fd[2];
    pid_t pid1, pid2;
    int status;

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
    close(pipe_fd[0]);
    close(pipe_fd[1]);
    waitpid(pid1, &status, 0);
    waitpid(pid2, &status, 0);
    return (0);
}
