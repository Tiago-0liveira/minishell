/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 02:29:23 by joaoribe          #+#    #+#             */
/*   Updated: 2024/02/17 00:16:12 by joaoribe         ###   ########.fr       */
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

	i = 0;
	paths = ft_split(cmd_path(ev), ':');
	while (paths[i])
	{
		path = ft_strjoin(paths[i], "/");
		path = ft_strjoin(path, cmd);
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

void	execution(t_mini *mini, t_command *cmd, char **ev)
{
	char	*path;

	if (if_builtin(cmd->cmd_name))
		built_in(mini, cmd);
	else
	{
		path = get_path(cmd->cmd_name, ev);
		if (execve(path, cmd->args, ev) < 0)
		{
			free_commands(cmd);
			return ;
		}
		free(path);
	}
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	free(mini->input.raw_line);
}
