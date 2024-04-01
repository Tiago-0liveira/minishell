/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 02:29:23 by joaoribe          #+#    #+#             */
/*   Updated: 2024/04/01 12:55:48 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	execution(t_command *cmd, char **ev)
{
	if (execve(cmd->cmd_name, cmd->args, ev) < 0)
	{
		error_msg_ret(CANNOT_EXECUTE_FILE, cmd->cmd_name, CMD_NOT_FOUND_RET);
		return (false);
	}
	return (true);
}

void	set_execution(t_mini *mini, t_command *cmd, char **ev, int has_next)
{
	if (if_builtin(cmd->cmd_name))
		execute_in_parent(mini, cmd, has_next);
	else
		execute_in_child(cmd, ev, has_next);
	if (cmd->fds[1] != -1)
	{
		close(cmd->fds[1]);
		cmd->fds[1] = -1;
	}
}
