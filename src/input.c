/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 14:36:06 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/03/29 00:11:07 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#ifdef DETAILED_INPUT_PROMPT

char	*get_input(void)
{
	char	*line;
	char	*tmp;

	update_prompt();
	if (mini()->solo_pipe)
		return (read_input());
	mini()->input.inputting = true;
	line = readline(mini()->output);
	mini()->input.inputting = false;
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
#else

char	*get_input(void)
{
	char	*line;
	char	*tmp;

	if (mini()->solo_pipe)
		return (read_input());
	mini()->input.inputting = true;
	line = readline(MINISHELL_BASIC_PROMPT);
	mini()->input.inputting = false;
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

#endif

char	*read_input(void)
{
	char	*tmp;
	char	*tmp2;
	int		fds[2];

	prepare_for_input(fds, solo_pipe_sigint_handler, mini()->output);
	mini()->input.inputting = true;
	tmp = get_next_line(mini()->input.stdin_cpy);
	mini()->input.inputting = false;
	if (mini()->input.stdin_cpy != -1)
	{
		dup2(mini()->input.stdin_cpy, STDIN_FILENO);
		close(mini()->input.stdin_cpy);
		mini()->input.stdin_cpy = -1;
		close(fds[0]);
	}
	if (!tmp)
		return (solo_pipe_read_input_error());
	if (!mini()->input.raw_line)
		return (tmp);
	tmp2 = ft_strtrim(tmp, " \t\n");
	if (!tmp2)
		free_shell(MALLOC_ERROR, STDERR_FILENO, free, tmp);
	free(tmp);
	return (tmp = tmp2, tmp2 = ft_strnjoin(3, mini()->input.raw_line, " ", tmp),
		free(tmp), free(mini()->input.raw_line), tmp2);
}

char	*solo_pipe_read_input_error(void)
{
	mini()->solo_pipe = 0;
	free(mini()->input.raw_line);
	if (g_signal == SIGINT)
		return (NULL);
	error_msg_ret(UNEXPECTED_EOF, NULL, SIGINT);
	printf("exit");
	mini()->input.pipe_c = 0;
	return (ft_strdup("exit"));
}
