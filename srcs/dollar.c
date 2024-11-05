/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 18:52:36 by mjoundi           #+#    #+#             */
/*   Updated: 2024/11/05 02:40:32 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//checking if its '$$' or the '$?' case
char	*dlr_qm(char *str, int *i)
{
	if (str[*i - 1] == '$')
		return ("$$");
	else
		return ("$?");
}

char	*increment_in_helper3(int *i, char *str)
{
	*i += 2;
	return (dlr_qm(str, i));
}

//parsing the variable name
int	helper2(char *str, int *i, int *index)
{
	if (*i != 0 && str[*i - 1] == '\\')
	{
		if (str[*i] != '\0')
			(*i)++;
		return (0);
	}
	index[0] = *i + 1;
	if (str[*i] != '\0')
		(*i)++;
	if (!(str[*i] >= 'a' && str[*i] <= 'z') && !(str[*i] >= 'A'
			&& str[*i] <= 'Z'))
	{
		if (str[*i] != '\0')
			(*i)++;
		return (0);
	}
	while (str[*i] != '\0' && is_ok(str[*i]))
		(*i)++;
	index[1] = *i;
	return (1);
}

//parsing the variable name
void	var_ret_helper1(char *str, int *i, int *double_quotes)
{
	if (*double_quotes == 0 && str[*i] == '"')
	{
		*double_quotes = 1;
		(*i)++;
	}
	else if (*double_quotes == 1 && str[*i] == '"')
	{
		*double_quotes = 1;
		(*i)++;
	}
	if (str[*i] == '\'' && *double_quotes == 0)
	{
		(*i)++;
		while (str[*i] != '\'' && str[*i] != '\0')
			(*i)++;
		if (str[*i] != '\0')
			(*i)++;
	}
}

//parsing the variable name
char	*helper3(char *str, int *i, int *index)
{
	int		double_quotes;
	char	*t;

	double_quotes = 0;
	while (str[*i] != '\0')
	{
		var_ret_helper1(str, i, &double_quotes);
		if (str[*i] == '$')
		{
			if (str[*i + 1] == '$' || str[*i + 1] == '?')
			{
				t = increment_in_helper3(i, str);
				if (t != NULL)
					return (t);
			}
			if (helper2(str, i, index) == 0)
				return (NULL);
			if (index[0] == index[1])
				continue ;
			break ;
		}
		if (str[*i] != '\0')
			(*i)++;
	}
	return (str);
}

//this function return the name of the varriable after
//the '$' sign in the string
char	*var_ret(char *str, int *i)
{
	int		index[2];
	char	*temp;
	char	*t;

	index[0] = -1;
	index[1] = 0;
	t = helper3(str, i, index);
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
	strcpy(temp, str);
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

//edit the str by changing the variables to its value
void	edit_str(char **str, int start, int end, char *env_value)
{
	int		new_len;
	char	*new_str;

	if (env_value)
		new_len = start + ft_strlen(env_value) + ft_strlen(*str + end);
	else
		new_len = start + ft_strlen(*str + end);
	new_str = (char *)malloc((new_len + 1) * sizeof(char));
	if (!new_str)
		return ;
	ft_strncpy(new_str, *str, start);
	new_str[start] = '\0';
	if (env_value)
		ft_strcat(new_str, env_value);
	ft_strcat(new_str, *str + end);
	free(*str);
	*str = new_str;
}

//the '$$' case to get the pid
void	double_dlr(int *start, int *i, int *end, char **str)
{
	char	*env_value;

	env_value = ft_itoa(get_pid());
	*start = *i - 2;
	*end = *i;
	edit_str(str, *start, *end, env_value);
	if (env_value)
		*i = *start + ft_strlen(env_value);
	else
		*i = *start;
	free(env_value);
}

// '$?' case to get the status of the last command
void	dollar_qm(int tab[3], char **str, int *exit_status)
{
	char	*env_value;

	env_value = ft_itoa(*exit_status);
	tab[1] = tab[0] - 2;
	tab[2] = tab[0];
	edit_str(str, tab[1], tab[2], env_value);
	if (env_value)
		tab[0] = tab[1] + ft_strlen(env_value);
	else
		tab[0] = tab[1];
	free(env_value);
}

void	restart_i(int *i, char *env_value, int start)
{
	if (env_value)
		*i = start + ft_strlen(env_value);
	else
		*i = start;
}

//main function of replasing the variables by its values
void	var_in_env(char **str, char **env, int *exit_status)
{
	char	*var_name;
	char	*env_value;
	int		tab[3];

	tab[0] = 0;
	while ((*str)[tab[0]] != '\0')
	{
		var_name = var_ret(*str, &tab[0]);
		if (var_name)
		{
			if (ft_strcmp(var_name, "dbldlr") == 0)
				double_dlr(&tab[1], &tab[0], &tab[2], str);
			else if (ft_strcmp(var_name, "dlrqm") == 0)
				dollar_qm(tab, str, exit_status);
			else
			{
				env_value = env_search(var_name, env);
				tab[1] = tab[0] - ft_strlen(var_name) - 1;
				tab[2] = tab[0];
				edit_str(str, tab[1], tab[2], env_value);
				restart_i(&tab[0], env_value, tab[1]);
				free(var_name);
			}
		}
	}
}
