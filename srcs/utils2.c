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

void	skip_q(char *str, int *i, char c)
{
	(*i)++;
	while (str[*i] != '\0' && str[*i] != c)
		(*i)++;
	if (str[*i] != '\0')
		(*i)++;
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
	// int	qf;

	// qf = 0;
	if (str[*i] == '"')
	{
		// qf = 1;
		without_2q_ret(i, str, result, j);
		// qf = 0;
	}
	else if (str[*i] == '\'')
	{
		// qf = 2;
		without_q_ret(i, str, result, j);
		// qf = 0;
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
