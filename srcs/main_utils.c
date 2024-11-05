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
			&& str[i] != '\n' && str[i] != '\r'
			&& str[i] != ':' && str[i] != '!')
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

int	*red_run(char **str, t_tmptools *tmp, char **en, t_maintools *t)
{
	t_redtools	*red;
	int			*fd;
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
		return (NULL);
	if (t->pf != 1)
		tmp->tmp = heredoc(&red, en);
	fd = func_red(red, &t->exit_status);
	if (fd == NULL)
	{
		free_red(red);
		return (NULL);
	}
	free_red(red);
	return (fd);
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
	int		exit_status;

	exit_status = 0;
	if (tp != NULL)
	{
		args = malloc(3 * sizeof(char *));
		if (args == NULL)
			return ;
		args[0] = ft_strdup("rm");
		args[1] = ft_strdup(tp);
		args[2] = NULL;
		check_ve(args, en, 1, &exit_status);
		free_args(&args);
	}
}
