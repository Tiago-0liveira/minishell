/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 21:19:38 by joaoribe          #+#    #+#             */
/*   Updated: 2024/03/29 17:16:14 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	*ft_lstlast_mini(t_command *lst)
{
	if (!lst)
		return (0);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

void	fd_error(t_redir *redir, bool isparent)
{
	error_msg(FD_NOT_FOUND, redir->file);
	if (isparent)
		return ;
	free_shell(NULL, 0, NULL, NULL);
}

bool	setup_redirections(t_command *cmd, bool isparent)
{
	int		fd;
	t_redir	*redir;

	redir = cmd->redirs;
	fd = 0;
	while (redir != NULL)
	{
		if (redir->type == RED_IN)
			fd = open(redir->file, O_RDONLY);
		else if (redir->type == RED_AIN)
			fd = open(redir->file, O_RDONLY);
		else if (redir->type == RED_OUT)
			fd = open(redir->file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		else if (redir->type == RED_AOUT)
			fd = open(redir->file, O_CREAT | O_WRONLY | O_APPEND, 0644);
		if (fd < 0)
			return (fd_error(redir, isparent), false);
		if (redir->type == RED_IN || redir->type == RED_AIN)
			dup2(fd, STDIN_FILENO);
		else if (redir->type == RED_OUT || redir->type == RED_AOUT)
			dup2(fd, STDOUT_FILENO);
		close(fd);
		redir = redir->next;
	}
	return (true);
}

void	handle_command_fail(t_command *cmd)
{
	if (cmd->prev && cmd->prev->fds[0] != -1)
	{
		close(cmd->prev->fds[0]);
		cmd->prev->fds[0] = -1;
	}
	if (cmd->prev && cmd->fds[1] != -1)
	{
		close(cmd->fds[1]);
		cmd->fds[1] = -1;
	}
}
