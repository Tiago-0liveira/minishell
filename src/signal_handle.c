/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 02:42:06 by joaoribe          #+#    #+#             */
/*   Updated: 2024/03/29 19:02:18 by tiagoliv         ###   ########.fr       */
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

void	solopipe_handler(int sig, siginfo_t *info, void *ctx)
{
	(void)sig;
	(void)ctx;
	(void)info;
}

void	sig_init(void)
{
	struct sigaction	sa;

	sa.sa_sigaction = solopipe_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO;
	sigaction(SIGCHLD, &sa, NULL);
	signal(SIGINT, prmpt_ctrlc);
	signal(SIGQUIT, SIG_IGN);
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

void	exec_sig(int signal)
{
	g_signal = signal;
	if (signal == SIGPIPE)
	{
		ft_putstr_fd("SIGPIPE\n", STDERR_FILENO);
		return ;
	}
	if (!mini()->input.inputting)
		return ;
	if (signal == SIGQUIT)
		ft_putstr_fd("Quit (core dumped)", STDERR_FILENO);
	write(STDERR_FILENO, "\n", 1);
}
