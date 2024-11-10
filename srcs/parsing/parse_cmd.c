/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjoundi <mjoundi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 19:16:19 by mjoundi           #+#    #+#             */
/*   Updated: 2024/08/27 19:16:19 by mjoundi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_in_q(char *str, int *i, int *c)
{
	if (str[*i] == '"')
	{
		(*i)++;
		while (str[*i] != '"' && str[*i] != '\0')
		{
			if (str[*i] >= 'a' && str[*i] <= 'z')
				(*c)++;
			(*i)++;
		}
	}
	if (str[*i] == '\'')
	{
		(*i)++;
		while (str[*i] != '\'' && str[*i] != '\0')
		{
			if (str[*i] >= 'a' && str[*i] <= 'z')
				(*c)++;
			(*i)++;
		}
	}
}

int	char_size(char *str)
{
	int	i;
	int	c;

	i = 0;
	c = 0;
	while (str[i] != '\0' && str[i] == ' ')
		i++;
	while (str[i] != '\0' && str[i] != ' ')
	{
		if (str[i] >= 'a' && str[i] <= 'z')
			c++;
		check_in_q(str, &i, &c);
		if (str[i] != '\0')
			i++;
	}
	return (c);
}

int	cmd_help(int **tab, char *str, char **temp)
{
	int	t;

	if (temp == NULL)
		return (-1);
	(*tab)[1] = 0;
	(*tab)[2] = -2;
	while (str[(*tab)[0]] != '\0' && str[(*tab)[0]] == ' ')
		(*tab)[0]++;
	while (str[(*tab)[0]] != '\0')
	{
		t = help_cmd(tab, str, temp);
		if (t == -1)
			return (-1);
		else if (t == 0)
			break ;
	}
	return (1);
}

int	free_and_ret(int **tab)
{
	int	t;

	t = (*tab)[0];
	free(*tab);
	return (t);
}

int	parse_cmd(char *str, char *cmd)
{
	char	*s;
	char	*temp;
	int		*tab;
	int		t;

	tab = malloc(4 * sizeof(int));
	tab[0] = 0;
	s = cmd;
	temp = malloc(char_size(str) + 1);
	t = cmd_help(&tab, str, &temp);
	if (t == -1)
		return (free(tab), -1);
	temp[tab[1]] = '\0';
	if (ft_strlen(temp) != 0)
	{
		tab[3] = ft_strcmp(temp, s);
		free(temp);
		if (tab[3] == 0)
			return (free_and_ret(&tab));
	}
	else
		free(temp);
	return (free(tab), -1);
}
