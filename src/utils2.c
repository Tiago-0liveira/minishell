/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 18:05:11 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/03/29 00:08:26 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	skip_spaces(char **line)
{
	while (**line == ' ')
		(*line)++;
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

bool	has_char_in_set(char *s, char *set)
{
	int	i;

	i = 0;
	while (s && s[i])
	{
		if (ft_strchr(set, s[i]))
			return (true);
		i++;
	}
	return (false);
}

void	command_add_back(t_command *new_command)
{
	t_mini		*mi;
	t_command	*last_command;

	mi = mini();
	if (mi->commands == NULL)
	{
		mi->commands = new_command;
		return ;
	}
	last_command = mi->commands;
	while (last_command->next != NULL)
	{
		last_command = last_command->next;
	}
	last_command->next = new_command;
}

void	prepare_for_input(int fds[2], void (*handler)(int), char *prompt)
{
	if (pipe(fds) == -1)
		free_shell(PIPE_ERROR, STDERR_FILENO, NULL, NULL);
	mini()->input.stdin_cpy = dup(STDIN_FILENO);
	dup2(fds[0], STDIN_FILENO);
	g_signal = 0;
	signal(SIGINT, handler);
	signal(SIGQUIT, handler);
	if (prompt)
		ft_putstr(prompt);
}
