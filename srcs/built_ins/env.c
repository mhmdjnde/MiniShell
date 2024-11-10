/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 19:26:53 by mjoundi           #+#    #+#             */
/*   Updated: 2024/11/10 03:03:02 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_in_error(char **new, int *i)
{
	while ((*i)-- > 0)
		free(new[*i]);
	free(new);
}

//copy the original env
char	**env_copy(char **env)
{
	int		rows;
	char	**new;
	int		i;

	i = 0;
	rows = 0;
	while (env[rows] != NULL)
		rows++;
	new = (char **)malloc(sizeof(char *) * (rows + 1));
	if (!new)
		return (NULL);
	while (i < rows)
	{
		new[i] = malloc(ft_strlen(env[i]) + 1);
		if (!new[i])
		{
			free_in_error(new, &i);
			return (NULL);
		}
		ft_strcpy(new[i], env[i]);
		i++;
	}
	new[i] = NULL;
	return (new);
}

void	print_exp(char **ex)
{
	int	i;

	i = 0;
	while (ex[i] != NULL)
	{
		printf("declare -x %s\n", ex[i]);
		i++;
	}
}
