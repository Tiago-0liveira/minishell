/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_expander.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 02:23:27 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/02/26 18:58:54 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*str_expander(char *str)
{
	char	*expanded;
	int		final_len;

	final_len = str_expander_len(str);
	if (final_len <= 0)
		return (NULL);
	expanded = malloc(final_len + 1);
	if (!expanded)
		free_shell(MALLOC_ERROR, STDERR_FILENO, NULL, NULL);
	memset(expanded, 0, final_len + 1);
	expand_vars(str, expanded, final_len);
	return (expanded);
}

void	expand_vars(char *str, char *expanded, int len)
{
	t_str_ex	ex;

	memset(&ex, 0, sizeof(t_str_ex));
	while (str[ex.i])
	{
		if ((str[ex.i] == QUOTE && ex.dquotes)
			|| (str[ex.i] == DQUOTE && ex.quotes))
			ft_strlcat(expanded, str + ex.i, ft_strlen(expanded) + 2);
		if (str[ex.i] == QUOTE)
			ex.quotes = !ex.quotes;
		else if (str[ex.i] == DQUOTE)
			ex.dquotes = !ex.dquotes;
		else if (str[ex.i] == ENV_VAR && (!ex.quotes || (ex.quotes
					&& ex.dquotes)))
		{
			ft_strlcat(expanded, str_expander_var_len(&ex, str), len);
			continue ;
		}
		else
			ft_strlcat(expanded, str + ex.i, ft_strlen(expanded) + 2);
		ex.i++;
	}
	expanded[len] = '\0';
}

int	str_expander_len(char *str)
{
	t_str_ex	ex;

	memset(&ex, 0, sizeof(t_str_ex));
	while (str[ex.i])
	{
		if ((str[ex.i] == QUOTE && ex.quotes && !ex.dquotes)
			|| (str[ex.i] == DQUOTE && ex.dquotes && !ex.quotes))
			ex.len -= 2;
		if (str[ex.i] == QUOTE)
			ex.quotes = !ex.quotes;
		else if (str[ex.i] == DQUOTE)
			ex.dquotes = !ex.dquotes;
		else if (str[ex.i] == ENV_VAR && (!ex.quotes || (ex.quotes
					&& ex.dquotes)))
		{
			str_expander_var_len(&ex, str);
			ex.len += ex.var_clen;
			continue ;
		}
		ex.len++;
		ex.i++;
	}
	return (ex.len);
}

/* @returns - the env variable content */
char	*str_expander_var_len(t_str_ex *ex, char *str)
{
	char	*tmp;
	char	*res;

	if (str[ex->i] == ENV_VAR)
	{
		ex->i++;
		ex->var_len += 1;
	}
	while (str[ex->i] && (valid_env_char(str[ex->i]) || str[ex->i] == ENV_Q))
	{
		ex->var_len++;
		ex->i++;
		if (str[ex->i - 1] == ENV_Q)
			break ;
	}
	if (str[ex->i - 1] != ENV_Q)
	{
		tmp = ft_substr(str, ex->i - ex->var_len, ex->var_len);
		res = get_env_var(mini()->env_list, tmp);
		free(tmp);
	}
	else
		res = get_env_var(mini()->env_list, str + ex->i - ex->var_len - 1);
	ex->var_clen = ft_strlen(res);
	return (res);
}

bool	expand_command(t_command *cmd)
{
	size_t	i;
	char	*expanded;
	t_redir	*redir;

	DEBUG_MSG("\n");
	i = 0;
	while (cmd->args[i])
	{
		if (cmd->args[i] && cmd->args[i][0] == '\0')
		{
			i++;
			continue ;
		}
		expanded = str_expander(cmd->args[i]);
		free(cmd->args[i]);
		cmd->args[i++] = expanded;
	}
	redir = cmd->redirs;
	while (redir)
	{
		if (redir->file && redir->file[0] == '\0')
			continue ;
		expanded = str_expander(redir->file);
		free(redir->file);
		redir->file = expanded;
		redir = redir->next;
	}
	if (cmd->args && cmd->args[0] != NULL)
		cmd->cmd_name = cmd->args[0];
	return (true);
}
