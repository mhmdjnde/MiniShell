/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   edit_her.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 02:31:16 by marvin            #+#    #+#             */
/*   Updated: 2024/11/01 02:31:16 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strstr(char *str, char *to_find)
{
	int	len;
	int	i;
	int	j;

	len = 0;
	while (to_find[len])
		len++;
	if (len == 0)
		return (str);
	i = 0;
	j = 0;
	while (str[i])
	{
		while (to_find[j] == str[i + j])
		{
			if (j + 1 == len)
				return (str + i);
			j++;
		}
		j = 0;
		i++;
	}
	return (NULL);
}

char	*find_l_h(char *str)
{
	char	*last_heredoc;
	char	*current;

	last_heredoc = NULL;
	current = str;
	while (1)
	{
		current = ft_strstr(current, "<<");
		if (!current)
			break ;
		last_heredoc = current;
		current += 2;
	}
	return (last_heredoc);
}

char	*get_fn(char *last_heredoc)
{
	char	*fn_start;
	char	*fn_end;
	size_t	fn_len;
	char	*fn;

	fn_start = last_heredoc + 2;
	while (*fn_start == ' ')
		fn_start++;
	fn_end = fn_start;
	while (*fn_end && *fn_end != ' '
		&& *fn_end != '>' && *fn_end != '<')
		fn_end++;
	fn_len = fn_end - fn_start;
	fn = malloc(fn_len + 1);
	ft_strncpy(fn, fn_start, fn_len);
	fn[fn_len] = '\0';
	return (fn);
}

//this is to replace teh herdoc with read from the temp file
void	replace_heredoc_with_file(char **input, char *replacement)
{
	char	*str;
	char	*last_h;
	char	*fn;
	size_t	new_l;
	char	*new_str;

	if (!input || !*input || !replacement)
		return ;
	str = *input;
	last_h = find_l_h(str);
	if (!last_h)
		return ;
	fn = get_fn(last_h);
	new_l = (last_h - str) + 2 + ft_strlen(replacement)
		+ ft_strlen(last_h + 2 + ft_strlen(fn)) + 1;
	new_str = malloc(new_l);
	ft_strncpy(new_str, str, last_h - str);
	new_str[last_h - str] = '\0';
	ft_strcat(new_str, "<");
	ft_strcat(new_str, replacement);
	ft_strcat(new_str, last_h + 3 + ft_strlen(fn));
	free(*input);
	*input = new_str;
	free(fn);
}
