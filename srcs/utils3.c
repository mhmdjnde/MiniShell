/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjoundi <mjoundi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 19:29:25 by mjoundi           #+#    #+#             */
/*   Updated: 2024/08/27 19:29:25 by mjoundi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	bs_loop(char *str, char **wb, int *i, int *j)
{
	*i = 0;
	while (str[*i] != '\0')
	{
		if (str[*i] == '\\')
			(*i)++;
		else
		{
			(*wb)[*j] = str[*i];
			(*i)++;
			(*j)++;
		}
	}
	(*wb)[*j] = '\0';
	free(str);
}

char	*rm_bs(char *str)
{
	int		i;
	int		len;
	int		j;
	char	*wb;

	i = 0;
	j = 0;
	len = 0;
	if (bs_check(str, '\\') == 1)
	{
		while (str[i] != '\0')
		{
			if (str[i] == '\\')
				i++;
			else
			{
				i++;
				len++;
			}
		}
		wb = malloc(len + 10);
		bs_loop(str, &wb, &i, &j);
		return (wb);
	}
	return (str);
}

void	dl_check_cpy(char *str, int *i, int *j, char **wd)
{
	int	qf;

	qf = 0;
	if (str[*i] == '"' && qf == 0)
		qf = 1;
	else if (str[*i] == '"' && qf == 1)
		qf = 0;
	if (str[*i] == '$' && !is_ok2(str[*i + 1]) && str[*i + 1] != '\''
		&& str[*i + 1] != '"' && str[*i + 1] != '\0' && str[*i + 1] != ' ')
		*i += 2;
	else if (str[*i] == '$' && (str[*i + 1] == '\'' || str[*i + 1] == '"')
		&& *i != 0 && (str[*i - 1] != '\'' && str[*i - 1] != '"') && qf != 1)
		(*i)++;
	else if (str[*i] == '$' && *i == 0 && (str[*i + 1] == '\''
			|| str[*i + 1] == '"'))
		(*i)++;
	else
	{
		(*wd)[*j] = str[*i];
		(*i)++;
		(*j)++;
	}
}

void	run_loop(char *str, int *i, int *j, char **wd)
{
	while (str[*i] != '\0')
	{
		if (str[*i] == '$' && (str[*i + 1] == '$' || str[*i + 1] == '?'))
		{
			(*wd)[*j] = str[*i];
			(*i)++;
			(*j)++;
			(*wd)[*j] = str[*i];
			(*i)++;
			(*j)++;
			continue ;
		}
		dl_check_cpy(str, i, j, wd);
	}
}

char	*rm_dl(char *str)
{
	int		i;
	int		j;
	char	*wd;

	i = 0;
	j = 0;
	if (bs_check(str, '$') == 1)
	{
		wd = malloc(ft_strlen(str) + 1);
		if (!wd)
			return (NULL);
		i = 0;
		run_loop(str, &i, &j, &wd);
		wd[j] = '\0';
		free(str);
		return (wd);
	}
	return (str);
}
