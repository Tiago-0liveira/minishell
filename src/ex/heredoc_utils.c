/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 15:30:50 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/03/28 17:24:37 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*heredoc_ctrd_error(t_mini *mini, char *delim)
{
	char	*err;
	char	*line_count;

	line_count = ft_itoa(mini->input.hd_lines);
	if (!line_count)
		free_shell(MALLOC_ERROR, EXIT_FAILURE, NULL, NULL);
	err = ft_strnjoin(5, HEREDOC_CTRD_WARNING, line_count, " delimited by end-of-file (wanted `", delim, "')");
	free(line_count);
	if (!err)
		free_shell(MALLOC_ERROR, EXIT_FAILURE, NULL, NULL);	
	return (err);
}

void	heredoc_cleanup(t_mini *mini, int fd, int fds[2])
{
	if (mini->input.stdin_cpy != -1)
	{
		dup2(mini->input.stdin_cpy, STDIN_FILENO);
		close(mini->input.stdin_cpy);
		mini->input.stdin_cpy = -1;
		close(fds[0]);
	}
	close(fd);
}

int	heredoc_process_input(char **input, char *delim, int fd, int fds[2])
{
	char	*tmp;

	if (*input && (*input)[ft_strlen(*input) - 1] == '\n')
	{
		tmp = ft_substr(*input, 0, ft_strlen(*input) - 1);
		if (!tmp)
			free_shell(MALLOC_ERROR, EXIT_FAILURE, NULL, NULL);
		free(*input);
		*input = tmp;
	}
	if (*input && (!ft_strcmp(*input, delim) && (*input)[0] != '\0'))
		return (free(tmp), heredoc_cleanup(mini(), fd, fds), 0);
	if (!*input)
	{
		if (g_signal != SIGINT)
		{
			tmp = heredoc_ctrd_error(mini(), delim);
			error_msg(tmp, NULL);
			return (free(tmp), heredoc_cleanup(mini(), fd, fds), -1);
		}
		return (heredoc_cleanup(mini(), fd, fds), -1);
	}
	return (1);
}
