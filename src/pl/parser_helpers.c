/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 16:09:31 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/03/06 03:46:20 by joaoribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	assign_redir(t_command *command, char *redir_file,
		enum e_redir_type type)
{
	t_redir	*redir;
	t_redir	*last_redir;

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

bool	update_command(t_command *command, char **raw_commands, size_t *i,
		size_t end)
{
	bool	ret;

	if (redir_type(raw_commands[*i]) != RED_NULL)
	{
		if (++(*i) < end)
			if (!assign_redir(command, raw_commands[*i],
					redir_type(raw_commands[*i - 1])))
				return (false);
	}
	else
	{
		if (!ft_strncmp(raw_commands[*i], "cd", 3))
			mini()->if_cd = 1;
		ret = valid_cmd_arg(raw_commands[*i]);
		if (!ret)
		{
			free(raw_commands[*i]);
			raw_commands[*i] = ft_strdup("");
		}
		if (ret && !add_arg(command, raw_commands[*i]))
			free_shell(MALLOC_ERROR, STDERR_FILENO, free_commands_wrapper,
				command);
	}
	return (true);
}

bool	add_arg(t_command *command, char *section)
{
	char	**new_args;
	size_t	k;
	size_t	i;

	k = 0;
	while (command->args && command->args[k])
		k++;
	new_args = malloc((k + 2) * sizeof(char *));
	if (!new_args)
		return (false);
	i = 0;
	while (i < k)
	{
		new_args[i] = command->args[i];
		i++;
	}
	new_args[i] = ft_strdup(section);
	if (!new_args[k])
		return (false);
	new_args[k + 1] = NULL;
	free(command->args);
	command->args = new_args;
	return (true);
}

char	*get_redir(char **line)
{
	int		i;
	char	*tmp;

	i = redir_size(*line);
	(*line) += i;
	tmp = ft_substr(*line - i, 0, i);
	if (redir_type(*line - i) == RED_OUT && *line && **line == PIPE)
		(*line) += i;
	return (tmp);
}
