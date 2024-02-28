/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 20:13:48 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/02/28 21:49:42 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	calculate_exit_code_from_string(const char *number)
{
	int		result;
	int		isnegative;
	size_t	str_size;
	size_t	i;

	result = 0;
	isnegative = number[0] == '-';
	i = isnegative;
	str_size = ft_strlen(number);
	while (i < str_size)
	{
		result = (result * 10 + (number[i] - '0')) % EXIT_STATUS_N;
		i++;
	}
	if (isnegative)
		result = EXIT_FAILURE - result;
	return (result);
}

bool	str_is_num(const char *str)
{
	size_t	i;

	i = 0;
	if (str[i] == '-')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (false);
		i++;
	}
	return (true);
}

void	bi_exit(t_mini *mini, char **args, bool has_next)
{
	int		status;
	bool	valid;

	status = 0;

	if (!args[1])
	{
		if (has_next)
			return ;
		free_shell(NULL, mini->command_ret, NULL, NULL);
	}
	valid = str_is_num(args[1]);
	if (!valid)
	{
		if (has_next)
			return ;
		error_msg(EXIT_NUMERIR_ARG_REQ, args[1]);
		free_shell(NULL, CMD_NUM_ARG_REQ_RET, NULL, NULL);
	}
	else if (args[2])
	{
		error_msg(TOO_MANY_ARGS, NULL);
		if (has_next)
			return ;
		free_shell(NULL, EXIT_FAILURE, NULL, NULL);
	}
	status = calculate_exit_code_from_string(args[1]);
	mini->command_ret = status;
	if (has_next)
		return ;
	free_shell(NULL, status, NULL, NULL);
}
