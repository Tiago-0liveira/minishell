/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 18:22:28 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/02/14 19:54:24 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_commands(t_command *commands)
{
	t_command	*tmp;

	tmp = commands;
	while (tmp != NULL)
	{
		if (tmp->cmd_name)
			free(tmp->cmd_name);
		if (tmp->in.file)
			free(tmp->in.file);
		if (tmp->out.file)
			free(tmp->out.file);
		if (tmp->args)
			free_list(tmp->args);
		commands = tmp;
		tmp = tmp->next;
		free(commands);
	}
	// free(commands);
}

void	free_list(char **list)
{
	int	i;

	i = 0;
	if (list == NULL)
	{
		return ;
	}
	while (list[i] != NULL)
	{
		if (list[i])
			free(list[i]);
		i++;
	}
	free(list);
}

void	free_mini(t_mini *mini)
{
	reset_mini(mini);
	free(mini);
}
