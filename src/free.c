/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 18:22:28 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/03/29 17:13:32 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_commands(t_command *commands)
{
	t_command	*tmp;
	t_doc		*doctmp;

	while (commands != NULL)
	{
		tmp = commands->next;
		free(commands->raw_cmd);
		if (commands->cmd_name && commands->expanded)
			free(commands->cmd_name);
		if (commands->args)
			free_list(commands->args);
		free_redirs_and_fds(commands);
		while (commands->docs)
		{
			doctmp = commands->docs;
			commands->docs = commands->docs->next;
			free(doctmp->doc);
			free(doctmp);
		}
		free(commands);
		commands = tmp;
	}
}

void	free_redirs_and_fds(t_command *command)
{
	t_redir	*tmp;

	if (command->fds[0] != -1)
		close(command->fds[0]);
	if (command->fds[1] != -1)
		close(command->fds[1]);
	if (command->stds[0] != -1)
		close(command->stds[0]);
	if (command->stds[1] != -1)
		close(command->stds[1]);
	if (!command->redirs)
		return ;
	while (command->redirs != NULL)
	{
		tmp = command->redirs->next;
		if (command->redirs->file)
			free(command->redirs->file);
		free(command->redirs);
		command->redirs = tmp;
	}
}

void	free_list(char **list)
{
	int	i;

	i = 0;
	if (list == NULL)
		return ;
	while (list[i] != NULL)
	{
		if (list && list[i])
			free(list[i]);
		i++;
	}
	if (list)
		free(list);
}

void	free_shell(char *err, int status, void (*cleanup_func)(void *),
		void *free_arg)
{
	t_mini	*m;

	m = mini();
	if (free_arg != NULL && cleanup_func != NULL)
		cleanup_func(free_arg);
	reset_mini(m);
	if (m->env_list)
		ft_lstclear(&(m->env_list), free);
	rl_clear_history();
	if (err)
		write(STDERR_FILENO, err, ft_strlen(err));
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	exit(status);
}

void	free_commands_wrapper(void *arg)
{
	t_command	*cmd;

	cmd = (t_command *)arg;
	free_commands(cmd);
}
