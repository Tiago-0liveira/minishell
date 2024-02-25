/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built-ins_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 03:56:01 by joaoribe          #+#    #+#             */
/*   Updated: 2024/02/25 18:25:24 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	if_builtin(char *s)
{
	if (!ft_strncmp(s, "cd", 2) || !ft_strncmp(s, "echo", 4)
		|| !ft_strncmp(s, "env", 3) || !ft_strncmp(s, "exit", 4)
		|| !ft_strncmp(s, "export", 6) || !ft_strncmp(s, "pwd", 3)
		|| !ft_strncmp(s, "unset", 5))
		return (1);
	return (0);
}
