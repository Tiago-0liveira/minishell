/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 20:27:57 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/02/14 19:24:39 by tiagoliv         ###   ########.fr       */
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
	int					fd;
	enum e_redir_type	type;
}						t_redir;

typedef struct s_command
{
	char				*cmd_name;
	char				**args;
	t_redir				in;
	t_redir				out;
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
void					reset_mini(t_mini *mini);

// utils.c
enum e_redir_type		redir_type(char *line);
void					free_commands(t_command *commands);
void					t_redir_init(t_redir *redir);
void					print_command(t_command *command);
// pl
//  \ lexer.c
bool					input_error_check(t_mini *mini);
bool					skip_spaces(char **line);
//  \ parser.c
bool					parse_input(t_mini *mini);
size_t					parse_size(char *line);
char					*get_next_section(char **line);
char					**parse(t_mini *mini);
t_command				*construct_command(char **raw_commands, size_t end);
//  \ parser_helpers.c
size_t					redir_size(char *line);
bool					should_split(char *line);
void					command_add_back(t_command **command,
							t_command *new_command);
void					assign_redir(t_command *command, char *redir_file,
							enum e_redir_type type);
void					assign_args(t_command *command, char **raw_commands,
							size_t *i, size_t end);
// free.c
void					free_commands(t_command *commands);
void					free_list(char **list);
void					free_mini(t_mini *mini);

#endif
