/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 15:22:05 by marvin            #+#    #+#             */
/*   Updated: 2024/11/02 02:46:22 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pipe_check(char *str)
{
	int		i;
	char	q;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '"' || str[i] == '\'')
		{
			q = str[i];
			i++;
			while (str[i] != q && str[i] != '\0')
				i++;
			if (str[i] == q)
				i++;
		}
		else if (str[i] == '|')
			return (1);
		else
			i++;
	}
	return (0);
}

int	check_after_pipe(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0' && str[i] == ' ')
		i++;
	if (str[i] == '|' || str[i] == '\0')
		return (-1);
	return (1);
}

int	pipe_count(char *str)
{
	int	i;
	int	c;

	i = 0;
	c = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '|')
			c++;
		i++;
	}
	return (c);
}

void	ini_pipe(int *i, int **tab, char *str, char ***cmds)
{
	*i = 0;
	(*tab)[1] = 0;
	*cmds = malloc((pipe_count(str) + 2) * sizeof(char *));
	while (str[*i] != '\0' && str[*i] == ' ')
		(*i)++;
	if (str[*i] == '|')
	{
		printf("bash: syntax error near unexpected token `|'\n");
		free(*cmds);
		*cmds = NULL;
	}
	(*tab)[0] = *i;
}

int	fill_cmds(char *str, int *i, int **tab, char ***cmds)
{
	if (check_after_pipe(str + *i + 1) == -1)
	{
		printf("bash: syntax error near unexpected token `|'\n");
		return (0);
	}
	(*cmds)[(*tab)[1]++] = ft_substr(str, (*tab)[0], *i - (*tab)[0]);
	(*i)++;
	while (str[*i] != '\0' && str[*i] == ' ')
		(*i)++;
	(*tab)[0] = *i;
	return (1);
}

void	p_q(int *i, char *str)
{
	char	q;

	q = str[*i];
	(*i)++;
	while (str[*i] != q && str[*i] != '\0')
		(*i)++;
	if (str[*i] == q)
		(*i)++;
}

int	cmd_parse(char *str, int *i, int **tab, char ***cmds)
{
	if (fill_cmds(str, i, tab, cmds) == 0)
	{
		(*cmds)[(*tab)[1]] = NULL;
		free_args(cmds);
		return (free(*tab), -1);
	}
	return (1);
}

char	**parse_pipe(char *str)
{
	int		i;
	char	**cmds;
	int		*tab;

	tab = malloc(2 * sizeof(int));
	cmds = NULL;
	ini_pipe(&i, &tab, str, &cmds);
	if (cmds == NULL)
		return (free(tab), NULL);
	while (str[i] != '\0')
	{
		if (str[i] == '"' || str[i] == '\'')
			p_q(&i, str);
		else if (str[i] == '|')
		{
			if (cmd_parse(str, &i, &tab, &cmds) == -1)
				return (NULL);
		}
		else
			i++;
	}
	cmds[tab[1]] = ft_substr(str, tab[0], i - tab[0]);
	cmds[tab[1] + 1] = NULL;
	return (free(tab), cmds);
}

void	her_run(char **str, t_tmptools *tmp, char **en, t_maintools *t)
{
	t_redtools	*red;
	int			i;

	i = 0;
	red = red_after_cmd(str, &t->exit_status);
	while (red != NULL && red[i].file != NULL)
	{
		tmp->tmp = red[i].file;
		red[i].file = without_quotes_ret(tmp->tmp, 0);
		i++;
		free(tmp->tmp);
	}
	if (red == NULL)
		return ;
	tmp->tmp = heredoc(&red, en);
	free_red(red);
}

void	heredoc_before_err(t_maintools *t, int err_i, char ***tmps)
{
	int		i;
	char	**temp;

	i = 0;
	*tmps = (char **)malloc(1 * sizeof(char *));
	(*tmps)[0] = NULL;
	temp = env_copy(t->cmds);
	while (i < err_i && temp[i] != NULL)
	{
		her_run(&temp[i], &t->tmp, t->en, t);
		if (t->tmp.tmp)
		{
			replace_heredoc_with_file(&t->cmds[i], t->tmp.tmp);
			*tmps = export_enc(*tmps, t->tmp.tmp);
			free(t->tmp.tmp);
			t->tmp.tmp = NULL;
		}
		i++;
	}
	free_args(&temp);
}

void	her_pipe(t_maintools *t, char ***tmps)
{
	int		i;
	char	**temp;

	i = 0;
	temp = env_copy(t->cmds);
	*tmps = (char **)malloc(1 * sizeof(char *));
	(*tmps)[0] = NULL;
	while (temp[i] != NULL)
	{
		her_run(&temp[i], &t->tmp, t->en, t);
		if (t->tmp.tmp)
		{
			replace_heredoc_with_file(&t->cmds[i], t->tmp.tmp);
			*tmps = export_enc(*tmps, t->tmp.tmp);
			free(t->tmp.tmp);
			t->tmp.tmp = NULL;
		}
		i++;
	}
	free_args(&temp);
}

int	check_token_err(char **cmds, t_maintools *t, char ***tmps)
{
	t_redtools	*red;
	char		*temp;
	int			i;
	int			es;

	i = 0;
	es = 0;
	while (cmds[i] != NULL)
	{
		temp = ft_strdup(cmds[i]);
		red = red_after_cmd(&temp, &es);
		if (red == NULL)
		{
			heredoc_before_err(t, i, tmps);
			return (free(temp), 0);
		}
		free(temp);
		free_red(red);
		i++;
	}
	return (1);
}

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

void	init_pipe_tools(t_pipetools *pt, int num_cmds)
{
	pt->i = 0;
	pt->prev_fd = -1;
	pt->num_cmds = num_cmds;
	pt->pids = malloc(num_cmds * sizeof(pid_t));
	if (!pt->pids)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
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

void	wait_pipe(t_maintools *tools, t_pipetools *pt)
{
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
