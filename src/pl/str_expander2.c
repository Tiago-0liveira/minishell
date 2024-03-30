/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_expander2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 17:03:17 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/03/30 00:07:42 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	expand_vars_hd2(char *str, char *expanded, int len)
{
	t_str_ex	ex;

	ft_memset(&ex, 0, sizeof(t_str_ex));
	while (str[ex.i])
	{
		if (str[ex.i] == QUOTE && !ex.dquotes)
			ex.quotes = !ex.quotes;
		else if (str[ex.i] == DQUOTE && !ex.quotes)
			ex.dquotes = !ex.dquotes;
		if (str[ex.i] == ENV_VAR && (!ex.quotes || (ex.quotes
					&& ex.dquotes)) && str_starts_with_env_var(str + ex.i)
			&& (str[ex.i + 1] != DQUOTE && str[ex.i + 1] != QUOTE))
		{
			ft_strlcat(expanded, str_expander_var_len(&ex, str), len + 1);
			free_assign_null((void **)&ex.var);
			continue ;
		}
		else
			ft_strlcat(expanded, str + ex.i, ft_strlen(expanded) + 2);
		ex.i++;
	}
	expanded[len] = '\0';
}

int	str_expander_len_hd2(char *str)
{
	t_str_ex	ex;

	ft_memset(&ex, 0, sizeof(t_str_ex));
	while (str[ex.i])
	{
		if (str[ex.i] == QUOTE && !ex.dquotes)
			ex.quotes = !ex.quotes;
		else if (str[ex.i] == DQUOTE && !ex.quotes)
			ex.dquotes = !ex.dquotes;
		else if (str[ex.i] == ENV_VAR && (!ex.quotes || (ex.quotes
					&& ex.dquotes)) && str_starts_with_env_var(str + ex.i)
			&& (str[ex.i + 1] != DQUOTE && str[ex.i + 1] != QUOTE))
		{
			str_expander_var_len(&ex, str);
			ex.len += ex.var_clen;
			free_assign_null((void **)&ex.var);
			continue ;
		}
		ex.len++;
		ex.i++;
	}
	return (ex.len);
}

char	*str_expander_hd2(char *str)
{
	char	*expanded;
	int		final_len;

	final_len = str_expander_len_hd2(str);
	if (final_len <= 0)
		return (NULL);
	expanded = malloc(final_len + 1);
	if (!expanded)
		free_shell(MALLOC_ERROR, STDERR_FILENO, NULL, NULL);
	ft_memset(expanded, 0, final_len + 1);
	expand_vars_hd2(str, expanded, final_len);
	return (expanded);
}
