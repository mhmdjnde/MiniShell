/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_tools.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 04:42:13 by marvin            #+#    #+#             */
/*   Updated: 2024/11/10 04:42:13 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ret_s_index2(char *str, char **env)
{
	int		i;

	i = 0;
	while (env[i] != NULL)
	{
		if (ft_strncmp(env[i], str, ft_strlen(str)) == 0
			&& ft_strlen(env[i]) >= ft_strlen(str)
			&& (env[i][ft_strlen(str)] == '='
			|| env[i][ft_strlen(str)] == '\0'))
			return (i);
		i++;
	}
	return (-1);
}

int	ret_s_indexx(char *str, char **env)
{
	int		i;

	i = 0;
	while (env[i] != NULL)
	{
		if (ft_strncmp(env[i], str, ft_strlen(str)) == 0)
		{
			return (i);
		}
		i++;
	}
	return (-1);
}

char	*ret_to_equal2(char *str)
{
	int		i;
	char	*r;

	i = 0;
	while (str[i] != '=' && str[i] != '\0')
		i++;
	r = (char *)malloc(sizeof(char) * (i + 1));
	if (!r)
		return (NULL);
	ft_strncpy(r, str, i);
	r[i] = '\0';
	return (r);
}

int	check_unset(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '=' || str[i] == '+')
			return (i);
		i++;
	}
	return (-1);
}

int	flag_check(char *str)
{
	int		i;
	char	q;

	i = 0;
	if (str[i] == '-')
		return (-1);
	else if (str[i] == '"' || str[i] == '\'')
	{
		q = str[i];
		i++;
		while (str[i] != '\0' && str[i] != q)
			i++;
		if (str[i] == '-')
			return (-1);
	}
	return (1);
}
