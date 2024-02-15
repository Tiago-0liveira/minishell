/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 14:44:32 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/02/15 14:45:47 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_mini(t_mini *mini)
{
	mini->command_ret = true;
	reset_mini(mini);
}

t_mini	*mini(void)
{
	static t_mini	mini;

	return (&mini);
}

void	reset_mini(t_mini *mini)
{
	if (mini->input.raw_line)
		free(mini->input.raw_line);
	mini->input.raw_line = NULL;
	mini->input.len = 0;
	mini->input.pipe_c = 0;
	if (mini->commands)
		free_commands(mini->commands);
	mini->commands = NULL;
}

t_list	*set_env(char **env)
{
	int		i;
	t_list	*env_list;

	i = -1;
	env_list = malloc(sizeof(t_list));
	if (!env_list)
		return (NULL);
	while (env[++i])
		ft_lstadd_back(&env_list, ft_lstnew(ft_strdup(env[i])));
	return (env_list);
}
