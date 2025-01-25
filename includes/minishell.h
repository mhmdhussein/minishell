/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohhusse <mohhusse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 23:09:17 by fel-ghaz          #+#    #+#             */
/*   Updated: 2025/01/21 08:49:47 by fel-ghaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H
#define _XOPEN_SOURCE 700

#include "libft/libft.h"
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

typedef struct s_env
{
	char			*key;
	bool			equal;
	char			*value;
	struct s_env	*next;
	char	*exp_result;
	char	*exp_value;
	int		exp_index;
	int		exp_quote;
} t_env;

typedef enum e_token_type
{
	WORD,
	PIPE,
	IN,
	OUT,
	APPEND,
	HEREDOC,
	ENV_VAR
} token_type;

typedef struct s_token
{
	char			*value;
	token_type 		type;
	struct s_token	*next;
	struct s_token	*previous;
} t_token;

typedef struct s_cmd
{
	char			**args;
	struct s_cmd	*next;
} t_cmd;

typedef struct s_shell
{
	t_env	*env;
	t_env	*xenv;
	t_cmd	*cmds;
	bool	running;
	int		last_exit_status;
} t_shell;

// Split
int  is_space(char character);
int  check_delimiter(char *str);
bool input_has_unmatched_quotes(char *str);
int  skip_quoted_sequence(char *str, int i);
int  skip_token(char *str, int i);
int  count_tokens(char *str);
int  handle_quoted_sequences(char *str, int *i, char *token);
int  process_quotes(const char *str, int *i, int *state, char *token);
int  handle_quoted_sequence(char *str, int *i, char *quote_char);
int  handle_delimiter(char *str, int *i, char *token, int *j);
int   process_and_count_token(char *str, int *i, char *token);
char *copy_token(char *str, int start, int end);
void  ft_split_helper1(char *str, int token_start, int *ij, char **tokens);
void  ft_split_helper0(char *str, int *ij, char **tokens);
char **ft_split(char *str);

//expansion
int ft_isspecial(int c);
char *ft_strjoincustom(char *s1, char *s2);
token_type check_variable(t_token *linked_list);
void check_quotes(char c, int *quote);
char *appendchar(char *str, char c);
t_token *expand_variable(t_token *tokens, t_shell *shell);
char *handle_env_variable(char *result, t_token *token, t_shell *shell, int *i);
void expand(t_token *token, t_shell *shell);
void expand_number(t_token *token);
char *handle_dollar_digit_cases(char *result, char *value, int *i);
char *expand_quote_cases(char *result, char *value, int *i, t_shell *shell);
char *expand_dollar(t_token *token, t_shell *shell, int *i);
char *handle_dollar_quote_dollar(t_env *env);
char *handle_expansion_loop(t_env *env, t_shell *shell);
void expand_general(t_token *token, t_shell *shell);
char *expand2(t_shell *shell, char *key);
void expand_cases(t_token *token, t_shell *shell, int *i);
char *remove_quotes(char *value, t_shell *shell);

// Builtins
void		exec_builtin(t_cmd *cmd, t_shell *shell);
void		ft_pwd(char **args);
void		ft_echo(t_cmd *cmd);
void		ft_env(t_env *env);
void		ft_cd(t_cmd *cmd, t_env *env);
void		ft_exit(t_cmd *cmd, t_shell *shell);
void		ft_unset(t_cmd *cmd, t_shell *shell);
void		ft_export(t_cmd *cmd, t_shell *shell);

// Builtins utils
bool	is_builtin(char *cmd);

token_type	determine_token_type(char *value);
t_token		*create_node(char *splitted_tokens);
t_token		*create_linked_list(char **splitted_tokens);
void		free_tokens(t_token *head);
t_token		*tokenize(char *input);
int			ft_strcmp(char *str1, char *str2);

// Exec
void		execute_command(t_cmd *cmd, t_shell *shell);

// Env
t_env		*create_env(char *key, char *value);
int			checkenv(t_env *env, char *key);
void		addenv(t_env *env, char *key, char *value);
char		*envget(t_env *env, char *key);
void		envset(t_env *env, char *key, char *value);

// Utils
char	*ft_strtok(char *str, const char *delim);
t_env	*merge_sort(t_env *env);
char *ft_strncpy(char *dest, char *src, int n);
void	free_double_array(char **array);

#endif
