/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 03:37:36 by marvin            #+#    #+#             */
/*   Updated: 2024/11/10 03:37:36 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_in_cd(t_maintools *tools)
{
	write(2, "cd: ", 4);
	ft_putstr_fd(env_search("HOME", tools->en), 2);
	write(2, " No such file or directory\n", 27);
	tools->exit_status = 1;
}

int	cd_err_1(t_maintools *tools)
{
	if (count_args(tools->str, "cd") > 1)
	{
		tools->strs = parse_args(tools->str, "cd");
		write(2, "bash: cd: too many arguments\n", 29);
		free(tools->str);
		tools->exit_status = 1;
		tools->cdf = 1;
		return (0);
	}
	return (1);
}

void	cd_err_2(t_maintools *tools)
{
	if (chdir(env_search("HOME", tools->en)) == 0)
	{
		free(tools->cd);
		tools->cd = ft_strdup(tools->temp);
	}
	else
		print_in_cd(tools);
}

int	cd(t_maintools *tools)
{
	tools->str = rm_dl(tools->str);
	var_in_env(&tools->str, tools->en, &tools->exit_status);
	tools->str = rm_bs(tools->str);
	if (cd_err_1(tools) == 0)
		return (0);
	if (count_args(tools->str, "cd") == 0)
	{
		tools->temp = get_pwd2(tools->en, &tools->exit_status);
		if (env_search("HOME", tools->en) != NULL)
			cd_err_2(tools);
		else
			write(2, "bash: cd: HOME not set\n", 24);
		free(tools->temp);
		free(tools->str);
		tools->cdf = 1;
		return (0);
	}
	tools->strs = parse_args(tools->str, "cd");
	do_cd(tools->en, tools->strs[1], &tools->cd, tools);
	free_args(&tools->strs);
	tools->cdf = 1;
	return (1);
}
