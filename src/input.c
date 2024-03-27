/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 14:36:06 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/03/27 20:47:40 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_input(bool prompt)
{
	char	*line;
	char	*tmp;

	if (prompt)
		update_prompt();
	if (mini()->solo_pipe)
		return (read_input());
	line = readline(mini()->output);
	if (line && *line)
	{
		tmp = ft_strtrim(line, " \t\n");
		free(line);
		if (!tmp)
			free_shell(MALLOC_ERROR, STDERR_FILENO, NULL, NULL);
		line = tmp;
		tmp = NULL;
	}
	else if (!line)
	{
		printf("exit\n");
		mini()->command_ret = 0;
		line = ft_strdup("exit");
	}
	return (line);
}

char	*read_input(void)
{
	char	*tmp;
	char	*tmp2;
	char	*line;

	write(1, mini()->output, ft_strlen(mini()->output));
	tmp = get_next_line(STDIN_FILENO);
	if (!tmp)
		return (solo_pipe_read_input_error());
	if (!mini()->input.raw_line)
		return (tmp);
	tmp2 = ft_strtrim(tmp, " \t\n");
	line = ft_strnjoin(3, mini()->input.raw_line, " ", tmp2);
	free(tmp);
	free(tmp2);
	free(mini()->input.raw_line);
	tmp = NULL;
	tmp2 = NULL;
	return (line);
}

char	*solo_pipe_read_input_error(void)
{
	mini()->solo_pipe = 0;
	free(mini()->input.raw_line);
	if (g_signal != SIGINT)
	{
		error_msg(UNEXPECTED_EOF, NULL);
		printf("exit");
		mini()->input.pipe_c = 0;
		return (ft_strdup("exit"));
	}
	return (NULL);
}

void	update_prompt(void)
{
	char	dir[PATH_MAX + 1];
	char	*color;
	char	*chr;
	char	*tmp;

	color = RED;
	chr = X;
	if (mini()->output)
	{
		free(mini()->output);
		mini()->output = NULL;
	}
	if (mini()->command_ret == 0)
	{
		color = GREEN;
		chr = CHECK;
	}
	if (getcwd(dir, PATH_MAX) && !mini()->solo_pipe)
		tmp = ft_strnjoin(6, color, chr, CYAN " ", dir, RESET, PROMPT);
	else
		tmp = ft_strdup("> ");
	if (!tmp)
		free_shell(MALLOC_ERROR, STDERR_FILENO, NULL, NULL);
	mini()->output = tmp;
}
