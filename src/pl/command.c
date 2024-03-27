/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 19:38:18 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/03/27 16:00:21 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	command_parser(char *input)
{
	t_str_ex	ex;
	int			j;
	t_command	*cmd;

	memset(&ex, 0, sizeof(t_str_ex));
	j = 0;
	while (1)
	{
		if (input[ex.i] == QUOTE && !ex.dquotes)
			ex.quotes = !ex.quotes;
		else if (input[ex.i] == DQUOTE && !ex.quotes)
			ex.dquotes = !ex.dquotes;
		else if ((input[ex.i] == PIPE || !input[ex.i])
			&& !ex.quotes && !ex.dquotes)
		{
			cmd = init_command(input + j, ex.i - j);
			command_add_back(cmd);
			if (!input[ex.i])
				break ;
			j = ++ex.i;
			continue ;
		}
		ex.i++;
	}
	return (true);
}

t_command	*init_command(char *input, int len)
{
	t_command	*command;

	command = malloc(sizeof(t_command));
	if (!command)
		free_shell(MALLOC_ERROR, STDERR_FILENO, NULL, NULL);
	command->cmd_name = NULL;
	command->raw_cmd = ft_substr(input, 0, len);
	command->args = NULL;
	command->redirs = NULL;
	command->expanded = false;
	command->status = 0;
	command->next = NULL;
	return (command);
}

void	build_command(t_command *cmd)
{
	char	**raw_commands;
	char	*expanded;
	size_t	i;
	size_t	end;

	i = 0;
	expanded = str_expander_hd2(cmd->raw_cmd);
	if (!expanded)
		return (cmd->cmd_name = ft_strdup(""), (void) NULL);
	raw_commands = parse(expanded);
	end = parse_size(expanded);
	free(expanded);
	i = 0;
	while (i < end)
	{
		if (!update_command(cmd, raw_commands, &i, end))
			return (free(cmd));
		i++;
	}
	if (cmd->args == NULL)
		cmd->cmd_name = "";
	else
		cmd->cmd_name = cmd->args[0];
	free_list(raw_commands);
}
