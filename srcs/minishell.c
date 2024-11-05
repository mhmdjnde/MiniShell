/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 19:07:27 by mjoundi           #+#    #+#             */
/*   Updated: 2024/11/02 02:38:02 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	while (str[i] != '\0')
	{
		if (!(str[i] >= '0' && str[i] <= '9'))
			return (0);
		i++;
	}
	return (1);
}

void	event_not_found(char *str, int *i, int *j)
{
	char	*t;

	(*i)++;
	*j = *i;
	while (str[*i] != '\0' && is_ok(str[*i]))
		(*i)++;
	t = ft_substr(str, *j, *i - *j);
	ft_putstr_fd(t, 2);
	write(2, " event not found\n", 17);
	free(t);
}

int	event_check(char *str)
{
	int		i;
	char	q;
	int		j;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '"' || str[i] == '\'')
		{
			q = str[i];
			i++;
			while (str[i] != '\0' && str[i] != q)
				i++;
		}
		else if (str[i] == '!')
		{
			event_not_found(str, &i, &j);
			return (0);
		}
		else
			i++;
	}
	return (1);
}

void	ini_tools_1(t_maintools *tools, char **env)
{
	tools->out = dup(STDOUT_FILENO);
	tools->in = dup(STDIN_FILENO);
	tools->exit_status = 0;
	setup_signals();
	tools->cd = getcwd(NULL, 0);
	tools->en = env_copy(env);
	tools->ex = env_copy(env);
	tools->cdf = 0;
	exp_q(tools->ex);
	sort_env(tools->ex, args_len(tools->ex));
	inc_shlvl(&tools->ex, &tools->en);
	edit_oldpwd(&tools->ex, &tools->en, tools);
	add_pwd(&tools->ex, &tools->en);
	tools->pf = 0;
}

int	ini_loop(t_maintools *tools)
{
	tools->strs = NULL;
	tools->tmp.fd = NULL;
	tools->tmp.tmp = NULL;
	tools->cdf = 0;
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
	if (event_check(tools->str) == 0)
	{
		free(tools->str);
		return (0);
	}
	tools->pf = 0;
	return (1);
}

int	check_before_run(t_maintools *t)
{
	if (!valquotes(t->str))
	{
		write(2, "invalid quotes\n", 15);
		free(t->str);
		t->exit_status = 2;
		return (0);
	}
	t->tmp.fd = red_run(&t->str, &t->tmp, t->en, t);
	if (t->tmp.fd == NULL)
	{
		if (t->tmp.tmp)
			free(t->tmp.tmp);
		free(t->str);
		return (0);
	}
	return (1);
}

void	print_in_cd(t_maintools *tools)
{
	write(2, "cd: ", 4);
	ft_putstr_fd(env_search("HOME", tools->en), 2);
	write(2, " No such file or directory\n", 27);
	tools->exit_status = 1;
}

void	print_ex_st(char **args, int *exit_status)
{
	write(2, "bash: exit: ", 12);
	write(2, args[1], ft_strlen(args[1]));
	write(2, ": numeric argument required\n", 25);
	*exit_status = 2;
}

void	check_exit_status(char *temp, char **args, int *exit_status)
{
	long long	l;

	l = 0;
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
	tools->strs = parse_args(tools->str, "echo");
	echo_args_check(tools->strs, tools);
	free_args(&tools->strs);
	tools->exit_status = 0;
}

void	export(t_maintools *tools)
{
	tools->str = rm_dl(tools->str);
	var_in_env(&tools->str, tools->en, &tools->exit_status);
	tools->str = rm_bs(tools->str);
	if (count_args(tools->str, "export") == 0)
	{
		print_exp(tools->ex);
		tools->exit_status = 0;
	}
	else
	{
		tools->strs = parse_args(tools->str, "export");
		add_exp(tools, &tools->ex, &tools->en, 0);
		free_args(&tools->strs);
	}
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

void	unset(t_maintools *tools)
{
	tools->str = rm_dl(tools->str);
	var_in_env(&tools->str, tools->en, &tools->exit_status);
	tools->str = rm_bs(tools->str);
	tools->strs = parse_args(tools->str, "unset");
	rm_exp(tools->strs, &tools->ex, &tools->en, &tools->exit_status);
	free_args(&tools->strs);
}

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

void	clean(t_maintools *tools)
{
	if (tools->pf != 1)
		del_temp(tools->tmp.tmp, tools->en);
	if (tools->tmp.tmp && tools->pf != 1)
		free(tools->tmp.tmp);
	fix_after_red(tools->in, tools->out, tools->tmp.fd);
	edit_oldpwd(&tools->ex, &tools->en, tools);
	edit_pwd(&tools->ex, &tools->en, tools);
	if (tools->str)
		free(tools->str);
	if (tools->tmp.fd)
		free(tools->tmp.fd);
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
				print_ex_st(args, exit_status);
		}
		free(temp);
	}
	else if (args_len(args) > 2)
	{
		temp = without_quotes_ret(args[1], 0);
		check_exit_status(temp, args, exit_status);
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
	freee(&tools);
	exit(tools.exit_status);
	return (0);
}
