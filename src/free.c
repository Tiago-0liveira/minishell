/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 18:22:28 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/02/20 22:58:52 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_commands(t_command *commands)
{
	t_command	*tmp;

	while (commands != NULL)
	{
		tmp = commands->next;
		if (commands->cmd_name)
			free(commands->cmd_name);
		if (commands->in.file)
			free(commands->in.file);
		if (commands->out.file)
			free(commands->out.file);
		if (commands->args)
			free_list(commands->args);
		free(commands);
		commands = tmp;
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
		free(list[i]);
		i++;
	}
	free(list);
}

// adicionei este free para os frees que ja tenho na execution. pode se manter os dois
void	free_shell(t_mini *mini, char *err, int status)
{
	printf("freeing shell\n");
	reset_mini(mini);
	if (mini->input.pip)
	{
		close(mini->input.pip[0]);
		close(mini->input.pip[1]);
	}
	if (mini->env_list)
		ft_lstclear(&(mini->env_list), free_content);
	rl_clear_history();
	if (err)
		write(2, err, ft_strlen(err));
	close(0);
	close(1);
	exit(status);
}
