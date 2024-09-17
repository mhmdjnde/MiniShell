/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_flags.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 19:19:33 by mjoundi           #+#    #+#             */
/*   Updated: 2024/09/14 05:01:00 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	q_parse(char *str, int i, int *j, char *flag)
{
	char	quote;

	quote = '\0';
	if (str[i] == '"' || str[i] == '\'')
	{
		if (quote == '\0')
			quote = str[i];
		else if (quote == str[i])
			quote = '\0';
		else
			return (-1);
	}
	else if (str[i] == flag[*j])
	{
		(*j)++;
		if (flag[*j] == '\0')
			return (1);
	}
	else if (str[i] != ' ' && quote == '\0')
		return (-1);
	return (1);
}

int	parse_flags(char *str, char *flag)
{
	int		i;
	int		j;
	int		t;

	i = 0;
	j = 0;
	while (str[i] != '\0')
	{
		t = q_parse(str, i, &j, flag);
		if (t == -1)
			return (-1);
		if (t == 1)
			return (1);
		i++;
	}
	if (flag[j] == '\0')
		return (1);
	return (-1);
}
