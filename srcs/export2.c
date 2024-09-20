/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 17:28:20 by fdahouk           #+#    #+#             */
/*   Updated: 2024/09/21 01:10:08 by marvin           ###   ########.fr       */
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
	{
		ft_putstr_fd(args[i], 2);
		write(2, "not valid idenfier\n", 19);
		// printf("%s: not a valid identifier\n", args[i]);
	}
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

void	add_exp(t_maintools *tools, char ***ex, char ***en, int sf)
{
	int		i;
	char	*temp;
	int		f;

	i = 1;
	f = 0;
	while ((tools->strs)[i] != NULL)
	{
		temp = without_quotes_ret((tools->strs)[i], 0);
		free((tools->strs)[i]);
		(tools->strs)[i] = temp;
		if (!exp_arg_check((tools->strs)[i]))
		{
			if ((tools->strs)[i][0] == '-')
			{
				write(2, "invalid option\n", 15);
				ft_putstr_fd((tools->strs)[i], 2);
				// printf("%s: invalid option\n", (tools->strs)[i]);
				tools->exit_status = 2;
				f = 1;
			}
			else
			{
				write(2, "not valid idenfier\n", 19);
				ft_putstr_fd((tools->strs)[i], 2);
				// printf("%s: not a valid identifier\n", (tools->strs)[i]);
				tools->exit_status = 1;
				f = 1;
			}
		}
		else
		{
			if (check_equal((tools->strs)[i]) == -1)
			{
				if (ret_s_index((tools->strs)[i], *ex) == -1)
					*ex = export_enc(*ex, (tools->strs)[i]);
			}
			else
				func_add_exp(i, en, (tools->strs), ex);
		}
		i++;
	}
	if (f == 0 && sf == 0)
		tools->exit_status = 0;
}

void	edit_pwd(char ***ex, char ***en)
{
	char	*pwd;
	t_maintools t_tools;

	pwd = getcwd(NULL, 0);
	t_tools.strs = malloc(3 * sizeof(char *));
	t_tools.strs[0] = malloc(5);
	ft_strcpy(t_tools.strs[0], "jnde");
	t_tools.strs[1] = malloc(5 + ft_strlen(pwd) + 1);
	ft_strcpy(t_tools.strs[1], "PWD=");
	strncat(t_tools.strs[1], pwd, ft_strlen(pwd));
	t_tools.strs[2] = NULL;
	add_exp(&t_tools, ex, en, 1);
	free(pwd);
	free_args(&t_tools.strs);
}
