/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_func.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 04:15:52 by marvin            #+#    #+#             */
/*   Updated: 2024/11/10 04:15:52 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pipe_fork(t_pipetools *pt)
{
	if (pt->i < pt->num_cmds - 1)
	{
		if (pipe(pt->pipefd) == -1)
		{
			perror("pipe");
			return (0);
		}
	}
	pt->pid = fork();
	if (pt->pid == -1)
	{
		perror("fork");
		return (0);
	}
	if (pt->pid == 0)
		restore_signals();
	else
		ignore_signals();
	return (1);
}

void	pipe_func(t_pipetools *pt, t_maintools *tools)
{
	if (pt->i > 0)
	{
		dup2(pt->prev_fd, STDIN_FILENO);
		close(pt->prev_fd);
	}
	if (pt->i < pt->num_cmds - 1)
	{
		dup2(pt->pipefd[1], STDOUT_FILENO);
		close(pt->pipefd[0]);
		close(pt->pipefd[1]);
	}
	tools->str = tools->cmds[pt->i];
	run_one_cmd(tools);
}

void	pipe_work(t_pipetools *pt, t_maintools *tools)
{
	if (pt->pid == 0)
	{
		pipe_func(pt, tools);
		exit(tools->exit_status);
	}
	else
	{
		pt->pids[pt->i] = pt->pid;
		if (pt->i > 0)
			close(pt->prev_fd);
		if (pt->i < pt->num_cmds - 1)
		{
			close(pt->pipefd[1]);
			pt->prev_fd = pt->pipefd[0];
		}
	}
}

int	pipe_main(t_pipetools *pt, t_maintools *tools)
{
	pt->num_cmds = 0;
	while (tools->cmds[pt->num_cmds] != NULL)
		pt->num_cmds++;
	pt->pids = malloc(pt->num_cmds * sizeof(pid_t));
	if (pt->pids == NULL)
	{
		perror("malloc");
		return (0);
	}
	while (tools->cmds[pt->i] != NULL)
	{
		if (pipe_fork(pt) == 0)
		{
			free(pt->pids);
			return (0);
		}
		pipe_work(pt, tools);
		pt->i++;
	}
	return (1);
}
