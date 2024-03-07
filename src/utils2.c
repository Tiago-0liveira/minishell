/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 18:05:11 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/03/07 19:14:34 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	skip_spaces(char **line)
{
	while (**line == ' ')
		(*line)++;
	return (**line);
}

size_t	redir_size(char *line)
{
	enum e_redir_type	type;

	type = redir_type(line);
	if (type == RED_AIN || type == RED_AOUT)
		return (2);
	if (type == RED_IN || type == RED_OUT || (line != NULL && (*line == PIPE
				|| *line == ' ')))
		return (1);
	return (0);
}

bool	has_char_in_set(char *s, char *set)
{
	int	i;

	i = 0;
	while (s && s[i])
	{
		if (ft_strchr(set, s[i]))
			return (true);
		i++;
	}
	return (false);
}
