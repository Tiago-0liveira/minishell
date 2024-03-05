/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 21:19:38 by joaoribe          #+#    #+#             */
/*   Updated: 2024/03/04 21:24:24 by joaoribe         ###   ########.fr       */
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

int	heredoc_signs_set(t_mini *mini, t_command *cmd)
{
	char	*heredoc_fd;

	signal(SIGQUIT, exec_sig);
	signal(SIGPIPE, exec_sig);
	signal(SIGINT, exec_sig);
	if (cmd->redirs && cmd->redirs->type == RED_AIN)
	{
		heredoc_fd = heredoc(mini);
		mini->hdfd = open(heredoc_fd, O_RDONLY);
		free(mini->hd_limiter);
		free(heredoc_fd);
		if (mini->command_ret == 130)
		{
			printf("\n");
			return (0);
		}
	}
	return (1);
}

void	bin_epe(t_mini *mini, t_command *cmd)
{
	close(mini->input.pip[0]);
	built_in(mini, cmd, 0);
}

void	fd_error(t_redir *redir, bool isparent)
{
	error_msg(FD_NOT_FOUND, redir->file);
	if (isparent)
		return ;
	free_shell(NULL, 0, NULL, NULL);
}

void	setup_redirections(t_command *cmd, bool isparent)
{
	int		fd;
	t_redir	*redir;

	redir = cmd->redirs;
	while (redir != NULL)
	{
		if (redir->type == RED_IN)
			fd = open(redir->file, O_RDONLY);
		else if (redir->type == RED_OUT)
			fd = open(redir->file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		else if (redir->type == RED_AOUT)
			fd = open(redir->file, O_CREAT | O_WRONLY | O_APPEND, 0644);
		if (fd < 0)
			fd_error(redir, isparent);
		if ((redir->type == RED_IN))
			dup2(fd, STDIN_FILENO);
		else if ((redir->type == RED_AIN))
			dup2(mini()->hdfd, STDIN_FILENO);
		else if (redir->type == RED_OUT || redir->type == RED_AOUT)
			dup2(fd, STDOUT_FILENO);
		close(fd);
		redir = redir->next;
	}
}
