/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 20:18:00 by joaoribe          #+#    #+#             */
/*   Updated: 2024/03/05 20:18:00 by joaoribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_contains_other_than(const char *str, char c)
{
	if (str == NULL)
		return 0;
	while (*str)
	{
		if (*str != c)
			return (1);
		str++;
	}
	return (0);
}

void	bi_echo(char **av)
{
	bool	flag_n;
	int		i;
	char	*tmp;

	i = 1;
	flag_n = false;
	if (av[i])
	{
		while (av[i] && !ft_strncmp(av[i], ECHO_FLAG_N, 2))
		{
			tmp = av[i] + 1;
			if (ft_contains_other_than(tmp, 'n'))
				break ;
			flag_n = true;
			i++;
		}
		while (av[i])
		{
			ft_putstr_fd(av[i], STDOUT_FILENO);
			if (av[i + 1])
				ft_putstr_fd(" ", STDOUT_FILENO);
			i++;
		}
	}
	if (!flag_n)
		ft_putstr_fd("\n", STDOUT_FILENO);
	mini()->command_ret = 0;
}
