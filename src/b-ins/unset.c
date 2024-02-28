/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 19:38:29 by joaoribe          #+#    #+#             */
/*   Updated: 2024/02/26 19:38:29 by joaoribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	bi_unset(t_mini *mini, char **av)
{
	int		i;
	int		res;

	i = -1;
	while (av[++i])
	{
		res = valid_env_var_name(av[i], false);
		if (res == -1)
			return (error_msg_ret(NOT_VALID_IDENT, av[i], 1));
		delete_var(&mini->env_list, av[i]);
	}
	mini->command_ret = 0;
}
