/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 22:27:57 by joaoribe          #+#    #+#             */
/*   Updated: 2024/02/10 05:36:47 by joaoribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void    ft_execution(t_mini *mini)
{
	int		i;
	pid_t	pid;

	i = -1;
	// sinal caso processo seja interrompido a meio
	while (++i <= mini->commands->pipes)
	{
		if (pipe(mini->commands->pipe) < 0)
			free_shell(mini, "Error\nPipe failure!\n", 1);
		pid = fork();
		if (pid < 0)
			free_shell(mini, "Error\nFork failure!\n", 1);
		if (!pid)
			child_process(mini);
		else
			parent_process(mini);
	}
}

void	child_process(t_mini *mini)
{
	
	
	// sinal caso o pipe quebre
	
}