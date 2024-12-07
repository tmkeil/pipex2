/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 23:33:10 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/07 01:39:29 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ft_execute(int std_out, char *argv, char **envp)
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
