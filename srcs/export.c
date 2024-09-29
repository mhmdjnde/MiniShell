/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjoundi <mjoundi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 19:01:12 by mjoundi           #+#    #+#             */
/*   Updated: 2024/08/27 19:01:12 by mjoundi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	copy_old(int *i, char ***new, int rows, char **env)
{
	while (*i < rows)
	{
		(*new)[*i] = malloc(ft_strlen(env[*i]) + 1);
		if (!(*new)[*i])
		{
			free_in_error((*new), i);
			return (0);
		}
		ft_strcpy((*new)[*i], env[*i]);
		(*i)++;
	}
	return (1);
}

char	**export_enc(char **env, char *var)
{
	int		rows;
	char	**new;
	int		i;

	rows = 0;
	while (env[rows] != NULL)
		rows++;
	new = (char **)malloc(sizeof(char *) * (rows + 2));
	if (!new)
		return (NULL);
	i = 0;
	if (copy_old(&i, &new, rows, env) == 0)
		return (NULL);
	new[i] = malloc(ft_strlen(var) + 1);
	if (!new[i])
	{
		free_in_error(new, &i);
		return (NULL);
	}
	ft_strcpy(new[i], var);
	new[++i] = NULL;
	free_args(&env);
	return (new);
}

void	sort_env(char **arr, int n)
{
	int		i;
	int		j;
	char	*temp;

	i = 0;
	while (i < n - 1)
	{
		j = i + 1;
		while (j < n)
		{
			if (ft_strcmp(arr[i], arr[j]) > 0)
			{
				temp = arr[i];
				arr[i] = arr[j];
				arr[j] = temp;
			}
			j++;
		}
		i++;
	}
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

int	check_equal(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '=')
			return (i);
		i++;
	}
	return (-1);
}

char	*add_quotes(char *str)
{
	int		i;
	char	*temp;
	int		j;

	i = 0;
	temp = malloc(ft_strlen(str) + 3);
	if (!temp)
		return (NULL);
	while (str[i] != '=' && str[i] != '\0')
	{
		temp[i] = str[i];
		i++;
	}
	temp[i++] = '=';
	temp[i++] = '"';
	j = i;
	i--;
	while (str[i] != '\0')
	{
		temp[j++] = str[i++];
	}
	temp[j] = '"';
	temp[j + 1] = '\0';
	free(str);
	return (temp);
}
