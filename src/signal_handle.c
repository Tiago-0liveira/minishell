/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 02:42:06 by joaoribe          #+#    #+#             */
/*   Updated: 2024/03/27 20:07:24 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	prmpt_ctrlc(int signal)
{
	g_signal = signal;
	write(1, "^C\n", 3);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	solopipe_handler(int signal)
{
	g_signal = signal;
	write(1, "\n", 1);
}

void	sig_init(void)
{
	struct sigaction	sa;

	memset(&sa, 0, sizeof(sa));
	if (mini()->solo_pipe)
		sa.sa_handler = solopipe_handler;
	else
		sa.sa_handler = prmpt_ctrlc;
	sigaction(SIGINT, &sa, NULL);
}

void	exec_sig(int signal)
{
	write(1, "\n", 1);
	if (signal == SIGQUIT)
		ft_putstr_fd("Quit (core dumped)", 2);
}

char	*if_relative_path(char *cmd, bool home_added)
{
	char	*c;

	if (home_added)
		free(cmd);
	c = get_relative_path(cmd);
	return (c);
}
