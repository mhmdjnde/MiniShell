/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   red_clean.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 04:28:16 by marvin            #+#    #+#             */
/*   Updated: 2024/11/10 04:28:16 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_red(t_redtools *red)
{
	int	i;

	i = 0;
	while (red[i].file != NULL)
	{
		free(red[i].file);
		i++;
	}
	free(red);
}

char	*del_part(char *str, int st, int len)
{
	int		l;
	int		nl;
	char	*new_str;

	l = ft_strlen(str);
	if (st < 0 || st >= l || len <= 0 || st + len > l)
		return (str);
	nl = l - len + 1;
	new_str = (char *)malloc((nl + 2) * sizeof(char));
	if (!new_str)
		return (NULL);
	ft_strncpy(new_str, str, st);
	new_str[st] = ' ';
	ft_strcpy(new_str + st + 1, str + st + len);
	free(str);
	new_str[nl] = '\0';
	return (new_str);
}

int	more_red_check(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '>' || str[i] == '<')
			return (1);
		i++;
	}
	return (0);
}

void	q_red(t_redtools **red)
{
	int		i;
	char	*t;

	i = 0;
	while (red[i]->file != NULL)
	{
		t = red[i]->file;
		red[i]->file = without_quotes_ret(t, 0);
		i++;
		free(t);
	}
}
