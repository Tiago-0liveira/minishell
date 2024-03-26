/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 20:28:47 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/03/26 19:38:16 by tiagoliv         ###   ########.fr       */
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
