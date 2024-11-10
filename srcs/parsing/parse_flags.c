/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_flags.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 19:19:33 by mjoundi           #+#    #+#             */
/*   Updated: 2024/09/29 22:03:43 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse_q_flags(int i, char *str, char *quote)
{
	if (str[i] == '"' || str[i] == '\'')
	{
		if (*quote == '\0')
			*quote = str[i];
		else if (*quote == str[i])
			*quote = '\0';
		else
			return (-1);
	}
	return (1);
}

int	parse_flags(char *str, char *flag)
{
	int		i;
	int		j;
	char	quote;

	i = 0;
	j = 0;
	quote = '\0';
	while (str[i] != '\0')
	{
		if (parse_q_flags(i, str, &quote) == -1)
			return (-1);
		else if (str[i] == flag[j])
		{
			j++;
			if (flag[j] == '\0')
				return (1);
		}
		else if (str[i] != ' ' && quote == '\0')
			return (-1);
		i++;
	}
	if (flag[j] == '\0')
		return (1);
	return (-1);
}
