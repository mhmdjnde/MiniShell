/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_func.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 04:44:55 by marvin            #+#    #+#             */
/*   Updated: 2024/11/10 04:44:55 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**ini_rem(int index, char **env, int *i, int *j)
{
	char	**new;

	if (index < 0 || index >= args_len(env))
		return (NULL);
	new = (char **)malloc(sizeof(char *) * args_len(env));
	if (!new)
		return (NULL);
	*i = 0;
	*j = 0;
	return (new);
}

char	**remove_at_s(char **env, int index)
{
	int		i;
	int		j;
	char	**new;

	new = ini_rem(index, env, &i, &j);
	if (new == NULL)
		return (NULL);
	while (i < args_len(env))
	{
		if (i != index)
		{
			new[j] = malloc(ft_strlen(env[i]) + 1);
			if (!new[j])
			{
				free_in_error(new, &i);
				return (NULL);
			}
			ft_strcpy(new[j], env[i]);
			j++;
		}
		i++;
	}
	new[j] = NULL;
	free_args(&env);
	return (new);
}

void	free_three(char *t, char *t2, char *t3)
{
	free(t);
	free(t2);
	free(t3);
}

int	printing_with_return(int *exit_status)
{
	write(2, "invalid option\n", 15);
	*exit_status = 2;
	return (1);
}

void	inc_and_free(int *i, char *t3, char *t2, char *t)
{
	(*i)++;
	free_three(t, t2, t3);
}
