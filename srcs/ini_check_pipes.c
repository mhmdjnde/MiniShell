/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ini_check_pipes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 04:09:02 by marvin            #+#    #+#             */
/*   Updated: 2024/11/10 04:09:02 by marvin           ###   ########.fr       */
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
