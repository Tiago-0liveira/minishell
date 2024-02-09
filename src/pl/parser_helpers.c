/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 16:09:31 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/02/09 20:21:27 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	redir_size(char *line)
{
	if (redir_type(line) == RED_AIN || redir_type(line) == RED_AOUT)
		return (2);
	if (redir_type(line) == RED_IN || redir_type(line) == RED_OUT)
		return (1);
	return (0);
}

size_t	count_args(char *line)
{
	size_t	c;
	bool	quotes;

	c = 0;
	quotes = false;
	while (*line)
	{
		skip_spaces(&line);
		if (redir_size(line))
		{
			c++;
			line += redir_size(line);
		}
		else
		{
			while (*line && ((!(*line == ' ') && !is_redir(line + 1)) || quotes))
			{
				if (*line == QUOTE || *line == DQUOTE)
					quotes = !quotes;
				line++;
			}
			c++;
		}
	}
	return (c);
}

char	*get_next_arg(char **line)
{
	size_t	i;
	char	*arg;
	bool	quotes;

	i = 0;
	quotes = false;
	if (redir_size(*line))
	{
		arg = ft_substr(*line, 0, redir_size(*line));
		(*line) += redir_size(*line);
		return (arg);
	}
	else
	{
		while ((*line)[i] && ((!((*line)[i] == ' ') && !is_redir(*line + i))
				|| quotes))
		{
			if ((*line)[i] == QUOTE || (*line)[i] == DQUOTE)
				quotes = !quotes;
			i++;
		}
		arg = ft_substr(*line, 0, i);
		(*line) += i;
	}
	return (arg);
}
