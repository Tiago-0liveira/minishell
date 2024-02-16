/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 18:22:28 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/02/16 04:55:09 by joaoribe         ###   ########.fr       */
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

// adicionei este free para os frees que ja tenho na execution. pode se manter os dois
void    free_shell(t_mini *mini, char *err, int status)
{
	if (mini->input.raw_line)
		free(mini->input.raw_line);
	if (mini->commands)
        free_commands(mini->commands);
	if (mini->input.pipe_c)
	{
		close(mini->input.pipe_c[0]);
		close(mini->input.pipe_c[1]);
	}
	if (mini->env_list)
		ft_lstclear(&(mini->env_list), free_content);
	free(mini);
	rl_clear_history();
    if (err)
        write(2, err, ft_strlen(err));
    close(0);
    close(1);
    exit(status);
}