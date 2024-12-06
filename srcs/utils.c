/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 13:43:03 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/06 22:45:17 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ft_error(int errno, int fd)
{
	ft_putendl_fd(strerror(errno), fd);
	exit(errno);
}

void	ft_clr(char **ptr)
{
	int	i;

	if (!ptr || !*ptr)
		return ;
	i = 0;
	while (ptr[i])
		free(ptr[i++]);
	free(ptr);
	ptr = NULL;
}

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
