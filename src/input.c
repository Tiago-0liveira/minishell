/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 14:36:06 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/02/25 19:42:08 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_input(bool prompt)
{
	char	*line;
	char	*tmp;

	if (prompt)
		update_prompt();
	line = readline(mini()->output);
	if (line)
	{
		tmp = ft_strtrim(line, " \t\n");
		free(line);
		if (!tmp)
			free_shell(MALLOC_ERROR, STDERR_FILENO, NULL, NULL);
		line = tmp;
		add_history(line);
	}
	else
		line = ft_strdup("exit");
	return (line);
}

void	update_prompt(void)
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
		tmp = ft_strnjoin(6, color, chr, CYAN " ", dir, RESET, PROMPT " ");
		mini()->output = tmp;
	}
}
