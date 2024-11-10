/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils8.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 04:53:09 by marvin            #+#    #+#             */
/*   Updated: 2024/11/10 04:53:09 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse_q2(char *str, int *i)
{
	(*i)++;
	while (str[*i] != '"' && str[*i] != '\0')
	{
		if (str[*i] == '\\' && str[*i + 1] != '\0'
			&& (str[*i + 1] == '"' || str[*i + 1] == '\''))
			*i += 2;
		else
			(*i)++;
	}
	if (str[*i] == '\0')
		return (0);
	(*i)++;
	return (1);
}

int	parse_q1(char *str, int *i)
{
	(*i)++;
	while (str[*i] != '\'' && str[*i] != '\0')
		(*i)++;
	if (str[*i] == '\0')
		return (0);
	(*i)++;
	return (1);
}

void	skip_q(char *str, int *i, char c)
{
	(*i)++;
	while (str[*i] != '\0' && str[*i] != c)
		(*i)++;
	if (str[*i] != '\0')
		(*i)++;
}

void	without_2q(int *qf, int *i, char *str)
{
	*qf = 1;
	(*i)++;
	while (str[*i] != '\0' && str[*i] != '"')
	{
		write(1, &str[*i], 1);
		(*i)++;
	}
	if (str[*i] != '\0')
		(*i)++;
	*qf = 0;
}

void	without_q(int *qf, int *i, char *str)
{
	*qf = 2;
	(*i)++;
	while (str[*i] != '\0' && str[*i] != '\'')
	{
		write(1, &str[*i], 1);
		(*i)++;
	}
	if (str[*i] != '\0')
		(*i)++;
	*qf = 0;
}
