/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 03:56:01 by joaoribe          #+#    #+#             */
/*   Updated: 2024/03/02 22:57:51 by joaoribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	if_builtin(char *s)
{
	if (!ft_strncmp(s, "cd", 3) || !ft_strncmp(s, "echo", 5)
		|| !ft_strncmp(s, "env", 4) || !ft_strncmp(s, "exit", 5)
		|| !ft_strncmp(s, "export", 7) || !ft_strncmp(s, "pwd", 5)
		|| !ft_strncmp(s, "unset", 6) || !ft_strncmp(s, "exit", 5))
		return (1);
	return (0);
}

void	built_in(t_mini *mini, t_command *cmd)
{
	if (!ft_strncmp(cmd->cmd_name, "cd", 2))
		bi_cd(mini, cmd->args);
	else if (!ft_strncmp(cmd->cmd_name, "echo", 4))
		bi_echo(cmd->args);
	else if (!ft_strncmp(cmd->cmd_name, "env", 3))
		bi_env(mini->env_list);
	else if (!ft_strncmp(cmd->cmd_name, "export", 6))
		bi_export(mini, cmd->args);
	else if (!ft_strncmp(cmd->cmd_name, "pwd", 3))
		bi_pwd();
	else if (!ft_strncmp(cmd->cmd_name, "unset", 5))
		bi_unset(mini, cmd->args);
	else if (!ft_strncmp(cmd->cmd_name, "exit", 4))
		bi_exit(mini, cmd->args, cmd->next != NULL);
}
