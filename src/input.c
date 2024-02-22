/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 14:36:06 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/02/22 01:16:18 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_input(bool prompt)
{
	char	*line;
	char	*tmp;

	if (prompt)
		display_prompt();
		// return ("cat aaa> b$ABCbb < tt");
#if defined(DEBUG)
	printf("-------------------------------------------\n");
#endif
	line = get_next_line(STDIN_FILENO);
#if defined(DEBUG) || defined(DEBUGG)
	printf("%s", line);
#endif
	if (!line)
	{
		ft_putendl_fd("!!!detected invalid input, will exit!", STDOUT_FILENO);
		line = ft_strdup("exit");
	}
	tmp = ft_strtrim(line, " \n");
	free(line);
	return (tmp);
}

void	display_prompt(void)
{
	char	dir[PATH_MAX + 1];

	/* TODO: display last program success */
	// printf("cmd_ret: %d\n", mini()->command_ret);
	if (mini()->command_ret == 0)
		color_out(GREEN, CHECK " ");
	else
		color_out(RED, X);
	mini()->command_ret = 0;
	/* TODO: display pwd */
	if (getcwd(dir, PATH_MAX))
		color_out(CYAN, dir);
	ft_putstr_fd(PROMPT, STDOUT_FILENO);
}
