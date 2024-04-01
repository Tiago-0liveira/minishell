/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 02:42:06 by joaoribe          #+#    #+#             */
/*   Updated: 2024/04/01 14:21:27 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	prmpt_ctrlc(int sig)
{
	g_signal = sig;
	mini()->command_ret = SIG_BASE_RET + sig;
	if (!mini()->input.inputting)
		return ;
	if (mini()->solo_pipe)
		write(1, "\n", 1);
	else
		write(1, "C^\n", 3);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	sig_quit_handler(int sig)
{
	(void)sig;
	if (mini()->input.inputting)
		return ;
	ft_putstr_fd("\nQuit (core dumped)\n", STDERR_FILENO);
}

void	sig_init(void)
{
	signal(SIGINT, prmpt_ctrlc);
	signal(SIGQUIT, sig_quit_handler);
	signal(SIGTSTP, SIG_IGN);
}

void	solo_pipe_sigint_handler(int sig)
{
	g_signal = sig;
	if (sig != SIGINT)
		return ;
	write(mini()->input.stdin_cpy, "\n", 1);
	dup2(mini()->input.stdin_cpy, STDIN_FILENO);
	close(mini()->input.stdin_cpy);
	mini()->input.stdin_cpy = -1;
}
