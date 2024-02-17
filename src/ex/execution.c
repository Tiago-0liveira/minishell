/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 22:27:57 by joaoribe          #+#    #+#             */
/*   Updated: 2024/02/17 00:26:04 by joaoribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	*ft_lstlast_mini(t_command *lst)
{
	if (!lst)
		return (0);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

int	ft_atoi_exit(const char *nptr)
{
	int		r;
	int		s;
	char	*d;

	d = (char *) nptr;
	r = 0;
	s = 1;
	if (!nptr)
		return (0);
	while (*d == ' ' || *d == '\n' || *d == '\t' || *d == '\v'
		|| *d == '\f' || *d == '\r')
		d++;
	if (*d == '-' || *d == '+')
	{
		if (*d == '-')
			s = -1;
		d++;
	}
	while (*d >= '0' && *d <= '9')
	{
		r = r * 10 + *d - '0';
		d++;
	}
	return (r * s);
}

void    ft_execution(t_mini *mini, char **ev)
{
	pid_t		pid;
	char		*l_cmd;

	// sinal caso processo seja interrompido a meio
	pid = 0;
	l_cmd = ft_lstlast_mini(mini->commands)->cmd_name;
	if (mini->commands->cmd_name)
	{
		if (!(mini->commands->next) && !ft_strncmp(mini->commands->cmd_name,
			"exit", 4))
			free_shell(mini, NULL, ft_atoi_exit(mini->commands->args[0])); // executar built-in direto se for so 1 comando exit
		else
		{
			while (mini->commands)
			{
				if (pipe(mini->input.pip) < 0)
					free_shell(mini, "Error\nPipe failure!\n", 1);
				pid = fork();
				if (pid < 0)
					free_shell(mini, "Error\nFork failure!\n", 1);
				if (!pid)
					child_process(mini, mini->commands, l_cmd, ev);
				else
					parent_process(mini, mini->commands, l_cmd);
				mini->commands = mini->commands->next;
			}
		}
	}
	// tratar sinais
}

void	child_process(t_mini *mini, t_command *cmd, char *l_cmd, char **ev)
{
	// sinal caso o pipe quebre
	close(mini->input.pip[0]);
	if (!ft_strncmp(mini->commands->cmd_name, cmd->cmd_name, ft_strlen(mini->commands->cmd_name)))
	{
		dup2(mini->input.cmd_input, STDIN_FILENO);
		close(mini->input.cmd_input);
	}
	if (!ft_strncmp(mini->commands->cmd_name, l_cmd, ft_strlen(mini->commands->cmd_name)))
		dup2(mini->input.pip[1], STDOUT_FILENO);
	if (cmd->in.type == RED_IN)
	{
		cmd->in.fd = open(cmd->in.file, O_RDONLY);
		if (!(cmd->in.fd))
			free_shell(mini, "Error\nFile open failed!\n", 1); //checar o que deve acontecer se ficheiro nao existir
		dup2(cmd->in.fd, STDIN_FILENO);
		close(cmd->in.fd);
	}
	if (cmd->out.type == RED_OUT || cmd->out.type == RED_AOUT)
	{
		if (cmd->out.type == RED_OUT)
			cmd->out.fd = open(cmd->out.file, O_CREAT | O_RDWR | O_TRUNC);
		else if (cmd->out.type == RED_AOUT)
			cmd->out.fd = open(cmd->out.file, O_CREAT | O_RDWR | O_APPEND);
		if (!(cmd->in.fd))
			free_shell(mini, "Error\nFile open failed!\n", 1);
		dup2(cmd->in.fd, STDOUT_FILENO);
		close(cmd->in.fd);
	}
	close(mini->input.pip[1]);
	execution(mini, cmd, ev);
}

void	parent_process(t_mini *mini, t_command *cmd, char *l_cmd)
{
	wait(0);
	if (ft_strncmp(mini->commands->cmd_name, cmd->cmd_name, ft_strlen(mini->commands->cmd_name)))
		close(mini->input.cmd_input);
	if (!ft_strncmp(mini->commands->cmd_name, l_cmd, ft_strlen(mini->commands->cmd_name)))
		mini->input.cmd_input = mini->input.pip[0];
	else
		close(mini->input.pip[0]);
	close(mini->input.pip[1]);
	// sigint??
}
