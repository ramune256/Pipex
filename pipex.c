/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shunwata <shunwata@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 19:27:03 by shunwata          #+#    #+#             */
/*   Updated: 2025/07/10 19:28:58 by shunwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

// エラーメッセージを表示して終了
void error_exit(const char *message)
{
    perror(message);
    exit(1);
}

// コマンド文字列を分割してargv配列を作成
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

// 最初のコマンドを実行する子プロセス
void execute_first_command(char *infile, char *cmd1, int *pipe_fd)
{
    int infile_fd;
    char **args;

    // 入力ファイルを開く
    infile_fd = open(infile, O_RDONLY);
    if (infile_fd == -1)
        error_exit("open infile");

    // 標準入力を入力ファイルに変更
    if (dup2(infile_fd, STDIN_FILENO) == -1)
        error_exit("dup2 stdin");

    // 標準出力をパイプの書き込み側に変更
    if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
        error_exit("dup2 stdout");

    // 不要なファイルディスクリプタを閉じる
    close(infile_fd);
    close(pipe_fd[0]);  // 読み込み側は使わない
    close(pipe_fd[1]);  // dup2後は不要

    // コマンドを分割してexecveで実行
    args = split_command(cmd1);
    if (execve(args[0], args, NULL) == -1)
    {
        // PATH環境変数からコマンドを検索する簡単な方法
        char path[256];
        sprintf(path, "/bin/%s", args[0]);
        execve(path, args, NULL);
        sprintf(path, "/usr/bin/%s", args[0]);
        execve(path, args, NULL);
        error_exit("execve cmd1");
    }
}

// 2番目のコマンドを実行する子プロセス
void execute_second_command(char *outfile, char *cmd2, int *pipe_fd)
{
    int outfile_fd;
    char **args;

    // 出力ファイルを開く（作成・書き込み・切り詰め）
    outfile_fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (outfile_fd == -1)
        error_exit("open outfile");

    // 標準入力をパイプの読み込み側に変更
    if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
        error_exit("dup2 stdin");

    // 標準出力を出力ファイルに変更
    if (dup2(outfile_fd, STDOUT_FILENO) == -1)
        error_exit("dup2 stdout");

    // 不要なファイルディスクリプタを閉じる
    close(outfile_fd);
    close(pipe_fd[0]);  // dup2後は不要
    close(pipe_fd[1]);  // 書き込み側は使わない

    // コマンドを分割してexecveで実行
    args = split_command(cmd2);
    if (execve(args[0], args, NULL) == -1)
    {
        // PATH環境変数からコマンドを検索する簡単な方法
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
    int pipe_fd[2];  // パイプのファイルディスクリプタ
    pid_t pid1, pid2;  // 子プロセスのPID
    int status;

    // 引数の数をチェック
    if (argc != 5)
    {
        fprintf(stderr, "Usage: %s infile cmd1 cmd2 outfile\n", argv[0]);
        exit(1);
    }

    // パイプを作成
    if (pipe(pipe_fd) == -1)
        error_exit("pipe");

    // 最初の子プロセスを作成
    pid1 = fork();
    if (pid1 == -1)
        error_exit("fork");

    if (pid1 == 0)
    {
        // 子プロセス1: 最初のコマンドを実行
        execute_first_command(argv[1], argv[2], pipe_fd);
    }

    // 2番目の子プロセスを作成
    pid2 = fork();
    if (pid2 == -1)
        error_exit("fork");

    if (pid2 == 0)
    {
        // 子プロセス2: 2番目のコマンドを実行
        execute_second_command(argv[4], argv[3], pipe_fd);
    }

    // 親プロセス: パイプを閉じて子プロセスの完了を待つ
    close(pipe_fd[0]);
    close(pipe_fd[1]);

    // 両方の子プロセスの完了を待つ
    waitpid(pid1, &status, 0);
    waitpid(pid2, &status, 0);

    return 0;
}
