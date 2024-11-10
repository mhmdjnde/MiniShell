/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_get_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 02:44:08 by marvin            #+#    #+#             */
/*   Updated: 2024/11/10 02:44:08 by marvin           ###   ########.fr       */
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

//parsing quptations
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

//getting the command from the string
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

//fork error handling
int	fork_check(int pid)
{
	if (pid == -1)
	{
		print_error("Error forking: ");
		return (-1);
	}
	return (1);
}

//execute command with execve and handle signals
int	execute_command(char *cmd_path, t_check_ve *vars, int *exit_status)
{
	int	pid;
	int	status;

	pid = fork();
	if (fork_check(pid) == -1)
		return (-1);
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
		if (WIFEXITED(status))
			*exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			*exit_status = 128 + WTERMSIG(status);
		return (*exit_status);
	}
}
