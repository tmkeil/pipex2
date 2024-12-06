/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 13:40:24 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/06 21:48:15 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# define BAD_FD 9
# define BAD_PIPE 32
# define BAD_UNDEFINED 0
# define BAD_ALLOCATION 12

# include "libft.h"
# include <sys/wait.h>
# include <string.h>

void	ft_clr(char **ptr);
void	ft_error(int errno);
char	*ft_getpath(char *cmd, char **envp);

#endif
