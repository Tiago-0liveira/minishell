/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 20:27:57 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/04/07 13:42:55 by tiagoliv         ###   ########.fr       */
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
# include <errno.h>
# include <termios.h>
# include <dirent.h>
# include <fcntl.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/ioctl.h>
# include <sys/file.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>

# define PROMPT "$ "
# define HEREDOC_PROMPT "heredoc>"

/* \' */
# define QUOTE 39
/* \" */
# define DQUOTE 34
/* | */
# define PIPE 124

/* < */
# define REDIR_IN 60
# define REDIR_APPEND_IN "<<"
/* > */
# define REDIR_OUT 62
# define REDIR_APPEND_OUT ">>"

# define SLASH_STR "/"
/* $ */
# define ENV_VAR 36
/* ~ */
# define TILDE 126

/* Last command exit status
	?
*/
# define ENV_Q 63

# define ECHO_FLAG_N "-n"

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
# define SYNTAX_ERROR "syntax error near unexpected token "
# define TOO_MANY_ARGS "too many arguments "
# define NOT_VALID_IDENT "not a valid identifier: "
# define OPEN_QUOTES_ERROR "open quotes are not supported!"
# define CMD_INSUF_PERMS "Permission denied: "
# define EXIT_NUMERIR_ARG_REQ "exit: numeric argument required: "
# define AMB_REDIR "ambiguous redirect: "
# define UNEXPECTED_EOF "syntax error: unexpected end of file"
# define NOT_A_DIR "cd: Not a directory: "
# define CANNOT_EXECUTE_FILE "cannot execute file: "

// heredoc defines
# define HEREDOC_CTRD_WARNING "warning: here-document at line "

# define CMD_NOT_FOUND_RET 127
# define CMD_INSUF_PERMS_RET 126
# define SIG_BASE_RET 128
# define CMD_NUM_ARG_REQ_RET 2

// free shell errors
# define FAILURE_GETTING_PATH "Failure getting path!\n"
# define DIR_CHANGE_ERROR "Directory change failure!\n"
# define PIPE_ERROR "Pipe error!\n"
# define FORK_ERROR "Fork error!\n"

# define EXIT_STATUS_N 256

//# define DETAILED_INPUT_PROMPT

# ifndef DETAILED_INPUT_PROMPT
#  define MINISHELL_BASIC_PROMPT "$> "
# endif

extern int	g_signal;

enum					e_redir_type
{
	RED_NULL = 0,
	RED_IN,
	RED_AIN,
	RED_OUT,
	RED_AOUT
};

typedef struct s_str_ex
{
	char				*res;
	int					len;
	int					i;
	bool				quotes;
	bool				dquotes;
	int					var_len;
	int					var_clen;
	char				*var;
}						t_str_ex;

typedef struct s_input
{
	char				*raw_line;
	size_t				len;
	size_t				pipe_c;
	int					pip[2];
	int					stdin_cpy;
	int					hd_lines;
	bool				inputting;
}						t_input;

typedef struct s_redir
{
	char				*file;
	int					fd;
	enum e_redir_type	type;
	struct s_redir		*next;
}						t_redir;

typedef struct t_doc
{
	char				*doc;
	struct t_doc		*next;
}						t_doc;

typedef struct t_doc_parser
{
	struct t_doc		*docs;
	bool				error;
}						t_doc_parser;

typedef struct s_command
{
	char				*cmd_name;
	char				*raw_cmd;
	char				**args;
	t_redir				*redirs;
	bool				expanded;
	t_doc				*docs;
	int					fds[2];
	int					stds[2];
	struct s_command	*prev;
	struct s_command	*next;
}						t_command;

typedef struct s_mini
{
	t_input				input;
	t_command			*commands;
	int					command_ret;
	t_list				*env_list;
	char				*output;
	int					hdfd;
	int					exit_unavailability;
	char				*home_tmp;
	int					if_cd;
	int					solo_pipe;
	int					doc_n;
}						t_mini;

// main.c
int						main(int ac, char **av, char **env);
void					init_main(char **env);

// input.c
char					*get_input(void);
# ifdef DETAILED_INPUT_PROMPT

void					update_prompt(void);

# endif

char					*read_input(void);
char					*solo_pipe_read_input_error(void);

// errors.c
void					error_msg(char *cmd, char *msg);
void					error_msg_ret(char *cmd, char *msg, int ret);

// path.c
char					*get_roa_path(char *cmd);
char					*get_envpath_cmd(char *cmd);
char					*path_add_home_prefix(char *cmd);
char					*get_relative_path(char *cmd);
char					*get_cmd_path(char *cmd);
// path_utils.c
bool					is_relative_path(char *cmd);
bool					is_absolute_path(char *cmd);
int						can_access_path(char *path);
int						can_path_to(char *path);
char					*get_env_value(t_list *envs, char *key);
// is located in utils.c (didn't fit)
char					*if_relative_path(char *cmd, bool home_added);
// mini.c
t_mini					*mini(void);
void					reset_mini(t_mini *mini);
t_list					*set_env(char **env);

// utils.c
enum e_redir_type		redir_type(char *line);
bool					valid_env_char(char c);
bool					valid_cmd_arg(char *str);
void					free_assign_null(void **ptr);
void					print_command(t_command *command);//TODO: remove 
// utils2.c
bool					skip_spaces(char **line);
size_t					redir_size(char *line);
bool					has_char_in_set(char *s, char *set);
void					command_add_back(t_command *new_command);
void					prepare_for_input(int fds[2], void (*handler)(int),
							char *prompt);
