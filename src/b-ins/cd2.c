/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd2.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 03:14:55 by joaoribe          #+#    #+#             */
/*   Updated: 2024/04/04 14:51:34 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	path_with_dots_2(char **pths, char *oldpwd, int *j, bool can_cd)
{
	int		i;
	char	*t_oldpwd;

	t_oldpwd = ft_strdup_oldpwd(oldpwd, &i);
	while (pths[++(*j)])
	{
		getcwd(t_oldpwd, PATH_MAX);
		if (access(pths[*j], F_OK | R_OK) == -1)
		{
			clean_after_access(oldpwd, pths, t_oldpwd, &i);
			error_msg_ret(FD_NOT_FOUND,
				mini()->commands->args[1], EXIT_FAILURE);
			return (0);
		}
		if (!is_dir(pths[*j]))
		{
			clean_after_access(oldpwd, pths, t_oldpwd, &i);
			error_msg_ret(NOT_A_DIR,
				mini()->commands->args[1], EXIT_FAILURE);
			return (0);
		}
		non_dot_chdir(pths, oldpwd, j, can_cd);
	}
	free(t_oldpwd);
	return (i);
}

void	non_dot_chdir(char **pths, char *oldpwd, int *j, int p)
{
	if (!p)
		chdir(pths[*j]);
	env_update(mini(), oldpwd);
}

void	clean_after_access(char *oldpwd, char **pths, char *t_oldpwd, int *i)
{
	*i = 1;
	chdir(oldpwd);
	free_list(pths);
	free(t_oldpwd);
}

bool	is_dir(const char *path)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) == 0)
		return (S_ISDIR(path_stat.st_mode));
	return (false);
}
