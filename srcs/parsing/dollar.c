/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 18:52:36 by mjoundi           #+#    #+#             */
/*   Updated: 2024/11/10 02:22:02 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
