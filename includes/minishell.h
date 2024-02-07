/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 20:27:57 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/02/07 17:26:43 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdbool.h>

# define PROMPT " > "
# define HEREDOC_PROMPT "heredoc>"

# define QUOTE '\''
# define DQUOTE '\"'
# define PIPE '|'

# define REDIR_IN '<'
# define REDIR_APPEND_IN "<<"
# define REDIR_OUT '>'
# define REDIR_APPEND_OUT ">>"

typedef enum _e_redir_type
{
	RED_IN,
	RED_AIN,
	RED_OUT,
	RED_AOUT
} e_redir_type;

typedef struct s_input
{
	char	*raw_line;
	size_t	len;
	size_t	pipe_c;
} t_input;

typedef struct s_redir
{
	char			*file;
	e_redir_type	type;
} t_redir;

typedef struct s_redirs
{
	t_redir		*redirs;
	size_t		redir_c;
} t_redirs;

typedef struct s_command
{
	char				*cmd_name;
	char				*raw_command;
	char					**args;
	t_redirs			redirs;
	struct s_command	*next;
} t_command;

typedef struct s_mini
{
	t_input		input;
	t_command	*commands;
	bool		command_ret;
} t_mini;

// main.c
int	main(int ac, char **av, char **env);

// input.c
char		*get_input(bool prompt);
void		display_prompt(void);

// mini.c
void		init_mini(t_mini *mini);
t_mini		*mini(void);

// utils.c
bool		is_redir(char *line);

// pl
//  \ lexer.c
bool		input_error_check(t_mini *mini);
bool		skip_spaces(char **line);
//  \ parser.c
bool		parse_input(t_mini *mini);
void		command_add_back(t_command *command, t_command *new_command);
t_command	*parse_command(char *raw_command);
void		command_add_redir(t_command *command, char *redir);
//  \ parser_helpers.c
char		**split_by_redirs(char *line);
size_t		count_redirs(char *line);
char		**split_by_pipes(char *line);
char		*find_next_redir(char *line);
char		*find_next_pipe(char *line);
#endif