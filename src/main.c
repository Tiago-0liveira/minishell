/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 20:28:37 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/02/17 20:34:51 by tiagoliv         ###   ########.fr       */
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
	mini()->env_list = set_env(env);
	if (!(mini()->env_list))
		free_shell(mini(), "Error\nMalloc failed!\n", 1);
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
			if (mini()->commands != NULL
				&& ft_strncmp(mini()->commands->cmd_name, "exit", 4) != 0)
				break ;
			cmd = mini()->commands;
			while (cmd)
			{
				print_command(cmd);
				cmd = cmd->next;
				if (cmd != NULL)
					printf("will pipe to: \n");
			}
			// ft_execution(mini(), env);
		}
		printf("her\n");
		reset_mini(mini());
	}
	printf("asfasdasdasdadsasdasd\n");
	free_shell(mini(), NULL, 0);
}
