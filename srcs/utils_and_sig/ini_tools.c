/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ini_tools.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 03:27:01 by marvin            #+#    #+#             */
/*   Updated: 2024/11/10 03:27:01 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		if (t->tmp.tmp && t->pf != 1)
			free(t->tmp.tmp);
		free(t->str);
		return (0);
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
	tools->of = 0;
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
	tools->str = readline("\001" /* start non-printable */
                         "\033[0;32mMini\033[0;31mHell> \033[0m" 
                         "\002");
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
