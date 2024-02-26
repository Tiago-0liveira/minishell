/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 20:27:57 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/02/26 19:03:13 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <limits.h>
# include <linux/limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <sys/wait.h>
# include <termios.h>

# define PROMPT "$ "
# define HEREDOC_PROMPT "heredoc>"

# define QUOTE '\''
# define DQUOTE '\"'
# define PIPE '|'

# define REDIR_IN '<'
# define REDIR_APPEND_IN "<<"
# define REDIR_OUT '>'
# define REDIR_APPEND_OUT ">>"

# define ESCAPE_CHAR '\\'
# define ENV_VAR '$'
/* Last command exit status */
# define ENV_Q  '?'
# define SPACE ' '

# define MALLOC_ERROR "Malloc failed!\n"

# define CHECK "\xE2\x9C\x93"
# define X "\xE2\x9C\x97"

# define RED "\033[0;31m"
# define GREEN "\033[0;32m"
# define BLUE "\033[0;34m"
# define CYAN "\033[0;36m"
# define RESET "\033[0m"

// error_msg errors
# define SHELL_ERROR "minishell: %s %s\n"
# define CMD_NOT_FOUND "command not found: "
# define FD_NOT_FOUND "no such file or directory: "
# define SYNTAX_ERROR "syntax error near unexpected token `"
# define TOO_MANY_ARGS "too many arguments: "
# define NOT_VALID_IDENT "not a valid identifier: "

// free shell errors
# define FAILURE_GETTING_PATH "Failure getting path!\n"
# define DIR_CHANGE_ERROR "Directory change failure!\n"
# define PIPE_ERROR "Pipe error!\n"
# define FORK_ERROR "Fork error!\n"

# define DEBUGGG

# define DEBUG_MSG(fmt, ...) printf("[%s::%s::%d]:" fmt, __FILE__, __func__, __LINE__, ##__VA_ARGS__)

enum					e_redir_type
{
	RED_NULL = 0,
	RED_IN,
	RED_AIN,
	RED_OUT,
	RED_AOUT
};

typedef void			(*t_cleanup_func)(void *);

typedef struct s_str_ex
{
	char				*res;
	int					len;
	int					i;
	bool				quotes;
	bool				dquotes;
	int					var_len;
	int					var_clen;
}						t_str_ex;

typedef struct s_input
{
	char				*raw_line;
	size_t				len;
	size_t				pipe_c;
	int					pip[2];
	int					cmd_input;
}						t_input;

typedef struct s_redir
{
	char				*file;
	int					fd;
	enum e_redir_type	type;
	struct s_redir		*next;
}						t_redir;

typedef struct s_command
{
	char				*cmd_name;
	char				**args;
	t_redir				*redirs;
	struct s_command	*next;
}						t_command;

typedef struct s_mini
{
	t_input				input;
	t_command			*commands;
	int					command_ret;
	t_list				*env_list;
	char				*hd_limiter;
	char				*output;
	int					hdfd;
}						t_mini;

// main.c
int						main(int ac, char **av, char **env);

// input.c
char					*get_input(bool prompt);
void					update_prompt(void);

// mini.c
void					init_mini(t_mini *mini);
t_mini					*mini(void);
void					reset_mini(t_mini *mini);
t_list					*set_env(char **env);

// utils.c
enum e_redir_type		redir_type(char *line);
bool					valid_env_char(char c);
void					print_command(t_command *command);
// pl
//  \ lexer.c
bool					skip_spaces(char **line);
bool					semantic_checker(char **raw_commands);
bool					valid_arg(char **sections, int *i, char **last_section,
							char **error);
//  \ parser.c
bool					parse_input(t_mini *mini);
size_t					parse_size(char *line);
char					*get_next_section(char **line);
char					**parse(t_mini *mini);
t_command				*construct_command(char **raw_commands,
							size_t end);
//  \ parser_helpers.c
size_t					redir_size(char *line);
void					command_add_back(t_command *new_command);
bool					assign_redir(t_command *command, char *redir_file,
							enum e_redir_type type);
bool					update_command(t_command *command, char **raw_commands,
							size_t *i, size_t end);
bool					add_arg(t_command *command, char *section);
// \ str_expander.c
char					*str_expander(char *str);
void					expand_vars(char *str, char *expanded, int len);
int						str_expander_len(char *str);
char					*str_expander_var_len(t_str_ex *ex, char *str);
bool					expand_command(t_command *cmd);
// free.c
void					free_commands(t_command *commands);
void					free_redirs(t_redir *redirs);
void					free_list(char **list);
void					free_shell(char *err, int status,
							void (*cleanup_func)(void *), void *free_arg);
void					free_commands_wrapper(void *arg);
// signal_handle.c
void					prmpt_ctrlc(int signal);
void					sig_init(void);
// ex
// \ execution.c
void					ft_execution(t_mini *mini, char **ev);
void					execute_in_child(t_mini *mini, t_command *cmd,
							char **ev, int has_next);
void					setup_redirections(t_command *cmd, bool isparent);
// \ execute.c
bool					execution(t_mini *mini, t_command *cmd, char **ev);
char					*get_path(char *cmd, char **ev);
char					*cmd_path(char **ev);
// \ heredoc.c
char					*heredoc(t_mini *mini);
// b-ins
// \ built-ins_utils.c
int						if_builtin(char *s);
void					built_in(t_mini *mini, t_command *cmd);
// \ cd
void					bi_cd(t_mini *mini, char **av);
// \ echo
void					bi_echo(char **av);
// \ env
void					bi_env(t_list *env_list);
char					*get_env_var(t_list *env_list, char *var);
// \ export
int						ft_strlen_eq(char *s);
void					free_content(void *content);
void					delete_var(t_list **head, void *node_to_del);
void					bi_export(t_mini *mini, char **av);
// \ pwd
void					bi_pwd(void);
// \ unset
void					bi_unset(t_mini *mini, char **av);
// errors.c
void					error_msg(char *cmd, char *msg);
// colors.c
void					color_out(char *color, char *str);
#endif
