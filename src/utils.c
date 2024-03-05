/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 17:13:51 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/03/04 21:10:17 by joaoribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

enum e_redir_type	redir_type(char *line)
{
	if (line == NULL)
		return (RED_NULL);
	if (strncmp(line, REDIR_APPEND_IN, 2) == 0)
		return (RED_AIN);
	if (strncmp(line, REDIR_APPEND_OUT, 2) == 0)
		return (RED_AOUT);
	if (*line == REDIR_IN)
		return (RED_IN);
	if (*line == REDIR_OUT)
		return (RED_OUT);
	return (RED_NULL);
}

bool	valid_env_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

bool	quoted_str(char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (*str == '\'' || *str == '\"')
			return (true);
		i++;
	}
	return (false);
}

bool	valid_cmd_arg(char *str)
{
	bool	quoted;
	int		l;

	quoted = quoted_str(str);
	l = str_expander_len(str);
	if (!quoted && l == 0)
		return (false);
	return (l > 0 || quoted);
}

void	free_assign_null(void **ptr)
{
	if (*ptr)
		free(*ptr);
	*ptr = NULL;
}
