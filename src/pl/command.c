/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 19:38:18 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/03/26 19:38:38 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	command_parser(char *input)
{
	bool		quotes;
	bool		dquotes;
	int			i;
	int			j;
	t_command	*cmd;

	quotes = false;
	dquotes = false;
	i = 0;
	j = 0;
	while (1)
	{
		if (input[i] == QUOTE && !dquotes)
			quotes = !quotes;
		else if (input[i] == DQUOTE && !quotes)
			dquotes = !dquotes;
		else if ((input[i] == PIPE || !input[i]) && !quotes && !dquotes)
		{
			cmd = init_command(input + j, i - j);
			if (!cmd)
				free_shell(MALLOC_ERROR, STDERR_FILENO, NULL, NULL);
			command_add_back(cmd);
			if (!input[i])
				break ;
			j = ++i;
			continue ;
		}
		i++;
	}
	return (true);
}

t_command	*init_command(char *input, int len)
{
	t_command	*command;

	command = malloc(sizeof(t_command));
	if (!command)
		return (NULL);
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
	{
		cmd->cmd_name = ft_strdup("");
		return ;
	}
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



