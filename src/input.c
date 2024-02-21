/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 14:36:06 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/02/21 14:43:56 by tiagoliv         ###   ########.fr       */
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
#ifdef DEBUG
	printf("-------------------------------------------\n");
#endif
	line = get_next_line(STDIN_FILENO);
#ifdef DEBUG
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
	/* TODO: display last program success */
	printf("cmd_ret: %d\n", mini()->command_ret);
	if (mini()->command_ret == 0)
		ft_putstr_fd(GREEN(SHELL_SUCCESS_UNICODE), STDOUT_FILENO);
	else
		ft_putstr_fd(RED(SHELL_ERROR_UNICODE), STDOUT_FILENO);
	mini()->command_ret = 0;
	/* TODO: display pwd */
	// ft_putstr_fd(PWD, STDOUT_FILENO);
	ft_putstr_fd(PROMPT, STDOUT_FILENO);
}
