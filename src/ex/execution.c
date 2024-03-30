/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 22:27:57 by joaoribe          #+#    #+#             */
/*   Updated: 2024/03/30 14:38:29 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_execution(t_mini *mini, char **ev)
{
	t_command	*cmd;
	int			has_next;

	cmd = mini->commands;
	while (cmd)
	{
		has_next = (cmd->next != NULL);
		if (pipe(cmd->fds) == -1)
			free_shell(PIPE_ERROR, EXIT_FAILURE, NULL, NULL);
		if (!build_command(cmd) || (cmd->args && !expand_command(cmd)))
		{
			handle_command_fail(cmd);
			cmd = cmd->next;
			continue ;
		}
		mini->if_cd = 0;
		set_execution(mini, cmd, ev, has_next);
		cmd = cmd->next;
	}
	wait_for_children(mini);
}

void	wait_for_children(t_mini *mini)
{
	int			status;

	while (1)
	{
		waitpid(0, &status, 0);
		if (errno == ECHILD)
			break ;
		if (WIFEXITED(status))
			mini->command_ret = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			mini->command_ret = WTERMSIG(status);
	}
}

void	execute_in_parent(t_mini *mini, t_command *cmd, int has_next)
{
	int	original_stdout;
	int	original_stdin;

	original_stdout = dup(STDOUT_FILENO);
	original_stdin = dup(STDIN_FILENO);
	if (original_stdout == -1 || original_stdin == -1)
		free_shell(PIPE_ERROR, EXIT_FAILURE, NULL, NULL);
	if (!ft_strncmp(cmd->cmd_name, "exit", 4))
	{
		cmd->stds[0] = original_stdin;
		cmd->stds[1] = original_stdout;
	}
	if (has_next)
		dup2(cmd->fds[1], STDOUT_FILENO);
	if (setup_redirections(cmd, true))
		built_in(mini, cmd);
	dup2(original_stdin, STDIN_FILENO);
	dup2(original_stdout, STDOUT_FILENO);
	close(original_stdin);
	close(original_stdout);
}

void	pid_zero(t_command *cmd, char **ev, int has_next)
{
	if (cmd->fds[0] != -1)
	{
		close(cmd->fds[0]);
		cmd->fds[0] = -1;
	}
	if (cmd->prev && cmd->prev->fds[0] != -1)
	{
		dup2(cmd->prev->fds[0], STDIN_FILENO);
		close(cmd->prev->fds[0]);
		cmd->prev->fds[0] = -1;
	}
	if (has_next && cmd->fds[1] != -1)
	{
		dup2(cmd->fds[1], STDOUT_FILENO);
		close(cmd->fds[1]);
		cmd->fds[1] = -1;
	}
	if (!setup_redirections(cmd, false))
		free_shell(NULL, 1, NULL, NULL);
	if (cmd->cmd_name != NULL && ft_strlen(cmd->cmd_name) > 0)
		if (!execution(cmd, ev))
			free_shell(NULL, 1, NULL, NULL);
	free_shell(NULL, EXIT_SUCCESS, NULL, NULL);
}

void	execute_in_child(t_command *cmd, char **ev, int has_next)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		pid_zero(cmd, ev, has_next);
	}
	else if (pid < 0)
		free_shell(FORK_ERROR, EXIT_FAILURE, NULL, NULL);
	else
	{
		if (cmd->prev && cmd->prev->fds[0] != -1)
		{
			close(cmd->prev->fds[0]);
			cmd->prev->fds[0] = -1;
		}
		if (cmd->prev && cmd->prev->fds[1] != -1)
		{
            close(cmd->prev->fds[1]);
            cmd->prev->fds[1] = -1;
        }
		if (WIFEXITED(mini()->command_ret))
			mini()->command_ret = WEXITSTATUS(mini()->command_ret);
		else if (WIFSIGNALED(mini()->command_ret))
			mini()->command_ret = WTERMSIG(mini()->command_ret);
	}
}
