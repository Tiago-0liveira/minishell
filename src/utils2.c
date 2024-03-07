/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 18:05:11 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/03/07 00:23:18 by joaoribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	skip_spaces(char **line)
{
	if (**line == '|')
	{
		error_msg(SYNTAX_ERROR, "|");
		return (NULL);
	}
	while (**line == ' ')
	{
		if (**line == '|')
		{
			error_msg(SYNTAX_ERROR, "|");
			return (NULL);
		}
		(*line)++;
	}
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
