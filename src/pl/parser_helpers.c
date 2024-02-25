/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 16:09:31 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/02/25 19:21:21 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	redir_size(char *line)
{
	enum e_redir_type	type;

	type = redir_type(line);
	if (type == RED_AIN || type == RED_AOUT)
		return (2);
	if (type == RED_IN || type == RED_OUT || (line != NULL && (*line == '|'
				|| *line == ' ')))
		return (1);
	return (0);
}

bool	assign_redir(t_command *command, char *redir_file,
		enum e_redir_type type)
{
	t_redir	*redir;
	t_redir	*last_redir;

	if (type == RED_IN && access(redir_file, F_OK) == -1)
		return (error_msg(FD_NOT_FOUND, redir_file), false);
	redir = malloc(sizeof(t_redir));
	if (!redir)
		free_shell(MALLOC_ERROR, STDERR_FILENO, free_commands_wrapper, command);
	redir->file = ft_strdup(redir_file);
	if (!redir->file)
		free_shell(MALLOC_ERROR, STDERR_FILENO, free_commands_wrapper, command);
	redir->type = type;
	redir->next = NULL;
	if (!command->redirs)
		command->redirs = redir;
	else
	{
		last_redir = command->redirs;
		while (last_redir && last_redir->next != NULL)
			last_redir = last_redir->next;
		last_redir->next = redir;
	}
	return (true);
}

void	command_add_back(t_command *new_command)
{
	t_mini		*mi;
	t_command	*last_command;

	mi = mini();
	if (mi->commands == NULL)
	{
		mi->commands = new_command;
		return ;
	}
	last_command = mi->commands;
	while (last_command->next != NULL)
	{
		last_command = last_command->next;
	}
	last_command->next = new_command;
}
