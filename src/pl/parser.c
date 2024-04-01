/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 20:28:47 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/04/01 12:22:58 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		if (!section)
			return (0);
		free(section);
		si++;
	}
	return (si);
}

char	*get_next_section(char **line)
{
	bool	quotes;
	bool	dquotes;
	int		i;

	quotes = false;
	dquotes = false;
	i = 0;
	if (redir_size(*line) > 0)
		return (get_redir(line));
	while (**line && (quotes || dquotes || redir_size(*line) == 0))
	{
		if (**line == QUOTE)
			quotes = !quotes;
		else if (**line == DQUOTE)
			dquotes = !dquotes;
		i++;
		(*line)++;
	}
	return (ft_substr(*line - i, 0, i));
}

char	**parse(char *input)
{
	size_t	sections;
	size_t	i;
	char	*line;
	char	**s;

	line = input;
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

bool	command_parser(char *input)
{
	t_str_ex	ex;
	int			j;
	t_command	*cmd;

	ft_memset(&ex, 0, sizeof(t_str_ex));
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

bool	build_command(t_command *cmd)
{
	char	**raw_commands;
	char	*expanded;
	size_t	i;
	size_t	end;

	i = 0;
	expanded = str_expander_hd2(cmd->raw_cmd);
	if (!expanded)
		return (false);
	raw_commands = parse(expanded);
	end = parse_size(expanded);
	free(expanded);
	i = 0;
	while (i < end)
	{
		if (!update_command(cmd, raw_commands, &i, end))
			return (free(cmd), false);
		i++;
	}
	if (cmd->args == NULL)
		cmd->cmd_name = "";
	else
		cmd->cmd_name = cmd->args[0];
	return (free_list(raw_commands), true);
}
