/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 23:18:01 by marvin            #+#    #+#             */
/*   Updated: 2024/09/13 23:18:01 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exp_q(char **ex)
{
	int		i;

	i = 0;
	while (ex[i] != NULL)
	{
		ex[i] = add_quotes(ex[i]);
		i++;
	}
}

int	empty(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] != ' ' && str[i] != '\t'
			&& str[i] != '\n' && str[i] != '\r')
			return (0);
		i++;
	}
	return (1);
}

void	q_args(char **args)
{
	int		i;
	char	*temp;

	i = 0;
	while (args[i] != NULL)
	{
		temp = args[i];
		args[i] = without_quotes_ret(temp, 0);
		free(temp);
		i++;
	}
}

void	q_args2(char **args)
{
	int		i;
	char	*temp;

	i = 1;
	while (args[i] != NULL)
	{
		temp = args[i];
		args[i] = without_quotes_ret(temp, 0);
		free(temp);
		i++;
	}
}

int	*red_run(char **str, t_tmptools *tmp, char **en)
{
	t_redtools	*red;
	int			*fd;
	int			i;

	i = 0;
	red = red_after_cmd(str);
	while (red != NULL && red[i].file != NULL)
	{
		tmp->tmp = red[i].file;
		red[i].file = without_quotes_ret(tmp->tmp, 0);
		i++;
		free(tmp->tmp);
	}
	if (red == NULL)
		return (NULL);
	tmp->tmp = heredoc(&red, en);
	fd = func_red(red);
	if (fd == NULL)
	{
		free_red(red);
		return (NULL);
	}
	free_red(red);
	return (fd);
}

void	setup_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = handle_sigint;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

int	fix_after_red(int in, int out, int *fd)
{
	if (dup2(out, STDOUT_FILENO) == -1 || dup2(in, STDIN_FILENO) == -1)
	{
		perror("dup2");
		return (-1);
	}
	if (fd[1] > 0)
		close(fd[1]);
	if (fd[0] > 0)
		close(fd[0]);
	return (1);
}

void	del_temp(char *tp, char **en)
{
	char	**args;

	if (tp != NULL)
	{
		args = malloc(3 * sizeof(char *));
		if (args == NULL)
			return ;
		args[0] = ft_strdup("rm");
		args[1] = ft_strdup(tp);
		args[2] = NULL;
		check_ve(args, en, 1);
	}
}