// pl
//  \ lexer.c
bool					input_error_check(t_mini *mini);
bool					syntax_check(char *input);
bool					semantic_checker(char **raw_commands);
bool					valid_section(char **sections, int *i,
							char **last_section, char **error);
bool					check_ambiguitity(char *file);
//	\ command.c
t_command				*init_command(char *input, int len);
t_doc_parser			*init_docs(char *input);
void					doc_add_back(t_doc **docs, char *new_doc_file);
bool					handle_heredocs(t_command *commands);
//  \ hd_arg_expander.c

//  \ parser.c
size_t					parse_size(char *line);
char					*get_next_section(char **line);
char					**parse(char *input);
bool					command_parser(char *input);
bool					build_command(t_command *cmd);
//  \ parser_helpers.c
bool					assign_redir(t_command *command, char *redir_file,
							enum e_redir_type type);
bool					update_command(t_command *command, char **raw_commands,
							size_t *i, size_t end);
bool					add_arg(t_command *command, char *section);
char					*get_redir(char **line);
char					*get_next_doc_file(t_doc **docs);
// \ str_expander.c
char					*str_expander(char *str);
void					expand_vars(char *str, char *expanded, int len);
int						str_expander_len(char *str);
char					*str_expander_var_len(t_str_ex *ex, char *str);
bool					str_starts_with_env_var(char *str);
// \ str_expander2.c
void					expand_vars_hd2(char *str, char *expanded, int len);
int						str_expander_len_hd2(char *str);
char					*str_expander_hd2(char *str);
// \ str_expander_utils.c
bool					expand_command(t_command *cmd);
void					expand_args(t_command *cmd);
bool					expand_redirs(t_command *cmd);
char					*str_expander_process_res(t_str_ex *ex, char *str);
// free.c
void					free_commands(t_command *commands);
void					free_redirs_and_fds(t_command *command);
void					free_list(char **list);
void					free_shell(char *err, int status,
							void (*cleanup_func)(void *), void *free_arg);
void					free_commands_wrapper(void *arg);
// signal_handle.c
void					prmpt_ctrlc(int sig);
void					sig_init(void);
void					solo_pipe_sigint_handler(int sig);
void					exec_sig(int signal);
// ex
// \ execution.c
void					ft_execution(t_mini *mini, char **ev);
void					execute_in_child(t_command *cmd, char **ev,
							int has_next);
void					execute_in_parent(t_mini *mini, t_command *cmd,
							int has_next);
void					wait_for_children(t_mini *mini);
// \ execution_utils.c
t_command				*ft_lstlast_mini(t_command *lst);
void					fd_error(t_redir *redir, bool isparent);
bool					setup_redirections(t_command *cmd, bool isparent);
void					handle_command_fail(t_command *cmd);
// \ execute.c
bool					execution(t_command *cmd, char **ev);
void					set_execution(t_mini *mini, t_command *cmd, char **ev,
							int has_next);
// \ heredoc.c
bool					heredoc_read_input_to_file(char *delim, char *file);
char					*heredoc_get_new_file(t_mini *mini);
char					*heredoc(t_mini *mini, char *delim);
char					*sanitize_hd_delim(char *delim);
int						sanitize_hd_delim_len(char *delim);
// \ heredoc_utils.c
char					*heredoc_ctrd_error(t_mini *mini, char *delim);
void					input_cleanup(t_mini *mini, int *fd, int fds[2]);
int						heredoc_process_input(char **input, char *delim,
							int fd, int fds[2]);
// \ heredoc_str_expander.c
void					expand_vars_hd(char *str, char *expanded, int len);
int						str_expander_len_hd(char *str);
char					*str_expander_hd(char *str);
char					*expand_input_hd(char *s);
// b-ins
// \ utils.c
int						if_builtin(char *s);
int						if_builtin_epe(char *s);
void					built_in(t_mini *mini, t_command *cmd);
t_list					*sort_list(t_list *lst);
// \ cd.c
void					bi_cd(t_mini *mini, t_command *cmd);
// \ cd2.c
int						path_with_dots_2(char **pths, char *oldpwd, int *j,
							bool can_cd);
void					non_dot_chdir(char **pths, char *oldpwd, int *j, int p);
void					clean_after_access(char *oldpwd, char **pths,
							char *t_oldpwd, int *i);
bool					is_dir(const char *path);
// \ cd3.c
int						cd_noarg_tilde(char *av, bool can_cd, char *oldpwd);
void					env_update(t_mini *mini, char *oldpwd);
char					*ft_strdup_oldpwd(const char *s, int *i);
int						cd_noarg_return(char *av, bool can_cd, char *oldpwd);
// \ echo.c
void					bi_echo(char **av);
// \ env.c
void					bi_env(t_list *env_list);
char					*get_env_var(t_list *env_list, char *var);
int						valid_env_var_name(char *str, bool is_entry);
void					delete_var(t_list **head, void *node_to_del);
// \ export.c
int						ft_strlen_eq(char *s);
void					show_export(t_list *env_list, char **av);
int						delete_if_needed(t_list **env_list, char *var, int len);
void					bi_export(t_mini *mini, char **av, int j);
void					export_add(t_mini *mini, char *av);
// \ pwd.c
void					bi_pwd(void);
// \ unset.c
void					bi_unset(t_mini *mini, t_command *cmd);
// \ exit.c
int						calculate_exit_code_from_string(const char *number);
bool					str_is_num(const char *str);
bool					bi_exit(t_mini *mini, char **args, bool has_next);
#endif
