/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 19:38:18 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/03/28 17:29:20 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	command->docs = NULL;
	command->next = NULL;
	return (command);
}

bool	handle_heredocs(t_command *commands)
{
	t_command		*tmp;
	t_doc_parser	*doc_parser;
	t_doc			*doc_tmp;

	doc_parser = NULL;
	tmp = commands;
	while (tmp)
	{
		doc_parser = init_docs(tmp->raw_cmd);
		if (doc_parser->error)
		{
			while (doc_parser->docs)
			{
				doc_tmp = doc_parser->docs;
				doc_parser->docs = doc_tmp->next;
				free(doc_tmp->doc);
				free(doc_tmp);
			}
			return (free(doc_parser), false);
		}
		tmp->docs = doc_parser->docs;
		free(doc_parser);
		tmp = tmp->next;
	}
	return (true);
}

t_doc_parser	*init_docs(char *input)
{
	t_doc_parser	*docs_parser;
	char	**raw_commands;
	char	*tmp;
	int		i;

	raw_commands = parse(input);
	docs_parser = malloc(sizeof(t_doc_parser));
	if (!docs_parser)
		free_shell(MALLOC_ERROR, EXIT_FAILURE, NULL, NULL);
	memset(docs_parser, 0, sizeof(t_doc_parser));
	i = 0;
	while (raw_commands && raw_commands[i])
	{
		if (redir_type(raw_commands[i]) == RED_AIN && raw_commands[i + 1])
		{
			tmp = heredoc(mini(), sanitize_hd_delim(raw_commands[i + 1]));
			doc_add_back(&docs_parser->docs, tmp);
			if (!tmp)
				return (free_list(raw_commands), docs_parser->error = true, docs_parser);
		}
		i++;
	}
	return (free_list(raw_commands), docs_parser);
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
