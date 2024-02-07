/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 17:13:51 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/02/07 17:20:35 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_redir(char *line)
{
	if (*line == REDIR_IN || *line == REDIR_OUT)
		return (true);
	if (strncmp(line, REDIR_APPEND_IN, 2) == 0 || strncmp(line, REDIR_APPEND_OUT, 2) == 0)
		return (true);
	return (false);
}
