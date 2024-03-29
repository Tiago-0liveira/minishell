/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 02:29:23 by joaoribe          #+#    #+#             */
/*   Updated: 2024/03/29 03:04:58 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	execution(t_command *cmd, char **ev)
{
	if (execve(cmd->cmd_name, cmd->args, ev) < 0)
	{
		cmd->failed = true;
		error_msg_ret(CMD_NOT_FOUND, cmd->cmd_name, CMD_NOT_FOUND_RET);
		return (false);
	}
	return (true);
}

void	set_execution(t_mini *mini, t_command *cmd, char **ev, bool last_failed)
{
	int			i;
	int			j;
	t_command	*lst;

	lst = ft_lstlast_mini(mini->commands);
	j = mini->input.pipe_c;
	i = if_builtin(cmd->cmd_name);
	if (j && (if_builtin_epe(cmd->cmd_name))
		&& cmd == lst && !cmd->redirs)
		bin_epe(mini, cmd);
	else if (j && i && cmd == lst && !cmd->redirs)
		return ;
	else if (j || !j || cmd == lst || cmd != lst)
	{
		if (i)
			execute_in_parent(mini, cmd, j);
		else if (!i)
			execute_in_child(cmd, ev, last_failed);
		if (cmd->next)
		{
			close(mini->input.pip[1]);
			mini->input.cmd_input = mini->input.pip[0];
		}
	}
}
