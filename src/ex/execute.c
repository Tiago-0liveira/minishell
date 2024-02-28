/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 02:29:23 by joaoribe          #+#    #+#             */
/*   Updated: 2024/02/28 21:59:36 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*cmd_path(char **ev)
{
	while (ft_strncmp("PATH", *ev, 4))
		ev++;
	return (*ev + 5);
}

char	*get_path(char *cmd, char **ev)
{
	int		i;
	char	**paths;
	char	*path;
	char	*tmp;

	i = 0;
	paths = ft_split(cmd_path(ev), ':');
	while (paths[i])
	{
		path = ft_strjoin(paths[i], "/");
		tmp = path;
		path = ft_strjoin(path, cmd);
		free(tmp);
		if (!access(path, F_OK | X_OK))
		{
			free_list(paths);
			return (path);
		}
		free(path);
		i++;
	}
	free_list(paths);
	return (0);
}

bool	execution(t_command *cmd, char **ev)
{
	char	*path;

	path = get_path(cmd->cmd_name, ev);
	if (!path || execve(path, cmd->args, ev) < 0)
	{
		error_msg_ret(CMD_NOT_FOUND, cmd->cmd_name, CMD_NOT_FOUND_RET);
		if (path)
			free(path);
		return (false);
	}
	free(path);
	return (true);
}
