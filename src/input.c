/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 14:36:06 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/02/23 16:54:33 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_input(bool prompt)
{
	char	*line;
	char	*tmp;

	if (prompt)
		display_prompt();
#if defined(DEBUG)
	printf("-------------------------------------------\n");
#endif
	line = readline(mini()->output);
	if (*line && line)
		add_history(line);
	tmp = ft_strtrim(line, " \t\n");
	if (!tmp)
		free_shell(MALLOC_ERROR, STDERR_FILENO, NULL, NULL);
	free(line);
	line = tmp;
#if defined(DEBUG) || !defined(DEBUGG)
	printf("%s", line);
#endif
	if (!line)
	{
		ft_putendl_fd("!!!detected invalid input, will exit!", STDOUT_FILENO);
		line = ft_strdup("exit");
	}
	return (line);
}

void	display_prompt(void)
{
	char	dir[PATH_MAX + 1];
	char	*color;
	char	*chr;
	char	*tmp;

	color = RED;
	chr = X;
	if (mini()->command_ret == 0)
	{
		color = GREEN;
		chr = CHECK;
	}
	mini()->command_ret = 0;
	if (getcwd(dir, PATH_MAX))
	{
		// bug in norminette
		// mini()->output = ft_strnjoin(6, color, chr, CYAN, dir, RESET, PROMPT);
		// for some reason the above line is invalid
		tmp = ft_strnjoin(6, color, chr, CYAN " ", dir, RESET, PROMPT " ");
		mini()->output = tmp;
	}
}
