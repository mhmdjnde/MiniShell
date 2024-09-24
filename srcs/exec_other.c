/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_other.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjoundi <mjoundi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 17:20:33 by mjoundi           #+#    #+#             */
/*   Updated: 2024/09/24 19:22:00 by mjoundi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_j_in_getcmd(char *str)
{
	int	j;

	j = 0;
	while (str[j] != '\0' && str[j] == ' ')
		j++;
	return (j);
}

void	q_cmd(char *str, int *i)
{
	if (str[*i] == '"')
	{
		(*i)++;
		while (str[*i] != '"' && str[*i] != '\0')
			(*i)++;
		if (str[*i] == '"')
			(*i)++;
	}
	else if (str[*i] == '\'')
	{
		(*i)++;
		while (str[*i] != '\'' && str[*i] != '\0')
			(*i)++;
		if (str[*i] == '\'')
			(*i)++;
	}
	else
		(*i)++;
}

char	*get_cmd(char *str)
{
	int		i;
	char	*temp;

	i = get_j_in_getcmd(str);
	while (str[i] != '\0' && str[i] != ' ')
	{
		q_cmd(str, &i);
	}
	temp = ft_substr(str, get_j_in_getcmd(str), i);
	return (temp);
}

int	execute_command(char *cmd_path, t_check_ve *vars, int *exit_status)
{
	int	pid;
	int	status;

	pid = fork();
	if (pid == -1)
	{
		print_error("Error forking: ");
		return (-1);
	}
	else if (pid == 0)
	{
		restore_signals();
		execve(cmd_path, vars->args, vars->env);
		print_error("Error executing command: ");
		exit(EXIT_FAILURE);
	}
	else
	{
		ignore_signals();
		waitpid(pid, &status, 0);
		setup_signals();
		if (status == 2)
			status = status + 128;
		*exit_status = status;
		return (*exit_status);
	}
}

int	check_absolute_path(char **args, t_check_ve *vars, int *exit_status)
{
	struct stat	sb;

	if (args[0][0] == '.' || args[0][0] == '/')
	{
		if (stat(args[0], &sb) == 0)
		{
			if ((sb.st_mode & S_IFMT) == S_IFDIR
				&& print_in_chek_absolute_dir(vars->args, vars->f, exit_status) == 0)
				return (-1);
			else if (access(args[0], X_OK) == 0)
			{
				vars->flag = 1;
				return (execute_command(args[0], vars, exit_status));
			}
			else
				return (print_in_chek_absolute_denied(vars->args, vars->f, exit_status));
		}
		else
			return (print_in_chek_absolute_no_file(args));
	}
	return (0);
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
	strcpy(cmd_path, vars->dir);
	if (dir_len > 0 && vars->dir[dir_len - 1] != '/')
		strcat(cmd_path, "/");
	strcat(cmd_path, cmd);
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
	if (vars->path == NULL)
	{
		write(2, "bash: ", 6);
		ft_putstr_fd(vars->args[0], 2);
		write(2, "No such file or directory\n", 26);
		return ;
	}
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

int	returned_status(t_check_ve *vars, int *exit_status)
{
	vars->status = execute_command(vars->cmd_path, vars, exit_status);
	free(vars->cmd_path);
	return (vars->status);
}

int	print_in_chek_absolute_dir(char **args, int f, int *exit_status)
{
	write(2, "bash: ", 6);
	without_quotes(args[0], 0);
	write(2, ": Is a directory\n", 17);
	if (f != 1)
		*exit_status = 126;
	return (0);
}

int	print_in_chek_absolute_denied(char **args, int f, int *exit_status)
{
	write(2, "bash: ", 6);
	without_quotes(args[0], 0);
	write(2, ": Permission denied\n", 20);
	if (f != 1)
		*exit_status = 126;
	return (-1);
}

int	print_in_chek_absolute_no_file(char **args)
{
	write(2, "bash: ", 6);
	without_quotes(args[0], 0);
	write(2, ": No such file or directory\n", 28);
	return (-1);
}

void	free_2charpointers(char *dir, char	*cmd_path)
{
	free(dir);
	free(cmd_path);
}

void	print_error(char *error)
{
	write(2, error, ft_strlen(error));
	write(2, strerror(errno), ft_strlen(strerror(errno)));
	write(2, "\n", 1);
}
