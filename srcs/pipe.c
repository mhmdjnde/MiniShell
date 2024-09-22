/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 15:22:05 by marvin            #+#    #+#             */
/*   Updated: 2024/09/22 09:04:17 by marvin           ###   ########.fr       */
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

char	**parse_pipe(char *str)
{
	int		i;
	char	q;
	char	**cmds;
	int		j;
	int		k;

	i = 0;
	k = 0;
	cmds = malloc((pipe_count(str) + 2) * sizeof(char *));
	while (str[i] != '\0' && str[i] == ' ')
		i++;
	if (str[i] == '|')
	{
		printf("bash: syntax error near unexpected token `|'\n");
		return (NULL);
	}
	j = i;
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
		{
			if (check_after_pipe(str + i + 1) == -1)
			{
				printf("bash: syntax error near unexpected token `|'\n");
				return (NULL);
			}
			cmds[k++] = ft_substr(str, j, i - j);
			i++;
			while (str[i] != '\0' && str[i] == ' ')
				i++;
			j = i;
		}
		else
			i++;
	}
	cmds[k] = ft_substr(str, j, i - j);
	cmds[k + 1] = NULL;
	return (cmds);
}

int	check_token_err(char **cmds)
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
			return (0);
		free(temp);
		free_red(red);
		i++;
	}
	return (1);
}

void	run_pipes(t_maintools *tools)
{
	int		i;
	int		pipefd[2];
	int		prev_fd = -1;
	pid_t	pid;
	int		status;
	int		num_cmds;

	i = 0;
	tools->cmds = parse_pipe(tools->str);
	if (tools->cmds == NULL)
		return;
	if (check_token_err(tools->cmds) == 0)
		return;

	num_cmds = 0;
	while (tools->cmds[num_cmds] != NULL)
		num_cmds++;
	while (tools->cmds[i] != NULL)
	{
		if (i < num_cmds - 1)
		{
			if (pipe(pipefd) == -1)
			{
				perror("pipe");
				return;
			}
		}
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			return;
		}

		if (pid == 0)
		{
			if (i > 0)
			{
				dup2(prev_fd, STDIN_FILENO);
				close(prev_fd);
			}
			if (i < num_cmds - 1)
			{
				dup2(pipefd[1], STDOUT_FILENO);
				close(pipefd[0]);
				close(pipefd[1]);
			}
			tools->str = tools->cmds[i];
			run_one_cmd(tools);
			exit(EXIT_SUCCESS);
		}
		else
		{
			if (i > 0)
				close(prev_fd);
			if (i < num_cmds - 1)
			{
				close(pipefd[1]);
				prev_fd = pipefd[0];
			}
		}
		i++;
	}
	i = 0;
	while (i < num_cmds)
	{
		wait(&status);
		i++;
	}
	if (prev_fd != -1)
		close(prev_fd);
}

// int	main(void)
// {
// 	char *str = "echo jnde > jnde | >>>>>> |<  grep jnde";

// 	char **cmds =  parse_pipe(str);
// 	printf("%d\n\n",  check_token_err(cmds));
	
// 	return 0;
// }

// void	run_pipe(t_maintools *tools)
// {
	
// }

// void 	run_pipes(t_maintools *tools)
// {
// 	int		i;
// 	char	**cmds;

// 	i = 0;
// 	cmds = parse_pipe(tools->str);
// 	free(tools->str);
// 	while (cmds[i] != NULL)
// 	{
// 		tools->str = cmds[i];
// 		run_pipe(tools);
// 		i++;
// 	}
// }
