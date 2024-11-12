/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   oldpwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 02:10:50 by marvin            #+#    #+#             */
/*   Updated: 2024/11/10 02:10:50 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//this function is used to return the OLDPWD fromt he environment
char	*get_oldpwd(char **env)
{
	int		exit_status;
	char	*en_old;

	en_old = ft_strdup("$OLDPWD");
	var_in_env(&en_old, env, &exit_status);
	if (!empty(en_old))
		return (en_old);
	else
	{
		free(en_old);
		return (NULL);
	}
}

//this is the functionality of the '-' case in cd
//it will get the OLDPWD and go to that directory
//also it will check if OLDPWD is not set
void	minus_func(char **old, char **env, int *exit_status, char **current_dir)
{
	if (*old != NULL)
	{
		*current_dir = get_pwd2(env, exit_status);
		if (chdir(*old) == 0)
		{
			printf("%s\n", *old);
			free(*old);
			*old = *current_dir;
		}
		else
		{
			*exit_status = 1;
			write(2, "cd: ", 4);
			ft_putstr_fd(*old, 2);
			write(2, "No such file or directory\n", 26);
			free(*current_dir);
		}
		*exit_status = 0;
	}
	else
	{
		ft_putstr_fd("OLDPWD not settt\n", 2);
		*exit_status = 1;
	}
}

//the main function of the '-' case for cd
void	minuscase(char **old, int *exit_status, char **env)
{
	char		*current_dir;
	char		*en_old;
	static int	f;

	en_old = get_oldpwd(env);
	if (en_old == NULL && f == 0)
	{
		ft_putstr_fd("OLDPWD not set\n", 2);
		free(*old);
		*old = NULL;
		f = 1;
		*exit_status = 1;
		return ;
	}
	else if (en_old != NULL)
	{
		free(*old);
		*old = en_old;
		f = 0;
	}
	minus_func(old, env, exit_status, &current_dir);
}
