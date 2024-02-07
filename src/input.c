/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 14:36:06 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/02/07 18:06:15 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *get_input(bool prompt)
{
	if (prompt)
		display_prompt();
	return (get_next_line(STDIN_FILENO));
}

void	display_prompt(void)
{
	/* TODO: display last program success */
	/* TODO: display pwd */
	ft_putstr_fd(PROMPT, STDOUT_FILENO);
}