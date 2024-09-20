/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 19:07:27 by mjoundi           #+#    #+#             */
/*   Updated: 2024/09/21 02:01:09 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int exit_status = 0;

// if (tools->in == -1 || tools->out == -1)
// {
// 	printf("Error: unable to duplicate file descriptors\n");
// 	// free_all();
// 	exit(1);
// }
void	ini_tools_1(t_maintools *tools, char **env)
{
	tools->out = dup(STDOUT_FILENO);
	tools->in = dup(STDIN_FILENO);
	tools->exit_status = 0;
	setup_signals();
	tools->cd = getcwd(NULL, 0);
	tools->en = env_copy(env);
	tools->ex = env_copy(env);
	exp_q(tools->ex);
	sort_env(tools->ex, args_len(tools->ex));
	inc_shlvl(&tools->ex, &tools->en);
}

int	ini_loop(t_maintools *tools)
{
	tools->strs = NULL;
	tools->tmp.fd = NULL;
	tools->tmp.tmp = NULL;
	tools->str = readline("mjoundi > ");
	if (!tools->str)
		return (-1);
	if (empty(tools->str))
	{
		free(tools->str);
		return (0);
	}
	if (*tools->str)
		add_history(tools->str);
	return (1);
}

int	check_before_run(t_maintools *tools)
{
	if (!valquotes(tools->str))
	{
		write(2, "invalid quotes\n", 15);
		free(tools->str);
		tools->exit_status = 2;
		return (0);
	}
	tools->tmp.fd = red_run(&tools->str, &tools->tmp, tools->en, &tools->exit_status);
	if (tools->tmp.fd == NULL)
	{
		if (tools->tmp.tmp)
			free(tools->tmp.tmp);
		free(tools->str);
		return (0);
	}
	return (1);
}

void	exitt(t_maintools *tools)
{
	tools->str = rm_dl(tools->str);
	var_in_env(&tools->str, tools->en, &tools->exit_status);
	tools->str = rm_bs(tools->str);
	tools->strs = parse_args(tools->str, "exit");
	printf("exit\n");
	exit_args_check(tools->strs, &tools->exit_status);
}

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
	if (count_args(tools->str, "export") == 0)
	{
		print_exp(tools->ex);
		tools->exit_status = 0;
	}
	else
	{
		tools->str = rm_dl(tools->str);
		var_in_env(&tools->str, tools->en, &tools->exit_status);
		tools->str = rm_bs(tools->str);
		tools->strs = parse_args(tools->str, "export");
		add_exp(tools, &tools->ex, &tools->en, 0);
		free_args(&tools->strs);
	}
}

int	cd(t_maintools *tools)
{
	tools->str = rm_dl(tools->str);
	var_in_env(&tools->str, tools->en, &tools->exit_status);
	tools->str = rm_bs(tools->str);
	if (count_args(tools->str, "cd") > 1)
	{
		tools->strs = parse_args(tools->str, "cd");
		write(2, "bash: cd: too many arguments\n", 29);
		free(tools->str);
		tools->exit_status = 1;
		return (0);
	}
	if (count_args(tools->str, "cd") == 0)
	{
		if (chdir(env_search("HOME", tools->en)) == 0)
			tools->cd = getcwd(NULL, 0);
		else
		{
			write(2, "cd: ", 4);
			ft_putstr_fd(env_search("HOME", tools->en), 2);
			write(2, " No such file or directory\n", 26);
			// printf("cd: %s : No such file or directory\n",
			// 	env_search("HOME", tools->en));
			tools->exit_status = 1;
		}
		free(tools->str);
		return (0);
	}
	tools->strs = parse_args(tools->str, "cd");
	do_cd(tools->en, tools->strs[1], &tools->cd, &tools->exit_status);
	free_args(&tools->strs);
	return (1);
}

void	unset(t_maintools *tools)
{
	tools->str = rm_dl(tools->str);
	var_in_env(&tools->str, tools->en, &tools->exit_status);
	tools->str = rm_bs(tools->str);
	tools->strs = parse_args(tools->str, "unset");
	rm_exp(tools->strs, &tools->ex, &tools->en);
	free_args(&tools->strs);
}

void	exec(t_maintools *tools)
{
	if (!empty(tools->str))
	{
		tools->str = rm_dl(tools->str);
		var_in_env(&tools->str, tools->en, &tools->exit_status);
		tools->str = rm_bs(tools->str);
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

void	clean(t_maintools *tools)
{
	del_temp(tools->tmp.tmp, tools->en);
	fix_after_red(tools->in, tools->out, tools->tmp.fd);
	edit_pwd(&tools->ex, &tools->en);
	if (tools->str)
		free(tools->str);
	if (tools->tmp.fd)
		free(tools->tmp.fd);
}

void	freee(t_maintools *tools)
{
	if (tools->tmp.fd)
		free(tools->tmp.fd);
	if (tools->str)
		free(tools->str);
	if (tools->strs)
		free_args(&tools->strs);
	if (tools->en)
		free_args(&tools->en);
	if (tools->ex)
		free_args(&tools->ex);
	if (tools->cd)
		free(tools->cd);
}

int	is_num(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while(str[i] != '\0')
	{
		if (!(str[i] >= '0' && str[i] <= '9'))
			return (0);
		i++;
	}
	return (1);
}

void	ex_st(char **args, int *exit_status)
{
	long long	l;
	char		*temp;

	if (args_len(args) == 2)
	{
		temp = without_quotes_ret(args[1], 0);
		if (!is_num(temp))
			*exit_status = 2;
		else
		{
			l = ft_atol(args[1]);
			if (l < 0 && l >= -9223372036854775807LL - 1)
				l += 256 * ((-l / 256) + 1);
			if (l <= 9223372036854775807LL && l >= -9223372036854775807LL - 1)
				*exit_status = l % 256;
			else
			{
				write(2, "bash: exit: ", 12);
				write(2, args[1], ft_strlen(args[1]));
				write(2, ": numeric argument required\n", 25);
				// printf("bash: exit: %s: numeric argument required", args[1]);
				*exit_status = 2;
			}
		}
		free(temp);
	}
	else if (args_len(args) > 2)
	{
		temp = without_quotes_ret(args[1], 0);
		if (!is_num(temp))
			*exit_status = 2;
		else
		{
			l = ft_atol(args[1]);
			if (l <= 9223372036854775807LL && l >= -9223372036854775807LL - 1)
				*exit_status = 1;
			else
				*exit_status = 2;
		}
		free(temp);
	}
}

int	main_main(t_maintools *tools)
{
	if (parse_cmd(tools->str, "exit") >= 0)
	{
		exitt(tools);
		ex_st(tools->strs, &tools->exit_status);
		return (0);
	}
	else if (parse_cmd(tools->str, "echo") >= 0)
		echo(tools);
	else if (parse_cmd(tools->str, "env") >= 0)
		envv(tools->en, &tools->exit_status);
	else if (parse_cmd(tools->str, "export") >= 0)
		export(tools);
	else if (parse_cmd(tools->str, "pwd") >= 0)
		get_pwd(tools->str, &tools->exit_status);
	else if (parse_cmd(tools->str, "cd") >= 0)
	{
		if (cd(tools) == 0)
			return (0);
	}
	else if (parse_cmd(tools->str, "unset") >= 0)
		unset(tools);
	else
		exec(tools);
	printf("%d\n\n", tools->exit_status); 
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
		return (1);
	clean(tools);
	return (0);
}

int	main(int ac, char **av, char **env)
{
	t_maintools	tools;
	int			test;

	(void)ac;
	(void)av;
	ini_tools_1(&tools, env);
	while (1)
	{
		test = ini_loop(&tools);
		if (test == -1)
			break ;
		else if (test == 0)
			continue ;
		if (pipe_check(tools.str) == 0)
		{
			test = run_one_cmd(&tools);
			if (test == -1)
				continue ;
			else if (test == 2)
				break ;
		}
		else
		{
			printf("pipe is availble\n");
		}
	}
	printf("exit\n");
	freee(&tools);
	return (0);
}
