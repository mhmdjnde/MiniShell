/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils7.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 04:49:52 by marvin            #+#    #+#             */
/*   Updated: 2024/11/10 04:49:52 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	valquotes(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '\\' && str[i + 1] != '\0'
			&& (str[i + 1] == '"' || str[i + 1] == '\''))
			i += 2;
		else if (str[i] == '"')
		{
			if (parse_q2(str, &i) == 0)
				return (0);
		}
		else if (str[i] == '\'')
		{
			if (parse_q1(str, &i) == 0)
				return (0);
		}
		else
			i++;
	}
	return (1);
}

int	get_index(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0' && str[i] == ' ')
		i++;
	while (str[i] != '\0' && str[i] != ' ')
	{
		if (str[i] == '"')
			skip_q(str, &i, '"');
		else if (str[i] == '\'')
			skip_q(str, &i, '\'');
		else
		{
			if (str[i] == ' ' || str[i] == '\0')
				return (i);
			i++;
		}
	}
	return (i);
}

int	get_pid(void)
{
	int	pid;

	pid = fork();
	if (pid < 0)
	{
		perror("fork failed");
		exit(EXIT_FAILURE);
	}
	else if (pid > 0)
	{
		wait(NULL);
		return (pid);
	}
	else
	{
		exit(0);
	}
	return (-1);
}

int	bs_check(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == c)
			return (1);
		i++;
	}
	return (0);
}
