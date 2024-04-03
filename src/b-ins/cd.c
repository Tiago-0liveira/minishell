/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 03:21:49 by joaoribe          #+#    #+#             */
/*   Updated: 2024/04/03 19:08:01 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**build_solo_path(char *oldpwd, char *av)
{
	char	**s;
	char	*tmp;

	s = malloc(2 * sizeof(char *));
	if (!s)
		free_shell(MALLOC_ERROR, EXIT_FAILURE, NULL, NULL);
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

bool	handle_bin(char **pth, char *oldpwd)
{
	if (!*pth)
	{
		chdir("/");
		env_update(mini(), oldpwd);
		return (false);
	}
	if (!ft_strncmp(*pth, "bin", 4))
	{
		free(*pth);
		*pth = ft_strjoin("/", "bin");
	}
	return (true);
}

int	path_with_dots(char *av, char *oldpwd, bool can_cd)
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
	if (!handle_bin(&pths[0], oldpwd))
		return (free_list(pths), 0);
	if (!ft_strncmp(pths[0], "~", 1))
	{
		free(pths[0]);
		pths[0] = ft_strdup(mini()->home_tmp + 5);
	}
	i = path_with_dots_2(pths, oldpwd, &j, can_cd);
	if (!i)
		return (0);
	free_list(pths);
	if (i == 2)
		return (2);
	return (1);
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

void	bi_cd(t_mini *mini, t_command *cmd)
{
	char	oldpwd[PATH_MAX + 1];
	int		i;
	char	*tmp;

	i = -1;
	if (!cd_start(cmd->args, oldpwd))
		return ;
	if (!cmd->args[1] || (cmd->args[1][0] == TILDE) ||
		!ft_strncmp(cmd->args[1], "--", 3))
	{
		if (!cd_noarg_return(cmd->args[1], cmd->prev || cmd->next, oldpwd))
			return ;
		else
			i = 1;
	}
	if (!ft_strncmp(cmd->args[1], "/", 2))
		return (printf("here1\n"), chdir("/"), env_update(mini, oldpwd));
	if (!ft_strncmp(cmd->args[1], "-", 2))
		return (tmp = get_env_var(mini->env_list, "OLDPWD"), chdir(tmp),
			free(tmp), env_update(mini, oldpwd));
	if (i == -1 || (cmd->args[1][0] == TILDE && cmd->args[1][1]))
		if (!path_with_dots(cmd->args[1], oldpwd, cmd->prev || cmd->next))
			return ;
	mini->command_ret = 0;
}
