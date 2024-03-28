/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 20:28:45 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/03/28 18:26:05 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	input_error_check(t_mini *mini)
{
	char	*line_cursor;
	bool	quotes;
	bool	dquotes;

	line_cursor = mini->input.raw_line;
	quotes = false;
	dquotes = false;
	while (*line_cursor)
	{
		if (!skip_spaces(&line_cursor))
			return (false);
		if (*line_cursor == QUOTE && !dquotes)
			quotes = !quotes;
		else if (*line_cursor == DQUOTE && !quotes)
			dquotes = !dquotes;
		else if (*line_cursor == PIPE && !quotes && !dquotes
			&& (line_cursor - 1 > mini->input.raw_line
				&& redir_type(line_cursor - 1) != RED_OUT))
			mini->input.pipe_c++;
		line_cursor++;
	}
	mini->input.len = line_cursor - mini->input.raw_line;
	if (quotes || dquotes)
		return (error_msg_ret(OPEN_QUOTES_ERROR, NULL, EXIT_FAILURE), false);
	return (true);
}

bool	syntax_check(char *input)
{
	char		**raw_commands;

	raw_commands = parse(input);
	if (!semantic_checker(raw_commands))
		return (free_list(raw_commands), false);
	free_list(raw_commands);
	return (true);
}

// check for all types of semantic errors
bool	semantic_checker(char **sections)
{
	bool	isvalid;
	int		i;
	char	*error;
	char	*last_section;

	last_section = NULL;
	isvalid = true;
	i = 0;
	error = NULL;
	while (sections && sections[i] && isvalid && !error)
	{
		isvalid = valid_section(sections, &i, &last_section, &error);
		i++;
	}
	if (error && !isvalid)
		return (error_msg_ret(SYNTAX_ERROR, error, EXIT_FAILURE), false);
	if (isvalid && i > 0 && sections[--i][0] == '|')
	{
		mini()->solo_pipe = 1;
		return (true);
	}
	else
		mini()->solo_pipe = 0;
	return (isvalid);
}

bool	valid_section(char **sections, int *i,
	char **last_section, char **error)
{
	if (*sections[*i] == PIPE)
	{
		if (!*last_section || (sections[*i + 1] && *sections[*i + 1] == PIPE))
			return (*error = sections[*i], false);
	}
	if (redir_type(*last_section) != RED_NULL)
	{
		if (redir_type(sections[*i]) != RED_NULL || *sections[*i] == PIPE)
			return (*error = sections[*i], false);
		(*i)++;
	}
	else if (redir_type(sections[*i]) != RED_NULL)
	{
		if (!sections[*i + 1])
			return (*error = "newline", false);
		else if (!check_ambiguitity(sections[*i + 1]))
			return (false);
		else if (redir_type(sections[*i + 1]) != RED_NULL
			|| *sections[*i + 1] == PIPE)
			return (*error = sections[*i + 1], false);
		(*i)++;
	}
	*last_section = sections[*i];
	return (true);
}

bool	check_ambiguitity(char *file)
{
	char	*tmp;
	bool	quotes;
	bool	dquotes;
	int		i;

	tmp = str_expander_hd2(file);
	if (!tmp)
		return (error_msg_ret(AMB_REDIR, file, EXIT_FAILURE), false);
	i = 0;
	quotes = false;
	dquotes = false;
	while (tmp && tmp[i])
	{
		if (tmp[i] == QUOTE && !dquotes)
			quotes = !quotes;
		else if (tmp[i] == DQUOTE && !quotes)
			dquotes = !dquotes;
		else if (tmp[i] == SPACE && !quotes && !dquotes)
			return (error_msg_ret(AMB_REDIR, file, EXIT_FAILURE), false);
		i++;
	}
	free(tmp);
	return (true);
}
