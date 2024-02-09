/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 20:27:57 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/02/09 16:51:49 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <stdbool.h>
# include <stdio.h>

# define PROMPT " > "
# define HEREDOC_PROMPT "heredoc>"

# define QUOTE '\''
# define DQUOTE '\"'
# define PIPE '|'

# define REDIR_IN '<'
# define REDIR_APPEND_IN "<<"
# define REDIR_OUT '>'
# define REDIR_APPEND_OUT ">>"

# define DEBUG

enum					e_redir_type
{
	RED_NULL = -1,
	RED_IN,
	RED_AIN,
	RED_OUT,
	RED_AOUT
};

typedef struct s_input
{
	char				*raw_line;
	size_t				len;
	size_t				pipe_c;
}						t_input;

typedef struct s_redir
{
	char				*file;
	enum e_redir_type	type;
}						t_redir;

typedef struct s_redirs
{
	t_redir				*redirs;
	size_t				redir_c;
}						t_redirs;

typedef struct s_command
{
	char				*cmd_name;
	char				*raw_command;
	char				**args;
	t_redirs			redirs;
	struct s_command	*next;
}						t_command;

typedef struct s_mini
{
	t_input				input;
	t_command			*commands;
	bool				command_ret;
}						t_mini;

// main.c
int						main(int ac, char **av, char **env);

// input.c
char					*get_input(bool prompt);
void					display_prompt(void);

// mini.c
void					init_mini(t_mini *mini);
t_mini					*mini(void);

// utils.c
bool					is_redir(char *line);
enum e_redir_type		redir_type(char *line);
bool					ft_isspace(char c);
char					*ft_strtrim2(char *s1, char const *set);
// pl
//  \ lexer.c
bool					input_error_check(t_mini *mini);
bool					skip_spaces(char **line);
//  \ parser.c
bool					parse_input(t_mini *mini);
void					command_add_back(t_command **command,
							t_command *new_command);
t_command				*parse_command(char *raw_command);
void					command_add_redir(t_command *command, char *redir);
char					**parse(char *line);
size_t					parse_size(char *line);
bool					should_split(char *line);
char					*get_next_section(char **line);
//  \ parser_helpers.c
size_t					redir_size(char *line);
size_t					count_args(char *line);
char					*get_next_arg(char **line);
#endif