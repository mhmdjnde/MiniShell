/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 02:16:44 by marvin            #+#    #+#             */
/*   Updated: 2024/11/10 02:16:44 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//checking if its '$$' or the '$?' case
char	*dlr_qm(char *str, int *i)
{
	if (str[*i - 1] == '$')
		return ("$$");
	else
		return ("$?");
}

char	*increment_in_helper3(int *i, char *str)
{
	*i += 2;
	return (dlr_qm(str, i));
}

//parsing the variable name
int	helper2(char *str, int *i, int *index)
{
	if (*i != 0 && str[*i - 1] == '\\')
	{
		if (str[*i] != '\0')
			(*i)++;
		return (0);
	}
	index[0] = *i + 1;
	if (str[*i] != '\0')
		(*i)++;
	if (!(str[*i] >= 'a' && str[*i] <= 'z') && !(str[*i] >= 'A'
			&& str[*i] <= 'Z'))
	{
		if (str[*i] != '\0')
			(*i)++;
		return (0);
	}
	while (str[*i] != '\0' && is_ok(str[*i]))
		(*i)++;
	index[1] = *i;
	return (1);
}

//parsing the variable name
void	var_ret_helper1(char *str, int *i, int *double_quotes)
{
	if (*double_quotes == 0 && str[*i] == '"')
	{
		*double_quotes = 1;
		(*i)++;
	}
	else if (*double_quotes == 1 && str[*i] == '"')
	{
		*double_quotes = 0;
		(*i)++;
	}
	if (str[*i] == '\'' && *double_quotes == 0)
	{
		(*i)++;
		while (str[*i] != '\'' && str[*i] != '\0')
			(*i)++;
		if (str[*i] != '\0')
			(*i)++;
	}
	else if (*double_quotes == 1)
	{
		while(str[*i] != '"' && str[*i] != '$' && str[*i] != '\0')
			(*i)++;
		if (str[*i] == '"')
			(*i)++;
	}
}

//parsing the variable name
char	*var_ret_func(char *str, int *i, int *index)
{
	static int		double_quotes;
	char	*t;

	// double_quotes = 0;
	while (str[*i] != '\0')
	{
		var_ret_helper1(str, i, &double_quotes);
		// if (double_quotes == 1)
		// {
		// 	while(str[*i] != '"' && str[*i] != '$' && str[*i] != '\0')
		// 		(*i)++;
		// }
		if (str[*i] == '$')
		{
			if (str[*i + 1] == '$' || str[*i + 1] == '?')
			{
				t = increment_in_helper3(i, str);
				if (t != NULL)
					return (t);
			}
			if (helper2(str, i, index) == 0)
				return (NULL);
			if (index[0] == index[1])
				continue ;
			break ;
		}
		if (str[*i] != '\0')
			(*i)++;
	}
	return (str);
}
