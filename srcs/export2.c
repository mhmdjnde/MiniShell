/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 17:28:20 by fdahouk           #+#    #+#             */
/*   Updated: 2024/09/20 01:02:44 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ret_s_index(char *str, char **env)
{
	int	i;

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

int	exp_arg_check(char *str)
{
	int		i;
	char	*temp;

	temp = without_quotes_ret(str, 0);
	if (!(temp[0] >= 'a' && temp[0] <= 'z')
		&& !(temp[0] >= 'A' && temp[0] <= 'Z') && temp[0] != '_')
	{
		free(temp);
		return (0);
	}
	i = 1;
	while (temp[i] != '=' && temp[i] != '\0')
	{
		if (!is_ok(temp[i]))
		{
			free(temp);
			return (0);
		}
		i++;
	}
	free(temp);
	return (1);
}

char	*ret_to_equal(char *str)
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

void	check_availble(int idx, char ***en, char **args, int i)
{
	if (idx == -1)
		*en = export_enc(*en, args[i]);
	else
	{
		free((*en)[idx]);
		(*en)[idx] = ft_strdup(args[i]);
	}
}

void	func_add_exp(int i, char ***en, char **args, char ***ex)
{
	char	*prefix;
	int		idx;

	if (check_equal(args[i]) == 0)
		printf("%s: not a valid identifier\n", args[i]);
	else
	{
		prefix = ret_to_equal(args[i]);
		idx = ret_s_index(prefix, *en);
		check_availble(idx, en, args, i);
		free(prefix);
		args[i] = add_quotes(args[i]);
		prefix = ret_to_equal(args[i]);
		idx = ret_s_index(prefix, *ex);
		check_availble(idx, ex, args, i);
		free(prefix);
	}
}

void	add_exp(char **args, char ***ex, char ***en, int sf)
{
	int		i;
	char	*temp;
	int		f;

	i = 1;
	f = 0;
	while (args[i] != NULL)
	{
		temp = without_quotes_ret(args[i], 0);
		free(args[i]);
		args[i] = temp;
		if (!exp_arg_check(args[i]))
		{
			if (args[i][0] == '-')
			{
				printf("%s: invalid option\n", args[i]);
				exit_status = 2;
				f = 1;
			}
			else
			{
				printf("%s: not a valid identifier\n", args[i]);
				exit_status = 1;
				f = 1;
			}
		}
		else
		{
			if (check_equal(args[i]) == -1)
			{
				if (ret_s_index(args[i], *ex) == -1)
					*ex = export_enc(*ex, args[i]);
			}
			else
				func_add_exp(i, en, args, ex);
		}
		i++;
	}
	if (f == 0 && sf == 0)
		exit_status = 0;
}

void	edit_pwd(char ***ex, char ***en)
{
	char	**args;
	char	*pwd;

	pwd = getcwd(NULL, 0);
	args = malloc(3 * sizeof(char *));
	args[0] = malloc(5);
	ft_strcpy(args[0], "jnde");
	args[1] = malloc(5 + ft_strlen(pwd) + 1);
	ft_strcpy(args[1], "PWD=");
	strncat(args[1], pwd, ft_strlen(pwd));
	args[2] = NULL;
	add_exp(args, ex, en, 1);
	free(pwd);
	free_args(&args);
}
