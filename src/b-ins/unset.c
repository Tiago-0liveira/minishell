/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 05:01:07 by joaoribe          #+#    #+#             */
/*   Updated: 2024/02/15 05:01:07 by joaoribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	bi_unset(t_mini *mini, char **av)
{
	int		i;
	int		j;
	t_list	*tmp;
	t_list	*exp;

	i = -1;
	j = 0;
	tmp = mini->env_list;
	while (av[++i])
	{
		tmp = mini->env_list;
		while (tmp)
		{
			if (!ft_strncmp(av[i], tmp->content, ft_strlen_eq(tmp->content)))
			{
				j = ft_strlen_eq(av[i]);
				printf("%i\n", j);
				if (!av[i][j])
				{
					delete_var(&(mini->env_list), tmp->content);
					break ;
				}
				else if (av[i][j] == '=')
				{
					delete_var(&(mini->env_list), tmp->content);
					exp = ft_lstnew(ft_strdup((char *)av[i]));
					ft_lstadd_back(&(mini->env_list), exp);
					break ;
				}
			}
			else
			{
				j = ft_strlen_eq(av[i]);
				if (av[i][j] == '=')
				{
					exp = ft_lstnew(ft_strdup((char *)av[i]));
					ft_lstadd_back(&(mini->env_list), exp);
					break ;
				}
			}
			tmp = tmp->next;
		}
		tmp = mini->env_list;
	}
}
