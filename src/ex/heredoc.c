/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 02:29:00 by joaoribe          #+#    #+#             */
/*   Updated: 2024/03/30 00:07:42 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	heredoc_read_input_to_file(char *delim, char *file)
{
	int		fd;
	int		fds[2];
	int		ret;
	char	*input;

	fd = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
		error_msg(FD_NOT_FOUND, "heredoc");
	prepare_for_input(fds, solo_pipe_sigint_handler, NULL);
	if (!mini()->input.hd_lines)
		mini()->input.hd_lines = 1;
	ret = 1;
	while (ret)
	{
		write(1, "> ", 2);
		input = get_next_line(mini()->input.stdin_cpy);
		ret = heredoc_process_input(&input, delim, fd, fds);
		if (ret == -1)
			return (false);
		mini()->input.hd_lines++;
		input = expand_input_hd(input);
		ft_putendl_fd(input, fd);
		free(input);
	}
	return (input_cleanup(mini(), &fd, fds), true);
}

char	*heredoc_get_new_file(t_mini *mini)
{
	char	*file;
	char	*num;

	num = ft_itoa(mini->doc_n++);
	if (!num)
		free_shell(MALLOC_ERROR, STDERR_FILENO, NULL, NULL);
	file = ft_strjoin("/tmp/hd", num);
	free(num);
	if (!file)
		free_shell(MALLOC_ERROR, STDERR_FILENO, NULL, NULL);
	return (file);
}

char	*heredoc(t_mini *mini, char *delim)
{
	char			*file;
	struct termios	termios;
	struct termios	termios_backup;
	bool			res;

	file = heredoc_get_new_file(mini);
	tcgetattr(STDIN_FILENO, &termios_backup);
	termios = termios_backup;
	termios.c_cc[VQUIT] = _POSIX_VDISABLE;
	tcsetattr(STDIN_FILENO, TCSANOW, &termios);
	if (!delim)
		free_shell(MALLOC_ERROR, EXIT_FAILURE, NULL, NULL);
	res = heredoc_read_input_to_file(delim, file);
	free(delim);
	tcsetattr(STDIN_FILENO, TCSANOW, &termios_backup);
	if (!res)
		return (free(file), NULL);
	return (file);
}

char	*sanitize_hd_delim(char *delim)
{
	t_str_ex	ex;
	char		*expanded;
	int			len;

	len = sanitize_hd_delim_len(delim);
	expanded = malloc(len + 1);
	if (!expanded)
		free_shell(MALLOC_ERROR, STDERR_FILENO, NULL, NULL);
	ft_memset(expanded, 0, len + 1);
	ft_memset(&ex, 0, sizeof(t_str_ex));
	while (delim[ex.i])
	{
		if ((delim[ex.i] == QUOTE && ex.dquotes) || (delim[ex.i] == DQUOTE
				&& ex.quotes))
			ft_strlcat(expanded, delim + ex.i, ft_strlen(expanded) + 2);
		else if (delim[ex.i] == QUOTE && !ex.dquotes)
			ex.quotes = !ex.quotes;
		else if (delim[ex.i] == DQUOTE && !ex.quotes)
			ex.dquotes = !ex.dquotes;
		else
			ft_strlcat(expanded, delim + ex.i, ft_strlen(expanded) + 2);
		ex.i++;
	}
	expanded[len] = '\0';
	return (expanded);
}

int	sanitize_hd_delim_len(char *delim)
{
	t_str_ex	ex;

	ft_memset(&ex, 0, sizeof(t_str_ex));
	while (delim[ex.i])
	{
		if ((delim[ex.i] == QUOTE && ex.quotes && !ex.dquotes)
			|| (delim[ex.i] == DQUOTE && ex.dquotes && !ex.quotes))
			ex.len -= 2;
		if (delim[ex.i] == QUOTE && !ex.dquotes)
			ex.quotes = !ex.quotes;
		else if (delim[ex.i] == DQUOTE && !ex.quotes)
			ex.dquotes = !ex.dquotes;
		ex.len++;
		ex.i++;
	}
	return (ex.len);
}
