/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 03:31:25 by marvin            #+#    #+#             */
/*   Updated: 2024/11/10 03:31:25 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_ex_st(char **args, int *exit_status)
{
	write(2, "bash: exit: ", 12);
	write(2, args[1], ft_strlen(args[1]));
	write(2, ": numeric argument required\n", 25);
	*exit_status = 2;
}

void	check_exit_status(char *temp, char **args, int *exit_status)
{
	long long	l;

	l = 0;
	if (!is_num(temp))
		*exit_status = 2;
	else
	{
		l = ft_atol(args[1]);
		if (l <= 9223372036854775807LL && l >= -9223372036854775807LL - 1)
			*exit_status = 1;
		else
			*exit_status = 2;
	}
	free(temp);
}

void	ex_st(char **args, int *exit_status)
{
	long long	l;
	char		*temp;

	if (args_len(args) == 2)
	{
		temp = without_quotes_ret(args[1], 0);
		if (!is_num(temp))
			*exit_status = 2;
		else
		{
			l = ft_atol(args[1]);
			if (l < 0 && l >= -9223372036854775807LL - 1)
				l += 256 * ((-l / 256) + 1);
			if (l <= 9223372036854775807LL && l >= -9223372036854775807LL - 1)
				*exit_status = l % 256;
			else
				print_ex_st(args, exit_status);
		}
		free(temp);
	}
	else if (args_len(args) > 2)
	{
		temp = without_quotes_ret(args[1], 0);
		check_exit_status(temp, args, exit_status);
	}
}

void	exitt(t_maintools *tools)
{
	tools->str = rm_dl(tools->str);
	var_in_env(&tools->str, tools->en, &tools->exit_status);
	tools->str = rm_bs(tools->str);
	tools->strs = parse_args(tools->str, "exit");
	printf("exit\n");
	exit_args_check(tools->strs, &tools->exit_status);
}
