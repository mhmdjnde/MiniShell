/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjoundi <mjoundi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 19:26:51 by mjoundi           #+#    #+#             */
/*   Updated: 2024/08/27 19:26:51 by mjoundi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	without_quotes(char *str, int flag)
{
	int	i;
	int	qf;

	i = 0;
	qf = 0;
	while (str[i] != '\0' && str[i] == ' ')
		i++;
	while (str[i] != '\0')
	{
		if (str[i] == '"')
			without_2q(&qf, &i, str);
		else if (str[i] == '\'')
			without_q(&qf, &i, str);
		else
		{
			write(1, &str[i], 1);
			i++;
		}
		if (i >= get_index(str) && flag == 1)
			break ;
	}
}

void	without_2q_ret(int *i, char *str, char **result, int *j)
{
	(*i)++;
	while (str[*i] != '\0' && str[*i] != '"')
	{
		(*result)[(*j)++] = str[*i];
		(*i)++;
	}
	if (str[*i] != '\0')
		(*i)++;
}

void	without_q_ret(int *i, char *str, char **result, int *j)
{
	(*i)++;
	while (str[*i] != '\0' && str[*i] != '\'')
	{
		(*result)[(*j)++] = str[*i];
		(*i)++;
	}
	if (str[*i] != '\0')
		(*i)++;
}

void	wq_helper(int *i, char *str, char **result, int *j)
{
	if (str[*i] == '"')
	{
		without_2q_ret(i, str, result, j);
	}
	else if (str[*i] == '\'')
	{
		without_q_ret(i, str, result, j);
	}
	else
	{
		(*result)[(*j)++] = str[*i];
		(*i)++;
	}
}

char	*without_quotes_ret(char *str, int flag)
{
	int		i;
	int		j;
	char	*result;

	i = 0;
	j = 0;
	result = malloc(ft_strlen(str) + 5);
	if (!result)
		return (NULL);
	while (str[i] != '\0' && str[i] == ' ')
		i++;
	while (str[i] != '\0')
	{
		wq_helper(&i, str, &result, &j);
		if (i >= get_index(str) && flag == 1)
			break ;
	}
	result[j] = '\0';
	return (result);
}
