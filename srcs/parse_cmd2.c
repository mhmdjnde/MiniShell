/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 04:03:14 by marvin            #+#    #+#             */
/*   Updated: 2024/11/10 04:03:14 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse_2q(int **tab, char *str, char **temp)
{
	(*tab)[2] = 1;
	(*tab)[0]++;
	while (str[(*tab)[0]] != '"' && str[(*tab)[0]] != '\0')
	{
		if (str[(*tab)[0]] >= 'a' && str[(*tab)[0]] <= 'z')
		{
			(*temp)[(*tab)[1]] = str[(*tab)[0]];
			(*tab)[1]++;
		}
		else if (str[(*tab)[0]] == '\'')
		{
			free((*temp));
			return (-1);
		}
		else
		{
			free((*temp));
			return (-1);
		}
		(*tab)[0]++;
	}
	return (1);
}

int	parse_q(char *str, int **tab, char **temp)
{
	(*tab)[2] = 2;
	(*tab)[0]++;
	while (str[(*tab)[0]] != '\'' && str[(*tab)[0]] != '\0')
	{
		if (str[(*tab)[0]] >= 'a' && str[(*tab)[0]] <= 'z')
		{
			(*temp)[(*tab)[1]] = str[(*tab)[0]];
			(*tab)[1]++;
		}
		else if (str[(*tab)[0]] == '"')
		{
			free((*temp));
			return (-1);
		}
		else
		{
			free((*temp));
			return (-1);
		}
		(*tab)[0]++;
	}
	return (1);
}

int	help_cmd(int **tab, char *str, char **temp)
{
	if (str[(*tab)[0]] == '"')
	{
		if (parse_2q(tab, str, temp) == -1)
			return (-1);
	}
	else if (str[(*tab)[0]] == '\'')
	{
		if (parse_q(str, tab, temp) == -1)
			return (-1);
	}
	else if (str[(*tab)[0]] >= 'a' && str[(*tab)[0]] <= 'z')
	{
		(*temp)[(*tab)[1]] = str[(*tab)[0]];
		(*tab)[1]++;
	}
	else if (str[(*tab)[0]] == ' ' || str[(*tab)[0]] == '\0')
		return (0);
	else
	{
		free((*temp));
		return (-1);
	}
	(*tab)[0]++;
	return (1);
}
