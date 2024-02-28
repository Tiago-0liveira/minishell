/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 01:50:53 by joaoribe          #+#    #+#             */
/*   Updated: 2024/02/14 01:50:53 by joaoribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	bi_env(t_list *env_list)
{
	while (env_list)
	{
		printf("%s\n", (char *)env_list->content);
		env_list = env_list->next;
	}
}

char	*get_env_var(t_list *env_list, char *var)
{
	int	i;

	if (!ft_strncmp(var, "?", 1))
		return (ft_itoa(mini()->command_ret));
	i = ft_strlen(var);
	while (env_list)
	{
		if (ft_strncmp((char *)env_list->content, var, i) == 0)
			return (ft_strchr((char *)env_list->content, '=') + 1);
		env_list = env_list->next;
	}
	return ("");
}

int	valid_env_var_name(char *str)
{
	int		namelen;
	int		i;

	namelen = ft_strlen_eq(str);
	if (namelen == 0)
		return (error_msg(NOT_VALID_IDENT, str), -1);
	if (str[namelen] != '=')
		return (0);
	i = 0;
	while (str[i] && str[i] != '=')
	{
		if (!valid_env_char(str[i]))
			return (false);
		i++;
	}
	return (namelen);
}
