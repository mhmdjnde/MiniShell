/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 15:22:05 by marvin            #+#    #+#             */
/*   Updated: 2024/09/18 15:22:05 by marvin           ###   ########.fr       */
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

// int	main(void)
// {
// 	char *test_cases[] = {
// 		"echo hello |   | wc -l",                    // Test with a single pipe
// 		"echo 'hello | world' | wc -c",           // Test with a pipe inside quotes
// 		"ls -l | grep .c | wc -l",                // Test with multiple pipes
// 		"cat file.txt |",                         // Test with a pipe at the end (syntax error)
// 		"| echo hi",                              // Test with a pipe at the start (syntax error)
// 		NULL
// 	};

// 	for (int i = 0; test_cases[i] != NULL; i++)
// 	{
// 		printf("\nTest %d: %s\n", i + 1, test_cases[i]);
// 		char **cmds = parse_pipe(test_cases[i]);
// 		if (cmds == NULL)
// 		{
// 			printf("Error parsing command\n");
// 			continue;
// 		}

// 		for (int j = 0; cmds[j] != NULL; j++)
// 		{
// 			printf("Parsed command %d: %s\n", j + 1, cmds[j]);
// 			free(cmds[j]);  // Don't forget to free allocated memory
// 		}
// 		free(cmds);  // Free the array itself
// 	}

// 	return 0;
// }
