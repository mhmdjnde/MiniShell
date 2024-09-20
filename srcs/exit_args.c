/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_args.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 17:03:44 by mjoundi           #+#    #+#             */
/*   Updated: 2024/09/20 23:44:50 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	arg_no_q(char **args, int *i, int *sf, int *exit_status)
{
	while (args[1][*i] != '\0' && args[1][*i] != '\'' && args[1][*i] != '"')
	{
		if ((args[1][*i] == '+' || args[1][*i] == '-')
			&& *sf == 0 && args[1][*i + 1] != '\0')
		{
			*sf = 1;
		}
		else if (!(args[1][*i] >= '0' && args[1][*i] <= '9'))
		{
			write(2, "numeric argument requied\n", 25);
			*exit_status = 2;
			return (0);
		}
		(*i)++;
		*sf = 1;
	}
	return (1);
}

void	sign_check(char **args, int *i, int *sf)
{
	if ((args[1][*i] == '+' || args[1][*i] == '-')
		&& *sf == 0 && args[1][*i + 1] != '\0')
	{
		*sf = 1;
		(*i)++;
	}
}

int	arg_2q(char **args, int *i, int *sf, int *exit_status)
{
	if (args[1][(*i)] == '"')
	{
		(*i)++;
		while (args[1][*i] != '\0' && args[1][*i] != '"')
		{
			sign_check(args, i, sf);
			if (args[1][*i] == '\'')
			{
				without_quotes(args[1], 0);
				write(2, "numeric argument requied\n", 25);
				*exit_status = 2;
				return (0);
			}
			if (!(args[1][*i] >= '0' && args[1][*i] <= '9'))
			{
				without_quotes(args[1], 0);
				*exit_status = 2;
				write(2, "numeric argument requied\n", 25);
				return (0);
			}
			(*i)++;
			*sf = 1;
		}
	}
	return (1);
}

int	arg_1q(char **args, int *i, int *sf, int *exit_status)
{
	if (args[1][*i] == '\'')
	{
		(*i)++;
		while (args[1][*i] != '\0' && args[1][*i] != '\'')
		{
			sign_check(args, i, sf);
			if (args[1][*i] == '"')
			{
				without_quotes(args[1], 0);
				write(2, "numeric argument requied\n", 25);
				*exit_status = 2;
				return (0);
			}
			if (!(args[1][*i] >= '0' && args[1][*i] <= '9'))
			{
				without_quotes(args[1], 0);
				write(2, "numeric argument requied\n", 25);
				*exit_status = 2;
				return (0);
			}
			(*i)++;
			*sf = 1;
		}
	}
	return (1);
}

void	exit_args_check(char **args, int *exit_status)
{
	int	i;
	int	ac;
	int	sf;

	i = 0;
	ac = 0;
	sf = 0;
	while (args[ac] != NULL)
		ac++;
	if (ac > 2)
	{
		write(2, "too many arguments\n", 17);
		*exit_status = 1;
	}
	else if (ac == 2)
	{
		while (args[1][i] != '\0')
		{
			if (arg_no_q(args, &i, &sf, exit_status) == 0)
				return ;
			if (arg_2q(args, &i, &sf, exit_status) == 0)
				return ;
			if (arg_1q(args, &i, &sf, exit_status) == 0)
				return ;
			if (args[1][i] != '\0')
				i++;
		}
	}
}
