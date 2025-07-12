/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 19:27:03 by shunwata          #+#    #+#             */
/*   Updated: 2025/07/12 18:09:59 by shunwata         ###   ########.fr       */
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

char **split_command(char *cmd)
{
    char **args = malloc(sizeof(char *) * 64);
    char *token;
    int i = 0;

    if (!args)
        error_exit("malloc");
    token = strtok(cmd, " ");
    while (token != NULL && i < 63)
    {
        args[i] = token;
        token = strtok(NULL, " ");
        i++;
    }
    args[i] = NULL;
    return args;
}

void execute_first_command(char *infile, char *cmd1, int *pipe_fd)
{
    int infile_fd;
    char **args;

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
    args = split_command(cmd1);
    if (execve(args[0], args, NULL) == -1)
    {
        char path[256];
        sprintf(path, "/bin/%s", args[0]);
        execve(path, args, NULL);
        sprintf(path, "/usr/bin/%s", args[0]);
        execve(path, args, NULL);
        error_exit("execve cmd1");
    }
}

void execute_second_command(char *outfile, char *cmd2, int *pipe_fd)
{
    int outfile_fd;
    char **args;

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
    args = split_command(cmd2);
    if (execve(args[0], args, NULL) == -1)
    {
        char path[256];
        sprintf(path, "/bin/%s", args[0]);
        execve(path, args, NULL);
        sprintf(path, "/usr/bin/%s", args[0]);
        execve(path, args, NULL);
        error_exit("execve cmd2");
    }
}

int main(int argc, char **argv)
{
    int pipe_fd[2];
    pid_t pid1, pid2;
    int status;

    if (argc != 5)
    {
        fprintf(stderr, "Usage: %s infile cmd1 cmd2 outfile\n", argv[0]);
        exit(1);
    }
    if (pipe(pipe_fd) == -1)
        error_exit("pipe");
    pid1 = fork();
    if (pid1 == -1)
        error_exit("fork");
    if (pid1 == 0)
        execute_first_command(argv[1], argv[2], pipe_fd);
    pid2 = fork();
    if (pid2 == -1)
        error_exit("fork");
    if (pid2 == 0)
        execute_second_command(argv[4], argv[3], pipe_fd);
    close(pipe_fd[0]);
    close(pipe_fd[1]);
    waitpid(pid1, &status, 0);
    waitpid(pid2, &status, 0);
    return (0);
}
