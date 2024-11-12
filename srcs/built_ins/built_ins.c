/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   en_ex_ec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 03:36:02 by marvin            #+#    #+#             */
/*   Updated: 2024/11/10 03:36:02 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	envv(char **env, int *exit_status)
{
	int	i;

	i = 0;
	while (env[i])
	{
		printf("%s\n", env[i]);
		i++;
	}
	*exit_status = 0;
}

void	echo(t_maintools *tools)
{
	tools->str = rm_dl(tools->str);
	var_in_env(&tools->str, tools->en, &tools->exit_status);
	tools->str = rm_bs(tools->str);
	tools->strs = parse_args(tools->str, "echo");
	echo_args_check(tools->strs);
	free_args(&tools->strs);
	tools->exit_status = 0;
}

void	export(t_maintools *tools)
{
	tools->str = rm_dl(tools->str);
	var_in_env(&tools->str, tools->en, &tools->exit_status);
	tools->str = rm_bs(tools->str);
	if (count_args(tools->str, "export") == 0)
	{
		print_exp(tools->ex);
		tools->exit_status = 0;
	}
	else
	{
		tools->strs = parse_args(tools->str, "export");
		add_exp(tools, &tools->ex, &tools->en, 0);
		free_args(&tools->strs);
	}
}

void	unset(t_maintools *tools)
{
	tools->str = rm_dl(tools->str);
	var_in_env(&tools->str, tools->en, &tools->exit_status);
	tools->str = rm_bs(tools->str);
	tools->strs = parse_args(tools->str, "unset");
	rm_exp(tools->strs, &tools->ex, &tools->en, &tools->exit_status);
	free_args(&tools->strs);
}
