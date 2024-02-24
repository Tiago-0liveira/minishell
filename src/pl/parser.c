/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 20:28:47 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/02/24 15:53:31 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	parse_input(t_mini *mini)
{
	int			commands;
	size_t		i;
	size_t		j;
	char		**raw_commands;
	t_command	*command;

	raw_commands = parse(mini);
	if (!raw_commands)
		free_shell(MALLOC_ERROR, EXIT_FAILURE, free, raw_commands);
	/*if (!semantic_checker(raw_commands))
		return (free_list(raw_commands), false);*/
	commands = ((int)mini->input.pipe_c) + 1;
	i = 0;
	j = 0;
	while (commands > 0)
	{
		i = j;
		while (raw_commands[j] && *raw_commands[j] != PIPE)
			j++;
		command = construct_command(raw_commands + i, j - i);
		if (!command)
			free_shell(MALLOC_ERROR, STDERR_FILENO, NULL, NULL);
		print_command(command);
		command_add_back(command);
		commands--;
		j++;
	}
	free_list(raw_commands);
	return (true);
}

size_t	parse_size(char *line)
{
	int		si;
	char	*section;

	section = NULL;
	si = 0;
	while (*line)
	{
		skip_spaces(&line);
		section = get_next_section(&line);
		// DEBUG_MSG("section[%d]: %s\n", si, section);
		if (!section)
			return (0);
		free(section);
		si++;
	}
	return (si);
}

char	*get_next_section(char **line)
{
	char	*start;
	bool	quotes;
	int		i;
	bool	escaped;

	quotes = false;
	start = *line;
	escaped = false;
	while (**line && (quotes || !redir_size(*line)) && skip_spaces(line))
	{
		if (**line == ESCAPE_CHAR || escaped)
			escaped = !escaped;
		else if (**line == QUOTE || **line == DQUOTE)
			quotes = !quotes;
		(*line)++;
	}
	i = *line - start;
	if (i == 0 && redir_size(*line))
	{
		i = redir_size(*line);
		(*line) += i;
	}
	return (substr_expander(start, i));
}

char	**parse(t_mini *mini)
{
	size_t	sections;
	size_t	i;
	char	*line;
	char	**s;

	line = mini->input.raw_line;
	sections = parse_size(line);
	i = 0;
	s = malloc((sections + 1) * sizeof(char *));
	if (!s)
		free_shell(MALLOC_ERROR, STDERR_FILENO, NULL, NULL);
	s[sections] = NULL;
	while (i < sections)
	{
		skip_spaces(&line);
		s[i] = get_next_section(&line);
		if (s[i] == NULL)
			return (free_list(s), NULL);
		i++;
	}
	return (s);
}

t_command	*construct_command(char **raw_commands, size_t end)
{
	t_command	*command;
	size_t		i;

	command = malloc(sizeof(t_command));
	if (!command)
		free_shell(MALLOC_ERROR, STDERR_FILENO, NULL, NULL);
	i = 0;
	command->redirs = NULL;
	command->args = NULL;
	while (i < end)
	{
		update_command(command, raw_commands, &i, end);
		// TODO: IGNORING RED_AIN FOR NOW AND TREATING AS normal RED_IN
		// maybe handle this inside assign_redir
		/*if (type == RED_AIN)
			mini->hd_limiter = ft_strdup(raw_commands[i]);*/
		i++;
	}
	if (command->args == NULL)
		command->cmd_name = "";
	else
		command->cmd_name = command->args[0];
	command->next = NULL;
	return (command);
}

void	update_command(t_command *command, char **raw_commands, size_t *i,
		size_t end)
{
	if (redir_type(raw_commands[*i]) != RED_NULL)
	{
		if (++(*i) < end)
			assign_redir(command, raw_commands[*i], redir_type(raw_commands[*i - 1]));
		/* it might be an error if it didnt enter the if */
	}
	else
	{
		if (!add_arg(command, raw_commands[*i]))
			free_shell(MALLOC_ERROR, STDERR_FILENO, free_commands_wrapper,
				command);
	}
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
		// free(command->args[i]);
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
