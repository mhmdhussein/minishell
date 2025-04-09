/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohhusse <mohhusse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 23:09:17 by fel-ghaz          #+#    #+#             */
/*   Updated: 2025/03/17 13:36:54 by mohhusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# define _XOPEN_SOURCE 700

# include "libft/libft.h"
# include <ctype.h>
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <strings.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_env
{
	char			*key;
	bool			equal;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef enum e_token_type
{
	WORD,
	PIPE,
	IN,
	OUT,
	APPEND,
	HEREDOC,
	ENV_VAR
}	t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

typedef struct s_cmd
{
	char			**args;
	int				input_fd;
	int				output_fd;
	bool			append;
	char			*delim;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_shell
{
	t_env	*env;
	t_cmd	*cmds;
	t_token	*tokens;
	bool	running;
	int		last_exit_status;
	int		std_out;
	int		std_in;
}	t_shell;

// Input
char			**split_input(char *str);
bool			input_has_unmatched_quotes(char *str);
int				ft_isspace(char c);
int				process_and_count_token(char *str, int *i, char *token);
char			*copy_token(char *str, int start, int end);
void			process_token(char *str, int token_start, int *ij, char **tokens);
int				check_delimiter(char *str);
char 			*remove_quotes(char *value);

// Tokenize
t_token			*tokenize(char *input);
void			revert_var(t_token	**tokens);
char			**detokenize(t_token *tokens);
t_token			*create_token(char *split);
void			add_token(t_token **head, t_token **newnode);
void			print_tokens(t_token *start);

// Expand
void			check_quotes(char c, int *quote);
char			*appendchar(char *str, char c);
void			expand_variables(t_token *tokens, t_shell *shell);
int				is_var_char(char c, int first);
char			*expand_token(char *value, t_shell *shell);

// Exec
void			exec_builtin(t_cmd *cmd, t_shell *shell);
bool			is_builtin(char *cmd);
void			execute_command(t_cmd *cmd, t_shell *shell);
char			*handle_dollar_quote_dollar(t_env *env);

// Redirections
int				redirections(t_shell *shell, t_cmd *cmd, t_token *tokens);
void			handle_heredoc(t_shell *shell, t_cmd *cmd, t_token *tokens);

// Pipes
int				check_pipes(t_token *tokens);
void			parse_commands(t_token	*tokens, t_shell *shell);
void			handle_pipes(t_shell *shell);

// Builtins
void			ft_cd(t_cmd *cmd, t_env *env);
void			ft_echo(t_cmd *cmd);
void			ft_env(t_env *env);
void			ft_exit(t_cmd *cmd, t_shell *shell);
void			ft_export(t_cmd *cmd, t_shell *shell);
void			ft_pwd();
void			ft_unset(t_cmd *cmd, t_shell *shell);

// Env
void			envset(t_env *env, char *key, char *value);
char			*envget(t_env *env, char *key);
void			addenv(t_env *env, char *key, char *value);
int				checkenv(t_env *env, char *key);
t_env			*create_env(char *key, char *value);
char			**env_to_array(t_env *env);

// Utils
int				ft_strcmp(char *str1, char *str2);
char			*ft_strtok(char *str, const char *delim);
t_env			*merge_sort(t_env *env);

// Free
void			free_double_array(char **array);
void			free_tokens(t_token *head);
void			free_env(t_env *env);
void			free_cmds(t_cmd *cmds);

#endif
