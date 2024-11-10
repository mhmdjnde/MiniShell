/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_other.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 17:20:33 by mjoundi           #+#    #+#             */
/*   Updated: 2024/11/10 02:43:48 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//checking the abs path that it is accessible
int	check_absolute_path(char **args, t_check_ve *vars, int *exit_status)
{
	struct stat	sb;

	if (args[0][0] == '.' || args[0][0] == '/')
	{
		if (stat(args[0], &sb) == 0)
		{
			if ((sb.st_mode & S_IFMT) == S_IFDIR
				&& print_in_chek_absolute_dir(vars->args,
					vars->f, exit_status) == 0)
				return (-1);
			else if (access(args[0], X_OK) == 0)
			{
				vars->flag = 1;
				return (execute_command(args[0], vars, exit_status));
			}
			else
				return (print_in_chek_absolute_denied(vars->args,
						vars->f, exit_status));
		}
		else
			return (print_in_chek_absolute_no_file(args));
	}
	return (0);
}

void	init_vars(t_check_ve *vars, int *exit_status)
{
	vars->flag = 0;
	vars->result = 0;
	vars->result = check_absolute_path(vars->args, vars, exit_status);
	vars->i = 0;
	vars->status = 0;
	vars->dir_len = 0;
	vars->path = NULL;
	vars->cmd_path = NULL;
	vars->dir = NULL;
	vars->path = env_search("PATH", vars->env);
	if (vars->path == NULL && access(vars->args[0], F_OK) != 0)
	{
		write(2, "bash: ", 6);
		ft_putstr_fd(vars->args[0], 2);
		write(2, " No such file or directory\n", 27);
		return ;
	}
}

int	returned_status(t_check_ve *vars, int *exit_status)
{
	vars->status = execute_command(vars->cmd_path, vars, exit_status);
	free(vars->cmd_path);
	return (vars->status);
}

char	*construct_command_path(t_check_ve *vars, char *cmd)
{
	int		dir_len;
	int		cmd_len;
	char	*cmd_path;

	dir_len = ft_strlen(vars->dir);
	cmd_len = ft_strlen(cmd);
	cmd_path = (char *)malloc(dir_len + 1 + cmd_len + 1);
	if (cmd_path == NULL)
	{
		perror("Error allocating memory");
		return (NULL);
	}
	ft_strcpy(cmd_path, vars->dir);
	if (dir_len > 0 && vars->dir[dir_len - 1] != '/')
		ft_strcat(cmd_path, "/");
	ft_strcat(cmd_path, cmd);
	free(vars->dir);
	return (cmd_path);
}

int	check_ve(char **args, char **env, int f, int *exit_status)
{
	t_check_ve	vars;

	init_vars2(&vars, f, args, env);
	init_vars(&vars, exit_status);
	if (vars.result != 0)
		return (vars.result);
	if (vars.path == NULL)
		return (-1);
	while (vars.path[vars.i] != '\0')
	{
		calc_dir_lenth(&vars);
		dup_into_dir(&vars);
		if (vars.dir == NULL)
			return (-1);
		vars.cmd_path = construct_command_path(&vars, args[0]);
		if (vars.cmd_path == NULL)
			return (-1);
		if (access(vars.cmd_path, X_OK) == 0)
			return (returned_status(&vars, exit_status));
		free(vars.cmd_path);
		increment_i(&vars);
	}
	if (vars.flag == 0)
		print_in_checkve(args, f, exit_status);
	return (-1);
}
