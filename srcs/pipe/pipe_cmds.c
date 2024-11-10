/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_cmds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 04:11:13 by marvin            #+#    #+#             */
/*   Updated: 2024/11/10 04:11:13 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
