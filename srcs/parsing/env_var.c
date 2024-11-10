/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 02:22:20 by marvin            #+#    #+#             */
/*   Updated: 2024/11/10 02:22:20 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//this function return the name of the varriable after
//the '$' sign in the string
char	*var_ret(char *str, int *i)
{
	int		index[2];
	char	*temp;
	char	*t;

	index[0] = -1;
	index[1] = 0;
	t = var_ret_func(str, i, index);
	if (t == NULL)
		return (NULL);
	else if (ft_strcmp(t, "$$") == 0)
		return ("dbldlr");
	else if (ft_strcmp(t, "$?") == 0)
		return ("dlrqm");
	if (index[0] == -1 || index[0] == index[1])
		return (NULL);
	temp = (char *)malloc((index[1] - index[0] + 1) * sizeof(char));
	if (!temp)
		return (NULL);
	ft_strncpy(temp, str + index[0], index[1] - index[0]);
	temp[index[1] - index[0]] = '\0';
	return (temp);
}

//take a string and add '=' at the end
char	*add_equal(char *str)
{
	char	*temp;

	temp = malloc(ft_strlen(str) + 2);
	if (!temp)
		return (NULL);
	ft_strcpy(temp, str);
	temp[ft_strlen(str)] = '=';
	temp[ft_strlen(str) + 1] = '\0';
	return (temp);
}

//make sure if the variable as parameter is in env
char	*env_search(char *str, char **env)
{
	char	*tos;
	int		i;

	tos = add_equal(str);
	i = 0;
	if (!tos)
		return (NULL);
	while (env[i])
	{
		if (ft_strncmp(env[i], tos, ft_strlen(tos)) == 0)
		{
			free(tos);
			return (env[i] + ft_strlen(str) + 1);
		}
		i++;
	}
	free(tos);
	return (NULL);
}
