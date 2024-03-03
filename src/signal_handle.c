/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 02:42:06 by joaoribe          #+#    #+#             */
/*   Updated: 2024/03/03 18:43:18 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	prmpt_ctrlc(int signal)
{
	(void)signal;
	mini()->command_ret = 130;
	write(1, "prmpt_ctrlc called\n", 18);
	write(1, "^C\n", 3);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	sig_init(void)
{
	signal(SIGINT, prmpt_ctrlc);
	signal(SIGQUIT, SIG_IGN);
}

void	exec_sig(int signal)
{
	if (signal == SIGQUIT)
	{
		ft_putstr_fd("Quit (core dumped)", 2);
		mini()->command_ret = SIGQUIT;
	}
	else if (signal == SIGPIPE || signal == SIGINT)
		mini()->command_ret = signal + SIG_BASE_RET;
}

void	hd_ctrlc(int signal)
{
	(void)signal;
	mini()->command_ret = signal + SIG_BASE_RET;
	dup2(mini()->original_stdin_fd, STDIN_FILENO);
	if (mini()->heredoc_is_running)
		mini()->heredoc_is_running = false;
	close(mini()->original_stdin_fd);
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}
