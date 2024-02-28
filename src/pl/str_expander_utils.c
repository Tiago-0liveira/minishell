/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_expander_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 19:21:41 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/02/28 18:35:39 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	expand_command(t_command *cmd)
{
	expand_args(cmd);
	if (!expand_redirs(cmd))
		return (false);
	if (cmd->args && cmd->args[0] != NULL)
		cmd->cmd_name = cmd->args[0];
	return (true);
}

void	expand_args(t_command *cmd)
{
	size_t	i;
	char	*expanded;

	i = 0;
	while (cmd->args && cmd->args[i])
	{
		if (cmd->args[i] && cmd->args[i][0] == '\0')
		{
			i++;
			continue ;
		}
		expanded = str_expander(cmd->args[i]);
		free(cmd->args[i]);
		cmd->args[i] = expanded;
		i++;
	}
}

bool	expand_redirs(t_command *cmd)
{
	t_redir	*redir;
	char	*expanded;

	redir = cmd->redirs;
	while (redir)
	{
		if (redir->file && redir->file[0] == '\0')
		{
			redir = redir->next;
			continue ;
		}
		expanded = str_expander(redir->file);
		free(redir->file);
		redir->file = expanded;
		if (redir->type == RED_IN)
			if (access(redir->file, F_OK | R_OK) != 0)
				return (error_msg_ret(FD_NOT_FOUND, redir->file, EXIT_FAILURE), false);
		redir = redir->next;
	}
	return (true);
}
