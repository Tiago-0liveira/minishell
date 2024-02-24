/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 17:13:51 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/02/24 15:57:40 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

enum e_redir_type	redir_type(char *line)
{
	if (line == NULL || ft_strlen(line) > 2)
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

void	print_command(t_command *command)
{
	size_t	i;
	t_redir	*redir;

	i = 0;
	DEBUG_MSG("print_command: ");
	while (command->args[i])
	{
		printf("|%s| ", command->args[i]);
		i++;
	}
	printf("\n");
	redir = command->redirs;
	while (redir)
	{
		DEBUG_MSG("redir:|%s| type:", redir->file);
		if (redir->type)
		{
			if (redir->type == RED_IN)
				printf("RED_IN\n");
			else if (redir->type == RED_AIN)
				printf("RED_AIN\n");
			else if (redir->type == RED_OUT)
				printf("RED_OUT\n");
			else if (redir->type == RED_AOUT)
				printf("RED_AOUT\n");
		}
		redir = redir->next;
		if (redir)
			DEBUG_MSG("output will be piped to:\n");
	}
}
