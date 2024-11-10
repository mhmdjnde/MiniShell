/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 03:04:08 by marvin            #+#    #+#             */
/*   Updated: 2024/11/10 03:04:08 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//return the index of the variable in the env
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

//check the args of the export command
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

//return the variable name (to the '=')
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

//add to env functionality
void	func_add_exp(int i, char ***en, t_maintools *tools, char ***ex)
{
	char	*prefix;
	int		idx;

	if (check_equal(tools->strs[i]) == 0)
	{
		write(2, "Bash: export: ", 14);
		ft_putstr_fd(tools->strs[i], 2);
		write(2, " not valid idenfier\n", 20);
		tools->exit_status = 1;
	}
	else
	{
		prefix = ret_to_equal(tools->strs[i]);
		idx = ret_s_index(prefix, *en);
		check_availble(idx, en, tools->strs, i);
		free(prefix);
		tools->strs[i] = add_quotes(tools->strs[i]);
		prefix = ret_to_equal(tools->strs[i]);
		idx = ret_s_index(prefix, *ex);
		check_availble(idx, ex, tools->strs, i);
		free(prefix);
	}
}
