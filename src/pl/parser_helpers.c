/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 16:09:31 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/02/07 18:11:53 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char **split_by_redirs(char *line)
{
	size_t	redirs_c;
	size_t	i;
	size_t	j;
	char *next_redir;
	char **words;

	redirs_c = count_redirs(line);
	j = ft_strlen(line);
	words = malloc(sizeof(char *) * (redirs_c + 2));
	if (!words)
		return (NULL);
	words[redirs_c + 1] = NULL;
	i = 0;
	printf("here2|%s|\n", line);
	while (*line)
	{	
		next_redir = find_next_redir(line);
		if (next_redir)
		{
			words[i] = ft_strtrim(ft_substr(line, 0, next_redir - line), " ");
			line += ft_strlen(next_redir);
		}
		else
			words[i] = ft_strtrim(line, " ");
		printf("word: %s\n", words[i]);
		i++;
	}
	exit(1);
	return (words);
}

size_t	count_redirs(char *line)
{
	size_t count;

	count = 0;
	while (*line)
	{
		if (is_redir(line))
			count++;
		line++;
	}
	return (count);
}

char **split_by_pipes(char *line)
{
	(void)line;
	return (NULL);
}

char *find_next_redir(char *line)
{
	while (*line)
	{
		if (is_redir(line))
			return (line);
		line++;
	}
	return (NULL);
}

char *find_next_pipe(char *line)
{
	while (*line)
	{
		if (*line == PIPE)
			return (line);
		line++;
	}
	return (NULL);
}