/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 22:55:38 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/03/02 01:58:17 by joaoribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	handle_redir_in(t_command *cmd, t_redir *redir)
{
	int	fd;

	if (cmd->std.in != STDIN_FILENO)
		close(cmd->std.in);
	fd = open(redir->file, O_RDONLY);
	if (fd == -1)
	{
		error_msg(FD_NOT_FOUND, redir->file);
		free_shell(NULL, 0, NULL, NULL);
	}
	if (redir->type == RED_IN)
		cmd->std.in = fd;
	else
		cmd->std.in =cmd->doctor.fd;
	dup2(cmd->std.in, STDIN_FILENO);
	close(fd);
	return (true);
}

bool	handle_redir_out(t_command *cmd, t_redir *redir)
{
	int	fd;
	int	flags;

	if (cmd->std.out != STDOUT_FILENO)
		close(cmd->std.out);
	flags = O_WRONLY | O_CREAT | O_TRUNC;
	if (redir->type == RED_AOUT)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	fd = open(redir->file, flags, 0644);
	if (fd == -1)
	{
		error_msg(FD_NOT_FOUND, redir->file);
		free_shell(NULL, 0, NULL, NULL);
	}
	cmd->std.out = fd;
	dup2(cmd->std.out, STDOUT_FILENO);
	close(fd);
	return (true);
}

void	handle_redirections(t_command *cmd)
{
	t_redir	*redir;

	redir = cmd->redirs;
	while (redir != NULL)
	{
		if (redir->type == RED_OUT || redir->type == RED_AOUT)
			handle_redir_out(cmd, redir);
		else if (redir->type == RED_IN || redir->type == RED_AIN)
			handle_redir_in(cmd, redir);
		redir = redir->next;
	}
}
