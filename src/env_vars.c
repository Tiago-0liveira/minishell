/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_vars.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 15:58:12 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/04/07 16:13:47 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_PWD(t_list **env_list)
{
	char	dir[PATH_MAX + 1];
	char	*tmp;

	delete_if_needed(env_list, "PWD=", 3);
	if (!getcwd(dir, PATH_MAX))
		free_shell(FAILURE_GETTING_PATH, EXIT_FAILURE, NULL, NULL);
	tmp = ft_strnjoin(3, "PWD=", dir, "");
	if (!tmp)
		free_shell(MALLOC_ERROR, EXIT_FAILURE, NULL, NULL);
	export_add(*env_list, tmp);
	free(tmp);	
}

void	set_SHLVL(t_list **env_list)
{
	char	*tmp;
	char	*tmp2;
	int		shlvl;

	tmp = get_env_var(*env_list, "SHLVL");
	delete_if_needed(env_list, "SHLVL=", 5);
	if (tmp)
		shlvl = ft_atoi(tmp) + 1;
	else
		shlvl = 1;
	tmp2 = ft_itoa(shlvl);
	if (!tmp2)
		free_shell(MALLOC_ERROR, EXIT_FAILURE, NULL, NULL);
	free(tmp);
	tmp = ft_strjoin("SHLVL=", tmp2);
	free(tmp2);
	if (!tmp)
		free_shell(MALLOC_ERROR, EXIT_FAILURE, NULL, NULL);
	export_add(*env_list, tmp);
	free(tmp);
}

char	**get_env_from_ll(t_list *env_list)
{
	int		i;
	char	**env;
	t_list	*tmp;

	i = 0;
	tmp = env_list;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	env = malloc(sizeof(char *) * (i + 1));
	if (!env)
		free_shell(MALLOC_ERROR, EXIT_FAILURE, NULL, NULL);
	env[i] = NULL;
	tmp = env_list;
	i = 0;
	while (tmp)
	{
		env[i] = tmp->content;
		i++;
		tmp = tmp->next;
	}
	return env;
}
