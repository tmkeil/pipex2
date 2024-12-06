/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 13:39:57 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/06 22:45:14 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ft_child1(char **argv, char **envp, int *fd)
{
	int		in;
	int		tmp_out;
	char	*path;
	char	**cmds;

	path = NULL;
	tmp_out = dup(STDOUT_FILENO);
	in = open(argv[1], O_RDONLY);
	if (in == -1)
		ft_error(BAD_FD, tmp_out);
	if (dup2(in, STDIN_FILENO) == -1)
        ft_error(BAD_FD, STDOUT_FILENO);
	if (dup2(fd[1], STDOUT_FILENO) == -1)
        ft_error(BAD_FD, STDOUT_FILENO);
	close(in);
	close(fd[0]);
	close(fd[1]);
	cmds = ft_split(argv[2], ' ');
	if (!cmds)
		ft_error(BAD_ALLOCATION, tmp_out);
	path = ft_getpath(cmds[0], envp);
	if (!path || execve(path, cmds, envp) == -1)
	{
		free(path);
		ft_clr(cmds);
		ft_error(BAD_UNDEFINED, tmp_out);
	}
}

void	ft_child2(char **argv, char **envp, int *fd)
{
	int		out;
	int		tmp_out;
	char	*path;
	char	**cmds;

	path = NULL;
	tmp_out = dup(STDOUT_FILENO);
	out = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (out == -1)
		ft_error(BAD_FD, tmp_out);
	if (dup2(fd[0], STDIN_FILENO) == -1)
        ft_error(BAD_FD, STDOUT_FILENO);
	if (dup2(out, STDOUT_FILENO) == -1)
        ft_error(BAD_FD, STDOUT_FILENO);
	close(out);
	close(fd[0]);
	close(fd[1]);
	cmds = ft_split(argv[3], ' ');
	if (!cmds)
		ft_error(BAD_ALLOCATION, tmp_out);
	path = ft_getpath(cmds[0], envp);
	if (!path || execve(path, cmds, envp) == -1)
	{
		free(path);
		ft_clr(cmds);
		ft_error(BAD_UNDEFINED, tmp_out);
	}
}

int	main(int argc, char **argv, char **envp)
{
	int		fd[2];
	pid_t	pid1;
	pid_t	pid2;

	if (argc != 5 || !*argv[1])
		return (1);
	if (pipe(fd) == -1)
		ft_error(BAD_PIPE, STDOUT_FILENO);
	pid1 = fork();
	if (pid1 == -1)
		ft_error(BAD_UNDEFINED, STDOUT_FILENO);
	if (pid1 == 0)
		ft_child1(argv, envp, fd);
	pid2 = fork();
	if (pid2 == -1)
		ft_error(BAD_UNDEFINED, STDOUT_FILENO);
	if (pid2 == 0)
		ft_child2(argv, envp, fd);
	close(fd[0]);
	close(fd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
	return (0);
}

// int main(int argc, char **argv, char **envp)
// {
// 	(void)argc;
// 	(void)argv;
// 	char *p = ft_getpath("ls", envp);
// 	printf("%s\n", p);
// 	return (0);
// }

// example: dup, dup2, open, close
// int  main(void)
// {
//  int temp;
//  int file;
//  int test_fd;

//  temp = dup(STDOUT_FILENO);
//  file = open("t.txt", O_WRONLY | O_CREAT, 0777);
//  if (file == -1)
//      return (1);
//  printf("the fd of file is: %i\n", file);
//  dup2(file, STDOUT_FILENO);
//  close(file);
//  test_fd = dup(STDOUT_FILENO);
//  write(test_fd, "hallo\n", 6);
//  write(STDOUT_FILENO, " abc\n", 5);
//  dup2(temp, STDOUT_FILENO);
//  write(STDOUT_FILENO, " back again\n", 12);
//  return (0);
// }

// example1 fork and waitpid
// int  main(void)
// {
//  pid_t   id1;
//  pid_t   id2;

//  id1 = fork();
//  id2 = fork();
//  if (id1 == 0)
//  {
//      if (id2 == 0)
//          printf("child of child\n");
//      else
//          printf("child1 of parent\n");
//  }
//  else
//  {
//      if (id2 == 0)
//          printf("child2 of parent\n");
//      else
//          printf("parent\n");
//  }
//  while (waitpid(-1, NULL, 0) != -1)
//      printf("waiting\n");
//  return (0);
// }

// simpler pipe example
// int  main(void)
// {
//  char    child_content[100];
//  int     fd[2];
//  pid_t   id;

//  if (pipe(fd) == -1)
//      return (printf("couldn't pipe\n"), 1);
//  id = fork();
//  if (id == -1)
//      return (printf("couldn't fork\n"), 1);
//  if (id == 0)
//  {
//      close(fd[0]);
//      if (write(fd[1], "abc\n", 4) == -1)
//          return (printf("couldn't write\n"), 1);
//      close(fd[1]);
//  }
//  else
//  {
//      close(fd[1]);
//      if (read(fd[0], child_content, sizeof(child_content)) == -1)
//          return (printf("couldn't read\n"), 2);
//      printf("data vom child: %s", child_content);
//      close(fd[0]);
//  }
//  if (waitpid(-1, NULL, 0) != -1)
//      printf("waiting\n");
//  return (0);
// }

// practice pipe
// int  prep(char *in, int **ptr)
// {
//  int     i;
//  long    val;
//  char    **splitted;

//  i = 0;
//  splitted = ft_split(in, ' ');
//  if (!splitted)
//      return (0);
//  while (splitted[i])
//      i++;
//  *ptr = ft_calloc(i + 1, sizeof(int));
//  if (!*ptr)
//      return (0);
//  (*ptr)[i] = INT_MIN;
//  while (i)
//  {
//      val = ft_atol(splitted[i - 1]);
//      if (val > INT_MAX || val <= INT_MIN)
//          return (0);
//      (*ptr)[i - 1] = val;
//      i--;
//  }
//  return (1);
// }

// exercise communication processes through a pipe
// int  main(int argc, char **argv)
// {
//  pid_t   id;
//  int     i;
//  int     sum;
//  int     fd[2];
//  int     *input;
//  int     res;
//  char    *r;

//  input = NULL;
//  if (argc != 2 || !*argv[1])
//      return (0);
//  if (!prep(argv[1], &input))
//      return (printf("numbers invalid\n"), 1);
//  if (pipe(fd) == -1)
//      return (printf("couldn't pipe\n"), 1);
//  id = fork();
//  if (id == -1)
//      return (printf("couldn't fork\n"), 1);
//  if (id == 0)
//  {
//      close(fd[0]);
//      i = 0;
//      sum = 0;
//      while (input[i] > INT_MIN)
//      {
//          printf("zahl ist %i\n", input[i]);
//          sum += input[i++];
//      }
//      if (write(fd[1], &sum, sizeof(int)) == -1)
//          return (printf("couldn't write\n"), 1);
//      close(fd[1]);
//  }
//  else
//  {
//      close(fd[1]);
//      if (read(fd[0], &res, sizeof(int)) == -1)
//          return (printf("couldn't read\n"), 1);
//      r = ft_itoa(res);
//      write(1, r, ft_strlen(r));
//      write(1, "\n", 1);
//      close(fd[0]);
//  }
//  if (waitpid(-1, NULL, 0) != -1)
//      printf("waiting\n");
//  return (0);
// }

// example what it should do
// int  main(void)
// {
//  system("< in.txt ls -l | wc -l > out.txt");
//  return (0);
// }
