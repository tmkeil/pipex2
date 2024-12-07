/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 23:33:01 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/07 04:00:47 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*ft_getpath(char *cmd, char **envp)
{
	char	*path;
	char	*full;
	char	**env;
	char	**start;

	path = NULL;
	full = NULL;
	while (*envp && !ft_strnstr(*envp, "PATH=", 5))
		envp++;
	env = ft_split(*envp, ':');
	start = env;
	if (!env || !*env)
		return (NULL);
	while (*env)
	{
		path = ft_strjoin(*env++, "/");
		full = ft_strjoin(path, cmd);
		free(path);
        if (!full)
            return (ft_clr(start), NULL);
		if (access(full, F_OK) == 0)
			return (ft_clr(start), full);
		free(full);
	}
	return (ft_clr(start), NULL);
}

static void	ft_execute(int std_out, char *argv, char **envp)
{
	char	*path;
	char	**cmds;

	cmds = ft_split(argv, ' ');
	if (!cmds)
		ft_error(BAD_ALLOCATION, std_out);
	path = ft_getpath(cmds[0], envp);
	if (!path || execve(path, cmds, envp) == -1)
	{
		free(path);
		ft_clr(cmds);
		ft_error(BAD_UNDEFINED, std_out);
	}
}

void    ft_create_pipe(bool last, int *fd, char **argv, char **envp)
{   
    int     out;
    pid_t	pid1;
	pid_t	pid2;

	if (pipe(fd) == -1)
		ft_error(BAD_PIPE, STDOUT_FILENO);
    out = dup(STDOUT_FILENO);
    pid1 = fork();
    pid2 = fork();
	if (pid1 == -1 || pid2 == -1)
		ft_error(BAD_UNDEFINED, STDOUT_FILENO);
    if (pid1 == 0 && pid2 > 0)
    {
        // Child1 of parent (for left side of the pipe -> write end)
        dup2(fd[1], STDOUT_FILENO);
        close(fd[0]);
        close(fd[1]);
        ft_execute(out, *argv, envp);
    }
    else if (pid1 > 0 && pid2 == 0)
    {
        // Child2 of parent (for right side of the pipe -> read end)
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        close(fd[1]);
        if (last)
            ft_execute(out, *(argv + 1), envp);
    }
    close(fd[0]);
    close(fd[1]);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
}

void    ft_set_in(char *argv)
{
    int in;
    int out;

    in = open(argv, O_RDONLY);
    if (in == -1)
        ft_error(BAD_FD, STDOUT_FILENO);
    dup2(in, STDIN_FILENO);
    close(in);
}

void    ft_set_out(char *argv)
{
    int out;

    out = open(argv, O_WRONLY | O_CREAT | O_TRUNC, 0777);
    if (out == -1)
        ft_error(BAD_FD, STDOUT_FILENO);
    dup2(out, STDOUT_FILENO);
    close(out);
}

int	main(int argc, char **argv, char **envp)
{
	int		fd[2];
    int     i;

	if (argc != 5 || !*argv[1])
		return (1);
    ft_set_in(argv[1]);
    ft_set_out(argv[argc - 1]);
    i = 1;
    ft_pipe(argc, argv, envp);
    // while (++i < argc - 1)
    //     ft_create_pipe(argc == (i + 3), fd, &argv[i], envp);
	close(fd[0]);
	close(fd[1]);
	return (0);
}

// int	main(int argc, char **argv, char **envp)
// {
// 	int		fd[2];
// 	pid_t	pid1;
// 	pid_t	pid2;

// 	if (argc != 5 || !*argv[1])
// 		return (1);
// 	if (pipe(fd) == -1)
// 		ft_error(BAD_PIPE, STDOUT_FILENO);
// 	pid1 = fork();
// 	if (pid1 == -1)
// 		ft_error(BAD_UNDEFINED, STDOUT_FILENO);
// 	if (pid1 == 0)
// 		ft_child1(argv, envp, fd);
// 	pid2 = fork();
// 	if (pid2 == -1)
// 		ft_error(BAD_UNDEFINED, STDOUT_FILENO);
// 	if (pid2 == 0)
// 		ft_child2(argv, envp, fd);
// 	close(fd[0]);
// 	close(fd[1]);
// 	waitpid(pid1, NULL, 0);
// 	waitpid(pid2, NULL, 0);
// 	return (0);
// }
