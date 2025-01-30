/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohhusse <mohhusse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 23:09:17 by fel-ghaz          #+#    #+#             */
/*   Updated: 2025/01/30 12:04:16 by mohhusse         ###   ########.fr       */
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
	char			*exp_result;
	char			*exp_value;
	int				exp_index;
	int				exp_quote;
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
	struct s_token	*previous;
}	t_token;

typedef struct s_cmd
{
	char			**args;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_shell
{
	t_env	*env;
	t_env	*xenv;
	t_cmd	*cmds;
	bool	running;
	int		last_exit_status;
}	t_shell;

// Split
char	**split_input(char *str);
bool	input_has_unmatched_quotes(char *str);
int		is_space(char c);
int		process_and_count_token(char *str, int *i, char *token);
char	*copy_token(char *str, int start, int end);
void	process_token(char *str, int token_start, int *ij, char **tokens);
int		check_delimiter(char *str);

// Exec
void	exec_builtin(t_cmd *cmd, t_shell *shell);
bool	is_builtin(char *cmd);
void	execute_command(t_cmd *cmd, t_shell *shell);

// Builtins
void	ft_cd(t_cmd *cmd, t_env *env);
void	ft_echo(t_cmd *cmd);
void	ft_env(t_env *env);
void	ft_exit(t_cmd *cmd, t_shell *shell);
void	ft_export(t_cmd *cmd, t_shell *shell);
void	ft_pwd(char **args);
void	ft_unset(t_cmd *cmd, t_shell *shell);

// Env
void	envset(t_env *env, char *key, char *value);
char	*envget(t_env *env, char *key);
void	addenv(t_env *env, char *key, char *value);
int		checkenv(t_env *env, char *key);
t_env	*create_env(char *key, char *value);
char	**env_to_array(t_env *env);

// Utils
int		ft_strcmp(char *str1, char *str2);
char	*ft_strtok(char *str, const char *delim);
t_env	*merge_sort(t_env *env);

// Free
void	free_double_array(char **array);
void	free_tokens(t_token *head);
void	free_env(t_env *env);

#endif
