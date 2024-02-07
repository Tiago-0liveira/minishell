/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 20:28:47 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/02/07 18:08:52 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	parse_input(t_mini *mini)
{
	size_t	commands;
	size_t	i;
	t_command	*command;
	char 	**raw_commands;
	
	raw_commands = ft_split(mini->input.raw_line, PIPE);
	if (raw_commands == NULL || raw_commands[0] == NULL)
	{
		mini->commands = parse_command(mini->input.raw_line);
		return (true);
	}
	commands = mini->input.pipe_c + 1;
	i = 0;
	while (i < commands)
	{
		command = parse_command(raw_commands[i]);
		if (!command)
			return (false);/* free raw_comamnds */
		if (!mini->commands)
			mini->commands = command;
		else
			command_add_back(mini->commands, command);
		i++;
	}
	return (true);
}

t_command *parse_command(char *raw_command)
{
	t_command *command;
	char **args;
	
	command = malloc(sizeof(t_command));
	if (!command)
		return (NULL);
	command->raw_command = raw_command;
	args = split_by_redirs(ft_strtrim(raw_command, " "));/* check for mem leak */
	command->cmd_name = args[0];
	command->redirs.redir_c = 0;
	command->next = NULL;
	args++;
	while (*args)
	{
		if (is_redir(*args))
		{
			command->redirs.redir_c++;
			//command_add_redir(command, *args);
			
		}
		
		args++;
	}
	
	return (command);
}

void	command_add_back(t_command *command, t_command *new_command)
{
	t_command *last_command;
	
	last_command = command;
	while (last_command->next)
		last_command = last_command->next;
	last_command->next = new_command;
}

void	command_add_redir(t_command *command, char *redir)
{
	//t_redir *new_redir;
	(void)command;
	(void)redir;
	/*new_redir = malloc(sizeof(t_redir));
	if (!new_redir)
		return ;
	
	new_redir->file = redir + 1;
	if (*redir == REDIR_IN)
		new_redir->type = REDIR_IN;
	else if (*redir == REDIR_APPEND_IN)
		new_redir->type = REDIR_APPEND_IN;
	else if (*redir == REDIR_OUT)
		new_redir->type = REDIR_OUT;
	else if (*redir == REDIR_APPEND_OUT)
		new_redir->type = REDIR_APPEND_OUT;*/
	/*new_redir->next = command->redirs;
	command->redirs = new_redir;*/
}