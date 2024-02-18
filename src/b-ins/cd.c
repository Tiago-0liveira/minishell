/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 03:21:49 by joaoribe          #+#    #+#             */
/*   Updated: 2024/02/18 05:15:52 by joaoribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	env_update(t_mini *mini, char *oldpwd)
{
	char	pwd[PATH_MAX + 1];
	char	*f_pwd;
	char	*f_oldpwd;
	char	**exp;

	if (!getcwd(pwd, PATH_MAX))
		free_shell(mini, "Error\nFailure getting path!\n", 1);
	f_pwd = pwd;
	f_oldpwd = oldpwd;
	exp = malloc(4 * sizeof(char *));
	if (!exp)
		free_shell(mini, "Error\nMalloc failure!\n", 1);
	exp[0] = ft_strdup("export");
	exp[1] = ft_strjoin("PWD=", f_pwd);
	exp[2] = ft_strjoin("OLDPWD=", f_oldpwd);
	exp[3] = NULL;
	bi_export(mini, exp);
	free_list(exp);
}

// so aceita 1 argumento.
void	bi_cd(t_mini *mini, char **av)
{
	char	oldpwd[PATH_MAX + 1];
	t_list	*tmp;
	char	*tmp_0;
	char	*tmp_oldpwd;
	char	**split_oldpwd;
	char	**tmp_split;
	char	**pths;
	char	*final_oldpwd;
	int		i;
	int		j;
	int		k;
	int		l;

	i = -1;
	j = -1;
	k = 0;
	l = 0;
	if (!getcwd(oldpwd, PATH_MAX))
		free_shell(mini, "Error\nFailure getting path!\n", 1);
	tmp = mini->env_list;
	if (!av[0] || (av[0][0] == '~' && !av[0][1]))
	{
		while (tmp)
		{
			if (!ft_strncmp(tmp->content, "HOME=", 5))
				break ;
			tmp = tmp->next;
		}
		tmp_0 = (char *)tmp->content;
		if (chdir(tmp_0 + 5))
			free_shell(mini, "Error\nDirectory change failure!\n", 1);
	}
	else
	{
		while (av[0][++i])
		{
			if (av[0][i] == '.')
			{
				if (av[0][i + 1] == '.')
				{
					pths = ft_split(av[0], '/');
					tmp_oldpwd = oldpwd;
					split_oldpwd = ft_split(tmp_oldpwd, '/');
					while (pths[++j])
					{
						if (!getcwd(oldpwd, PATH_MAX))
							free_shell(mini, "Error\nFailure getting path!\n", 1);
						if (pths[j][0] == '.')
						{
							final_oldpwd = ft_strdup("/");
							tmp_split = split_oldpwd;
							while (tmp_split[k])
								k++;
							k--;
							while (l < k)
							{
								final_oldpwd = ft_strjoin(final_oldpwd, tmp_split[l]);
								if (l + 1 != k)
									final_oldpwd = ft_strjoin(final_oldpwd, "/");
								l++;
							}
							printf("%s\n", final_oldpwd);
							if (chdir(final_oldpwd))
								free_shell(mini, "Error\nDirectory change failuree!\n", 1);
							env_update(mini, oldpwd);
							free(final_oldpwd);
						}
						else
						{
							printf("%s\n", pths[j]);
							if (chdir(pths[j]))
								free_shell(mini, "Error\nDirectory change failures!\n", 1);
							env_update(mini, oldpwd);

						}
					}
				}
			}
		}
		if (!l)
		{
			if (chdir(av[0]))
				free_shell(mini, "Error\nDirectory change failurea!\n", 1);
			env_update(mini, oldpwd);
		}
	}
}
