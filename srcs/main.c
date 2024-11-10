/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 19:07:27 by mjoundi           #+#    #+#             */
/*   Updated: 2024/11/10 23:22:31 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec(t_maintools *tools)
{
	if (!empty(tools->str))
	{
		tools->str = rm_dl(tools->str);
		var_in_env(&tools->str, tools->en, &tools->exit_status);
		tools->str = rm_bs(tools->str);
		if (!empty(tools->str))
		{
			tools->temp = get_cmd(tools->str);
			if (count_args(tools->str, tools->temp) != 0)
				tools->strs = parse_args(tools->str, tools->temp);
			else
			{
				tools->strs = malloc(2 * sizeof(char *));
				tools->strs[0] = ft_strdup(tools->temp);
				tools->strs[1] = NULL;
			}
			q_args(tools->strs);
			check_ve(tools->strs, tools->en, 0, &tools->exit_status);
			free_args(&tools->strs);
			free(tools->temp);
		}
	}
}

int	main_main(t_maintools *tools)
{
	if (parse_cmd(tools->str, "exit") >= 0)
	{
		exitt(tools);
		ex_st(tools->strs, &tools->exit_status);
		return (-1);
	}
	else if (parse_cmd(tools->str, "echo") >= 0)
		echo(tools);
	else if (parse_cmd(tools->str, "env") >= 0)
		envv(tools->en, &tools->exit_status);
	else if (parse_cmd(tools->str, "export") >= 0)
		export(tools);
	else if (parse_cmd(tools->str, "pwd") >= 0)
		get_pwd(tools->str, &tools->exit_status, tools->en);
	else if (parse_cmd(tools->str, "cd") >= 0)
	{
		if (cd(tools) == 0)
			return (0);
	}
	else if (parse_cmd(tools->str, "unset") >= 0)
		unset(tools);
	else
		exec(tools);
	return (1);
}

int	run_one_cmd(t_maintools *tools)
{
	int	test;

	if (check_before_run(tools) == 0)
		return (1);
	test = main_main(tools);
	if (test == -1)
		return (2);
	else if (test == 0)
	{
		if (tools->tmp.fd)
			free(tools->tmp.fd);
		return (1);
	}
	clean(tools);
	return (0);
}

void	main_loop(t_maintools *tools, int *test)
{
	while (1)
	{
		*test = ini_loop(tools);
		if (g_s == 2 || g_s == 3)
		{
			tools->exit_status = 128 + g_s;
			g_s = 0;
		}
		if (*test == -1)
			break ;
		else if (*test == 0)
			continue ;
		if (pipe_check(tools->str) == 0)
		{
			*test = run_one_cmd(tools);
			if (*test == 2)
				break ;
		}
		else
		{
			run_pipes(tools);
			if (tools->cmds != NULL)
				free_args(&tools->cmds);
		}
	}
}

int	main(int ac, char **av, char **env)
{
	t_maintools	tools;
	int			test;

	(void)ac;
	(void)av;
	ini_tools_1(&tools, env);
	main_loop(&tools, &test);
	rl_clear_history();
	clear_history();
	if (test != 2)
		printf("exit\n");
	free_tools(&tools);
	exit(tools.exit_status);
	return (0);
}
