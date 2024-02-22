/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 20:28:45 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/02/22 01:10:59 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* TODO: check for all types of semantic errors */
bool	input_error_check(t_mini *mini)
{
	char	*line_cursor;
	bool	quotes;
	bool	dquotes;
	bool	escaped;

	line_cursor = mini->input.raw_line;
	quotes = false;
	dquotes = false;
	escaped = false;
	while (*line_cursor)
	{
		if (!skip_spaces(&line_cursor))
			return (false);
		if (*line_cursor == ESCAPE_CHAR || escaped)
			escaped = !escaped;
		else if (*line_cursor == QUOTE)
			quotes = !quotes;
		else if (*line_cursor == DQUOTE)
			dquotes = !dquotes;
		else if (*line_cursor == PIPE)
			mini->input.pipe_c++;
		line_cursor++;
	}
	mini->input.len = line_cursor - mini->input.raw_line;
	return (!(quotes || dquotes) && ft_strlen(mini->input.raw_line) > 0);
}

bool	skip_spaces(char **line)
{
	while (**line == ' ')
		(*line)++;
	return (**line);
}

bool	escaping(char c, bool *escaping)
{
	if (c == ESCAPE_CHAR)
	{
		*escaping = true;
		return (true);
	}
	if (!*escaping)
		return (false);
	*escaping = false;
	return (c == QUOTE || c == DQUOTE || c == PIPE || c == ESCAPE_CHAR
		|| c == ENV_VAR || c == SPACE || c == REDIR_IN || c == REDIR_OUT
		|| c == '\n');
}

// check for all types of semantic errors
bool	semantic_checker(char **sections)
{
	bool	commandexpected;
	bool	isvalid;
	int		i;

	commandexpected = true;
	isvalid = true;
	i = 0;
	while (sections && sections[i] && isvalid)
	{
		isvalid = valid_arg(sections, i, &commandexpected);
		i++;
	}
	printf("semantic_checker output: %d\n", isvalid && !commandexpected);
	if (!isvalid || commandexpected)
		error_msg(SYNTAX_ERROR, sections[i - 1]); // TODO: need better error msg
	return (isvalid && !commandexpected);
}

// this function needs to check if the sequence of sections is valid
// for example "ls | | ls" is invalid and "| " is invalid
// it also needs to check for redirections
// use redir_type to check for redirections
bool	valid_arg(char **sections, int i, bool *commandexpected)
{
	if (redir_type(sections[i]) != RED_NULL || *sections[i] == PIPE)
	{
		if (*commandexpected)
			return (false);
		*commandexpected = true;
	}
	else
		*commandexpected = false;
	return (true);
}
