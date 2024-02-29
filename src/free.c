/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 18:22:28 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/02/29 04:41:26 by joaoribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_commands(t_command *commands)
{
	t_command	*tmp;

	while (commands != NULL)
	{
		tmp = commands->next;
		if (commands->redirs)
			free_redirs(commands->redirs);
		if (commands->args && !mini()->relative_path)
			free_list(commands->args);
		free(commands);
		commands = tmp;
	}
	mini()->relative_path = 0;
}

void	free_redirs(t_redir *redirs)
{
	t_redir	*tmp;

	while (redirs != NULL)
	{
		tmp = redirs->next;
		if (redirs->file)
			free(redirs->file);
		free(redirs);
		redirs = tmp;
	}
}

void	free_list(char **list)
{
	int	i;

	i = 0;
	if (list == NULL)
		return ;
	while (list[i] != NULL)
	{
		if (list && list[i] && list[i][0])
			free(list[i]);
		i++;
	}
	if (list)
		free(list);
}

// mudei a funcao para podermos dar free de alguma variavel que tenhamos alocado
void	free_shell(char *err, int status, void (*cleanup_func)(void *),
		void *free_arg)
{
	t_mini	*m;

	m = mini();
	if (free_arg != NULL && cleanup_func != NULL)
		cleanup_func(free_arg);
	reset_mini(m);
	if (m->input.pip)
	{
		close(m->input.pip[0]);
		close(m->input.pip[1]);
	}
	if (m->env_list)
		ft_lstclear(&(m->env_list), free);
	rl_clear_history();
	if (err)
		write(STDERR_FILENO, err, ft_strlen(err));
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	exit(status);
}

void	free_commands_wrapper(void *arg)
{
	t_command	*cmd;

	cmd = (t_command *)arg;
	free_commands(cmd);
}
