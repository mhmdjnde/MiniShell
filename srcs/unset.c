/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 03:12:30 by marvin            #+#    #+#             */
/*   Updated: 2024/08/28 03:12:30 by marvin           ###   ########.fr       */
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

void	complete_if(char *t, char *t2, char ***en, char ***ex)
{
	if (ret_s_indexx(t2, *en) != -1)
		*en = remove_at_s(*en, ret_s_indexx(t2, *en));
	if (ret_s_index2(t, *ex) != -1)
		*ex = remove_at_s(*ex, ret_s_index2(t, *ex));
}

void	rm_exp(char **args, char ***ex, char ***en, int *exit_status)
{
	int		i;
	char	*t;
	char	*t2;
	char	*t3;

	i = 1;
	if (args_len(args) >= 2 && flag_check(args[1]) == -1
		&& printing_with_return(exit_status) == 1)
		return ;
	while (args[i] != NULL)
	{
		t3 = without_quotes_ret(args[i], 0);
		t = ret_to_equal2(t3);
		t2 = add_equal(t);
		if (check_unset(args[i]) != -1)
		{
			inc_and_free(&i, t3, t2, t);
			continue ;
		}
		complete_if(t, t2, en, ex);
		i++;
		free_three(t, t2, t3);
	}
	*exit_status = 0;
}
