/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 03:21:49 by joaoribe          #+#    #+#             */
/*   Updated: 2024/03/06 03:26:13 by joaoribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**build_solo_path(char *oldpwd, char *av)
{
	char	**s;
	char	*tmp;

	s = malloc(2 * sizeof(char *));
	if (ft_strncmp(av, "/home", 5))
	{
		tmp = ft_strjoin(oldpwd, "/");
		s[0] = ft_strjoin(tmp, av);
		free(tmp);
	}
	else
		s[0] = ft_strdup(av);
	s[1] = NULL;
	return (s);
}

int	path_with_dots(char *av, char *oldpwd, int p)
{
	int		i;
	int		j;
	char	**pths;

	j = -1;
	pths = NULL;
	if ((ft_strnstr(av, "/", ft_strlen(av)) || av[0] == '.')
		&& ft_strncmp(av, "/home", 5))
		pths = ft_split(av, '/');
	else
		pths = build_solo_path(oldpwd, av);
	if (!ft_strncmp(pths[0], "~", 1))
	{
		free(pths[0]);
		pths[0] = ft_strdup(mini()->home_tmp + 5);
	}
	i = path_with_dots_2(pths, oldpwd, &j, p);
	if (!i)
		return (0);
	free_list(pths);
	if (i == 2)
		return (2);
	return (1);
}

void	good_old_cd(char *av, char *oldpwd, int j, int p)
{
	if (j == 1)
	{
		if (access(av, F_OK | R_OK) == -1)
		{
			error_msg_ret(FD_NOT_FOUND, av, EXIT_FAILURE);
			return ;
		}
		if (!p)
			chdir(av);
		env_update(mini(), oldpwd);
	}
}

int	cd_start(char **av, char *oldpwd)
{
	if (!getcwd(oldpwd, PATH_MAX))
		free_shell(FAILURE_GETTING_PATH, EXIT_FAILURE, NULL, NULL);
	if (av && av[0] && av[1] && av[2])
	{
		error_msg_ret(TOO_MANY_ARGS, "cd", EXIT_FAILURE);
		return (0);
	}
	return (1);
}

void	bi_cd(t_mini *mini, char **av, int p)
{
	char	oldpwd[PATH_MAX + 1];
	int		i;

	i = -1;
	if (!cd_start(av, oldpwd))
		return ;
	if (!av[1] || (av[1][0] == TILDE) || (!ft_strncmp(av[1], "$vari", 5)))
		if (!cd_noarg_tilde(av[1], p, &i))
			return ;
	if (!ft_strncmp(av[1], "/", 2) || !ft_strncmp(av[1], "/$vari", 7))
	{
		chdir("/");
		return ;
	}
	if (i == -1 || (av[1][0] == TILDE && av[1][1]))
	{
		if (!path_with_dots(av[1], oldpwd, p))
			return ;
	}
	mini->command_ret = 0;
}
