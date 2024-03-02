/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 22:27:57 by joaoribe          #+#    #+#             */
/*   Updated: 2024/03/02 04:06:34 by joaoribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_execution(t_mini *mini, char **ev)
{
	t_command	*cmd;
	t_command	*prev_cmd;
	char		*heredoc_fd;

	cmd = mini->commands;
	prev_cmd = NULL;
	while (cmd)
	{
		if (cmd->redirs && cmd->redirs->type == RED_AIN)
		{
			heredoc_fd = heredoc(cmd);
			cmd->doctor.fd = open(heredoc_fd, O_RDONLY);
			free(cmd->doctor.delim);
			free(heredoc_fd);
		}
		prepare_cmd(cmd, prev_cmd, cmd->next);
		if (!expand_command(cmd, ev))
		{
			cmd = cmd->next;
			continue ;
		}
		if (if_builtin(cmd->cmd_name))
			execute_in_parent(cmd);
		else
		{
			execute_in_child(cmd, prev_cmd, ev);
		}
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

void	execute_in_parent(t_command *cmd)
{	
	cmd->std.out = dup(STDOUT_FILENO);
	cmd->std.in = dup(STDIN_FILENO);
	if (cmd->next)
		dup2(cmd->pip[1], STDOUT_FILENO);
	handle_redirections(cmd);
	built_in(mini(), cmd);
	dup2(cmd->std.in, STDIN_FILENO);
	dup2(cmd->std.out, STDOUT_FILENO);
	close(cmd->std.in);
	close(cmd->std.out);
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
		handle_redirections(cmd);
		cmd->pid = pid;
		prepare_cmd_for_child(cmd, prev);
		ft_putendl_fd("1", 1);
		if (!execution(cmd, ev))
			free_shell(NULL, 1, NULL, NULL);
		free_shell(NULL, EXIT_SUCCESS, NULL, NULL);
	}
	else if (pid)
	{
		cmd->pid = pid;
		if (!cmd->next)
			cmd->std.in = STDIN_FILENO;mak
		if (cmd->std.in != STDIN_FILENO)
			close(cmd->std.in);
		if (cmd->std.out != STDOUT_FILENO)
			close(cmd->std.out);
		if (cmd->next)
			close(cmd->pip[1]);
	}
}
