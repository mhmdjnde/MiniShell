/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 03:26:17 by marvin            #+#    #+#             */
/*   Updated: 2024/11/10 03:26:17 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	event_not_found(char *str, int *i, int *j)
{
	char	*t;

	(*i)++;
	*j = *i;
	while (str[*i] != '\0' && is_ok(str[*i]))
		(*i)++;
	t = ft_substr(str, *j, *i - *j);
	ft_putstr_fd(t, 2);
	write(2, " event not found\n", 17);
	free(t);
}

int	event_check(char *str)
{
	int		i;
	char	q;
	int		j;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '"' || str[i] == '\'')
		{
			q = str[i];
			i++;
			while (str[i] != '\0' && str[i] != q)
				i++;
		}
		else if (str[i] == '!')
		{
			event_not_found(str, &i, &j);
			return (0);
		}
		else
			i++;
	}
	return (1);
}
