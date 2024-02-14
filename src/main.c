/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 20:28:37 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/02/14 19:58:31 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int ac, char **av, char **env)
{
	t_command	*cmd;

	// char	**s;
	(void)av;
	(void)ac;
	(void)env;
	/* TODO: Load env? */
	cmd = NULL;
	init_mini(mini());
	while (1)
	{
		mini()->input.raw_line = get_input(true);
		if (!input_error_check(mini()))
		{
			mini()->command_ret = false;
			printf("minishell: syntax error\n");
		}
		else
		{
			parse_input(mini());
			cmd = mini()->commands;
			while (cmd->next != NULL)
			{
				print_command(cmd->next);
				cmd = cmd->next;
				if (cmd->next != NULL)
					printf("will pipe to: \n");
			}
		}
		reset_mini(mini());
	}
	free_mini(mini());
	return (0);
}
