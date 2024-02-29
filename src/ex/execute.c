/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 02:29:23 by joaoribe          #+#    #+#             */
/*   Updated: 2024/02/29 04:36:15 by joaoribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*cmd_path(char **ev)
{
	while (ft_strncmp("PATH", *ev, 4))
		ev++;
	return (*ev + 5);
}

char	*delete_until_char(char *str, char c) 
{
    char	*ptr;

	if (!str)
        return (NULL);
	ptr = str + ft_strlen(str) - 1;
    while (ptr >= str && *ptr != c)
        ptr--;
    if (*ptr == c)
        ptr++;
    *ptr = '\0';
    return (str);
}

char	*get_path(char *cmd, char **ev)
{
	int		i;
	char	**paths;
	char	*path;
	char	*tmp;
	char	*p;

	i = 0;
	paths = ft_split(cmd_path(ev), ':');
	while (paths[i])
	{
		path = ft_strjoin(paths[i], "/");
		tmp = path;
		if (ft_strnstr(cmd, "/", ft_strlen(path)))
		{
			mini()->relative_path = 1;
			if (!access(cmd, F_OK))
			{
				p = ft_strdup(cmd);
				p = delete_until_char(p, '/');
			}
			if (ft_strnstr(path, p, ft_strlen(path)))
			{
				free(p);
				free(tmp);
				free_list(paths);
				return (cmd);
			}
		}
		else
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
	if (path)
		free(path);
	free_list(paths);
	return (0);
}

bool	execution(t_command *cmd, char **ev, char *path)
{
	if (!path || execve(path, cmd->args, ev) < 0)
	{
		error_msg_ret(CMD_NOT_FOUND, cmd->cmd_name, CMD_NOT_FOUND_RET);
		return (false);
	}
	return (true);
}
