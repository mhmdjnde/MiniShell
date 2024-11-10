/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_other_tools1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 02:35:42 by marvin            #+#    #+#             */
/*   Updated: 2024/11/10 02:35:42 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_in_checkve(char **args, int f, int *exit_status)
{
	if (f != 1)
		*exit_status = 127;
	write(2, "bash: ", 6);
	ft_putstr_fd(args[0], 2);
	write(2, " command not found\n", 19);
}

void	init_vars2(t_check_ve *vars, int f, char **args, char **env)
{
	vars->f = f;
	vars->args = args;
	vars->env = env;
}

void	calc_dir_lenth(t_check_ve *vars)
{
	vars->dir_len = 0;
	while (vars->path[vars->i + vars->dir_len] != ':'
		&& vars->path[vars->i + vars->dir_len] != '\0')
		vars->dir_len++;
}

void	dup_into_dir(t_check_ve *vars)
{
	vars->dir = strndup(vars->path + vars->i, vars->dir_len);
	if (vars->dir == NULL)
	{
		perror("Error allocating memory");
		return ;
	}
}

void	increment_i(t_check_ve *vars)
{
	if (vars->path[vars->i + vars->dir_len] == ':')
		vars->i += vars->dir_len + 1;
	else
		vars->i += vars->dir_len;
}
