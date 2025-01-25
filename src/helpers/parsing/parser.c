/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fel-ghaz <fel-ghaz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 13:09:14 by fel-ghaz          #+#    #+#             */
/*   Updated: 2025/01/20 15:07:56 by fel-ghaz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// int ft_strcmp(char *str1, char *str2)
// {
// 	int i;

//     i = 0;
//     while (str1[i] && str2[i])
//     {
//         if (str1[i] != str2[i])
//             return (str1[i] - str2[i]);
//         i++;
//     }
//     return (str1[i] - str2[i]);
// }

token_type determine_token_type(char *value)
{
	if (ft_strcmp(value, "|") == 0)
		return (PIPE);
	if (ft_strcmp(value, "<") == 0)
		return (IN);
	if (ft_strcmp(value, ">") == 0)
		return (OUT);
	if (ft_strcmp(value, ">>") == 0)
		return (APPEND);
	if (ft_strcmp(value, "<<") == 0)
		return (HEREDOC);
	if (value[0] == '$')
		return (ENV_VAR);
	return (WORD);
}
t_token *create_node(char *splitted_tokens)
{
	t_token *new_node;
	token_type type;
	char *value;

	new_node = (t_token *)malloc(sizeof(t_token));
	if (!new_node)
		return (NULL);
	value = strdup(splitted_tokens);
	type = determine_token_type(splitted_tokens);
	new_node->value = value;
	new_node->type = type;
	new_node->next = NULL;
	new_node->previous = NULL;

	return (new_node);
}
t_token *create_linked_list(char **splitted_tokens)
{
	t_token *head;
	t_token *tail;
	t_token *new_node;
	int i;

	i = 0;
	head = NULL;
	tail = NULL;
	while (splitted_tokens[i])
	{
		if (!(new_node = create_node(splitted_tokens[i++])))
			return (free_tokens(head), NULL);
		if (!head)
			head = tail = new_node;
		else
		{
			tail = tail->next = new_node;
			new_node->previous = tail;
		}
	}
	return (head);
}

void free_tokens(t_token *head)
{
	t_token *current = head;
	while (current)
	{
		t_token *temp = current;
		current = current->next;
		free(temp->value);
		free(temp);
	}
}
t_token *tokenize(char *input)
{
	char **splitted_tokens;
	t_token *linked_list;
	int i;

	splitted_tokens = ft_split(input);
	if (!splitted_tokens)
		return (NULL);
	linked_list = create_linked_list(splitted_tokens);
	i = 0;
	while (splitted_tokens[i])
	{
		free(splitted_tokens[i]);
		i++;
	}
	free(splitted_tokens);
	return (linked_list);
}
