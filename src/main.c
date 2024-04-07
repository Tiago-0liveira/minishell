/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 20:28:37 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/04/07 13:42:50 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal = 0;

int	main(int ac, char **av, char **env)
{
	(void)av;
	(void)ac;
	init_main(env);
	while (1)
	{
		sig_init();
		mini()->input.raw_line = get_input();
		if (mini()->input.raw_line && ft_strlen(mini()->input.raw_line) == 0)
		{
			free(mini()->input.raw_line);
			continue ;
		}
		else if (mini()->input.raw_line && input_error_check(mini())
			&& syntax_check(mini()->input.raw_line) && !mini()->solo_pipe
			&& command_parser(mini()->input.raw_line)
			&& handle_heredocs(mini()->commands))
		{
			ft_execution(mini(), env);
			add_history(mini()->input.raw_line);
		}
		reset_mini(mini());
	}
	free_shell(NULL, 0, NULL, NULL);
}

void	init_main(char **env)
{
	char	*tmp;
	char	dir[PATH_MAX + 1];
	int		shlvl;

	rl_catch_signals = 0;
	ft_memset(mini(), 0, sizeof(t_mini));
	mini()->env_list = set_env(env);
	delete_if_needed(&mini()->env_list, "PWD=", 3);
	if (!getcwd(dir, PATH_MAX))
		free_shell(FAILURE_GETTING_PATH, EXIT_FAILURE, NULL, NULL);
	tmp = ft_strnjoin(3, "PWD=", dir, "");
	export_add(mini(), tmp);
	tmp = get_env_var(mini()->env_list, "SHLVL");
	if (tmp)
	{
		shlvl = ft_atoi(tmp);
		shlvl++;
	}
	else
		shlvl = 1;
	delete_if_needed(&mini()->env_list, "SHLVL=", 5);
	tmp = ft_itoa(shlvl);
	tmp2 = ft_strnjoin(3, "SHLVL=", tmp, "");
}
