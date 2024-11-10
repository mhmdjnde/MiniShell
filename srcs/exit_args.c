/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_args.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 17:03:44 by mjoundi           #+#    #+#             */
/*   Updated: 2024/11/10 02:50:17 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//checking num arg with no quotations
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

//checking num arg with double quotations
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

//checking num arg with single quotation
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

void	print_and_exit_status(int *exit_status)
{
	write(2, "too many arguments\n", 17);
	*exit_status = 1;
}

//check the args of the echo command
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
		print_and_exit_status(exit_status);
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
