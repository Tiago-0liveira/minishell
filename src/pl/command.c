/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 19:38:18 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/03/28 02:48:40 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	command_parser(char *input)
{
	t_str_ex	ex;
	int			j;
	t_command	*cmd;

	memset(&ex, 0, sizeof(t_str_ex));
	j = 0;
	while (1)
	{
		if (input[ex.i] == QUOTE && !ex.dquotes)
			ex.quotes = !ex.quotes;
		else if (input[ex.i] == DQUOTE && !ex.quotes)
			ex.dquotes = !ex.dquotes;
		else if ((input[ex.i] == PIPE || !input[ex.i])
			&& !ex.quotes && !ex.dquotes)
		{
			cmd = init_command(input + j, ex.i - j);
			command_add_back(cmd);
			if (!input[ex.i])
				break ;
			j = ++ex.i;
			continue ;
		}
		ex.i++;
	}
	return (true);
}

t_command	*init_command(char *input, int len)
{
	t_command	*command;
	char		*tmp;

	command = malloc(sizeof(t_command));
	if (!command)
		free_shell(MALLOC_ERROR, STDERR_FILENO, NULL, NULL);
	command->cmd_name = NULL;
	tmp = ft_substr(input, 0, len);
	if (!tmp)
		free_shell(MALLOC_ERROR, STDERR_FILENO, NULL, NULL);
	command->raw_cmd = ft_strtrim(tmp, " ");
	free(tmp);
	if (!command->raw_cmd)
		free_shell(MALLOC_ERROR, STDERR_FILENO, NULL, NULL);
	command->args = NULL;
	command->redirs = NULL;
	command->expanded = false;
	command->docs = init_docs(command->raw_cmd);
	command->next = NULL;
	return (command);
}

t_doc	*init_docs(char *input)
{
	t_doc	*docs;
	char	**raw_commands;
	char	*tmp;
	char	*tmp2;
	int		i;

	raw_commands = parse(input);
	docs = NULL;
	i = 0;
	while (raw_commands && raw_commands[i])
	{
		if (redir_type(raw_commands[i]) == RED_AIN && raw_commands[i + 1])
		{
			tmp = sanitize_hd_delim(raw_commands[i + 1]);
			tmp2 = heredoc(mini(), tmp);
			doc_add_back(&docs, tmp2);
			free(tmp);
		}
		i++;
	}
	return (free_list(raw_commands), docs);
}

void	doc_add_back(t_doc **docs, char *new_doc_file)
{
	t_doc	*tmp;

	if (!*docs)
	{
		*docs = malloc(sizeof(t_doc));
		if (!*docs)
			free_shell(MALLOC_ERROR, STDERR_FILENO, NULL, NULL);
		(*docs)->doc = new_doc_file;
		(*docs)->next = NULL;
		return ;
	}
	tmp = *docs;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = malloc(sizeof(t_doc));
	if (!tmp->next)
		free_shell(MALLOC_ERROR, STDERR_FILENO, NULL, NULL);
	tmp->next->doc = new_doc_file;
	tmp->next->next = NULL;
}

bool	build_command(t_command *cmd)
{
	char	**raw_commands;
	char	*expanded;
	size_t	i;
	size_t	end;

	i = 0;
	expanded = str_expander_hd2(cmd->raw_cmd);
	cmd->expanded = true;
	if (!expanded)
		return (cmd->cmd_name = ft_strdup(""), false);
	raw_commands = parse(expanded);
	end = parse_size(expanded);
	free(expanded);
	i = 0;
	while (i < end)
	{
		if (!update_command(cmd, raw_commands, &i, end))
			return (free(cmd), false);
		i++;
	}
	if (cmd->args == NULL)
		cmd->cmd_name = "";
	else
		cmd->cmd_name = cmd->args[0];
	return (free_list(raw_commands), true);
}
