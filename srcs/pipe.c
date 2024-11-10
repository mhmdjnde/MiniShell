/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 15:22:05 by marvin            #+#    #+#             */
/*   Updated: 2024/11/10 04:15:30 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	wait_pipe(t_maintools *tools, t_pipetools *pt)
{
	pt->i = 0;
	while (pt->i < pt->num_cmds)
	{
		waitpid(pt->pids[pt->i], &pt->status, 0);
		pt->i++;
	}
	tools->exit_status = WEXITSTATUS(pt->status);
}

void	pipes_helper(t_maintools *tools, t_pipetools *pt)
{
	wait_pipe(tools, pt);
	free(pt->pids);
	pt->i = 0;
	while (pt->tmps[pt->i] != NULL)
	{
		del_temp(pt->tmps[pt->i], tools->en);
		pt->i++;
	}
	free_args(&pt->tmps);
	if (pt->prev_fd != -1)
		close(pt->prev_fd);
	setup_signals();
}

void	syn_err(t_maintools *tools, t_pipetools *pt)
{
	tools->exit_status = 2;
	pt->i = 0;
	while (pt->tmps[pt->i] != NULL)
	{
		del_temp(pt->tmps[pt->i], tools->en);
		pt->i++;
	}
	free_args(&pt->tmps);
}

void	run_pipes(t_maintools *tools)
{
	t_pipetools	pt;

	pt.i = 0;
	tools->pf = 1;
	pt.tmps = NULL;
	pt.prev_fd = -1;
	tools->cmds = parse_pipe(tools->str);
	free(tools->str);
	if (tools->cmds == NULL)
	{
		tools->exit_status = 2;
		return ;
	}
	if (check_token_err(tools->cmds, tools, &pt.tmps) == 0)
	{
		syn_err(tools, &pt);
		return ;
	}
	her_pipe(tools, &pt.tmps);
	pt.num_cmds = 0;
	if (pipe_main(&pt, tools) == 0)
		return ;
	pipes_helper(tools, &pt);
}
