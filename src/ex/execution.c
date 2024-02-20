/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 22:27:57 by joaoribe          #+#    #+#             */
/*   Updated: 2024/02/20 23:25:05 by tiagoliv         ###   ########.fr       */
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

void	ft_execution(t_mini *mini, char **ev)
{
	t_command	*cmd;
	int			has_next;

	cmd = mini->commands;
	while (cmd)
	{
		has_next = (cmd->next != NULL);
		if (has_next)
		{
			if (pipe(mini->input.pip) < 0)
			{
				free_shell(mini, "Error\nPipe failure!\n", 1);
			}
		}
		handle_command(mini, cmd, ev, has_next);
		if (has_next)
		{
			// Update cmd_input for the next command to read from this pipe
			close(mini->input.pip[1]);
			// Close the write-end as it's not needed anymore
			mini->input.cmd_input = mini->input.pip[0];
		}
		cmd = cmd->next;
	}
}

void	handle_command(t_mini *mini, t_command *cmd, char **ev, int has_next)
{
	if (!has_next && !cmd->in.type && !cmd->out.type)
	{
		// Direct execution without forking if it's the last command with no redirections
		execute_direct(mini, cmd, ev);
	}
	else
	{
		// Setup and execute command in a child process
		execute_in_child(mini, cmd, ev, has_next);
	}
}

void	execute_direct(t_mini *mini, t_command *cmd, char **ev)
{
	// Direct execution logic (without forking)
	// This is a placeholder; actual execution logic depends on your shell's capabilities
	execution(mini, cmd, ev);
}

void	execute_in_child(t_mini *mini, t_command *cmd, char **ev, int has_next)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		// Child process
		if (mini->input.cmd_input != STDIN_FILENO)
		{
			dup2(mini->input.cmd_input, STDIN_FILENO);
			// Set up input redirection from previous pipe if necessary
			close(mini->input.cmd_input);
		}
		if (has_next)
		{
			close(mini->input.pip[0]);
			// Close read-end; not needed in child
			dup2(mini->input.pip[1], STDOUT_FILENO);
			// Redirect stdout to pipe's write-end
		}
		setup_redirections(cmd);  // Handle file-based redirections
		execution(mini, cmd, ev); // Execute the command
		exit(EXIT_SUCCESS);       // Ensure child process exits after execution
	}
	else if (pid < 0)
	{
		free_shell(mini, "Error\nFork failure!\n", 1);
	}
	else
	{
		// Parent process
		wait(NULL); // Wait for child to complete
		if (has_next)
		{
			close(mini->input.pip[1]);
			// Close the write-end after child execution
		}
		if (mini->input.cmd_input != STDIN_FILENO)
		{
			close(mini->input.cmd_input);
			// Close the read-end of the previous pipe if it was used
		}
	}
}

void	setup_redirections(t_command *cmd)
{
	int	fd_in;
	int	fd_out;

	if (cmd->in.type != RED_NULL)
	{
		fd_in = open(cmd->in.file, O_RDONLY);
		if (fd_in < 0)
		{
			perror("Error opening input file");
			exit(EXIT_FAILURE);
		}
		dup2(fd_in, STDIN_FILENO);
		close(fd_in);
	}
	if (cmd->out.type != RED_NULL)
	{
		if (cmd->out.type == RED_OUT)
		{
			fd_out = open(cmd->out.file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		}
		else
		{ // RED_AOUT
			fd_out = open(cmd->out.file, O_CREAT | O_WRONLY | O_APPEND, 0644);
		}
		if (fd_out < 0)
		{
			perror("Error opening output file");
			exit(EXIT_FAILURE);
		}
		dup2(fd_out, STDOUT_FILENO);
		close(fd_out);
	}
}

/*void	ft_execution(t_mini *mini, char **ev)
{
	t_command	*tmp;
	pid_t		pid;
	char		*l_cmd;

	// sinal caso processo seja interrompido a meio
	pid = 0;
	tmp = mini->commands;
	l_cmd = ft_lstlast_mini(mini->commands)->cmd_name;
	if (mini->commands->cmd_name)
	{
		if (!(mini->commands->next))
			execution(mini, tmp, ev);
		else
		{
			while (tmp)
			{
				if (pipe(mini->input.pip) < 0)
					free_shell(mini, "Error\nPipe failure!\n", 1);
				pid = fork();
				if (pid < 0)
					free_shell(mini, "Error\nFork failure!\n", 1);
				if (!pid)
					child_process(mini, tmp, l_cmd, ev);
				else
					parent_process(mini, tmp, l_cmd);
				tmp = tmp->next;
			}
		}
	}
	// tratar sinais
}

void	child_process(t_mini *mini, t_command *cmd, char *l_cmd, char **ev)
{
	// sinal caso o pipe quebre
	close(mini->input.pip[0]);
	if (!ft_strncmp(mini->commands->cmd_name, cmd->cmd_name,
			ft_strlen(cmd->cmd_name)))
	{
		dup2(mini->input.cmd_input, STDIN_FILENO);
		close(mini->input.cmd_input);
	}
	if (!ft_strncmp(mini->commands->cmd_name, l_cmd, ft_strlen(cmd->cmd_name)))
		dup2(mini->input.pip[1], STDOUT_FILENO);
	if (cmd->in.type == RED_IN)
	{
		cmd->in.fd = open(cmd->in.file, O_RDONLY);
		if (!(cmd->in.fd))
			free_shell(mini, "Error\nFile open failed!\n", 1);
				// checar o que deve acontecer se ficheiro nao existir
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
	if (ft_strncmp(mini->commands->cmd_name, cmd->cmd_name,
			ft_strlen(cmd->cmd_name)))
		close(mini->input.cmd_input);
	if (!ft_strncmp(mini->commands->cmd_name, l_cmd, ft_strlen(cmd->cmd_name)))
		mini->input.cmd_input = mini->input.pip[0];
	else
		close(mini->input.pip[0]);
	close(mini->input.pip[1]);
	// sigint??
}*/
