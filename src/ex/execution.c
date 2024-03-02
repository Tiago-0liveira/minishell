/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 22:27:57 by joaoribe          #+#    #+#             */
/*   Updated: 2024/03/02 00:56:12 by joaoribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_execution(t_mini *mini, char **ev)
{
	t_command	*cmd;
	t_command	*prev_cmd;

	cmd = mini->commands;
	prev_cmd = NULL;
	while (cmd)
	{
		prepare_cmd(cmd, prev_cmd, cmd->next);
		if (!expand_command(cmd, ev))
		{
			cmd = cmd->next;
			continue ;
		}
		if (if_builtin(cmd->cmd_name) && (!cmd->next))
			execute_in_parent(cmd, prev_cmd);
		else if (!if_builtin(cmd->cmd_name))
			execute_in_child(cmd, prev_cmd, ev);
		prev_cmd = cmd;
		cmd = prev_cmd->next;
	}
	wait_for_children(mini);
}

void	wait_for_children(t_mini *mini)
{
	t_command	*cmd;

	cmd = mini->commands;
	while (cmd)
	{
		if (!cmd->exec_parent)
		{
			waitpid(cmd->pid, &cmd->status, 0);
			if (WIFEXITED(cmd->status))
				mini->command_ret = WEXITSTATUS(cmd->status);
		}
		cmd = cmd->next;
	}
}

void	execute_in_parent(t_command *cmd, t_command *prev)
{
	handle_redirections(cmd);
	built_in(mini(), cmd);
	if (prev && !cmd->next)
		close(prev->pip[0]);
}

void	execute_in_child(t_command *cmd, t_command *prev, char **ev)
{
	pid_t	pid;

	(void)prev;
	pid = fork();
	if (pid < 0)
		free_shell(FORK_ERROR, EXIT_FAILURE, NULL, NULL);
	if (pid == 0)
	{
		cmd->pid = pid;
		prepare_cmd_for_child(cmd, prev);
		handle_redirections(cmd);
		if (!execution(cmd, ev))
			free_shell(NULL, 1, NULL, NULL);
		free_shell(NULL, EXIT_SUCCESS, NULL, NULL);
	}
	else if (pid)
	{
		cmd->pid = pid;
		if (cmd->std.in != STDIN_FILENO)
			close(cmd->std.in);
		if (cmd->std.out != STDOUT_FILENO)
			close(cmd->std.out);
		if (cmd->next)
			close(cmd->pip[1]);
	}
}
