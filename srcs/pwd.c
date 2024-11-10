/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 01:20:43 by marvin            #+#    #+#             */
/*   Updated: 2024/11/10 01:20:43 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_free_cwd(char *cwd)
{
	printf("%s\n", cwd);
	free(cwd);
}

void	free_in_pwd(char **args, char *temp)
{
	free(temp);
	free_args(&args);
}

//this function edit the PWD variable in the env
//but the difference between this one the the other
//add_pwd function is that in this one we have the pass as parameter
//and here we are addng only when we have the PWD variable already there
void	add_pwd_2(char *pwd, t_maintools *tools)
{
	t_maintools	t_tools;

	if (env_search("PWD", tools->en) != NULL)
	{
		t_tools.strs = malloc(3 * sizeof (char *));
		if (!t_tools.strs)
			exit(1);
		t_tools.strs[0] = ft_strdup("jnde");
		t_tools.strs[1] = malloc(5 + ft_strlen(pwd) + 1);
		ft_strcpy(t_tools.strs[1], "PWD=");
		ft_strncat(t_tools.strs[1], pwd, ft_strlen(pwd));
		t_tools.strs[2] = NULL;
		add_exp(&t_tools, &tools->ex, &tools->en, 0);
		free_args(&t_tools.strs);
	}
}

//this function  is used to get the current working directory
// if the current directory is not found(get deleted) then we try
//getting the current directory from the  environment variables
//if the PWD variable is deleted we return NULL
char	*get_pwd2(char **env, int	*exit_status)
{
	char	*cur;

	cur = getcwd(NULL, 0);
	if (cur != NULL)
		return (cur);
	else
	{
		cur = ft_strdup("$PWD");
		var_in_env(&cur, env, exit_status);
		if (!empty(cur))
			return (cur);
		else
		{
			free(cur);
			return (NULL);
		}
	}
}

// this is the functionality of the pwd command
// we check in case of invalid flag adn then we get the 
// current working directory
void	get_pwd(char *str, int *exit_status, char **en)
{
	char	*cwd;
	char	**args;
	char	*temp;

	if (count_args(str, "pwd") != 0)
	{
		args = parse_args(str, "pwd");
		temp = without_quotes_ret(args[1], 0);
		if (temp[0] == '-')
		{
			write(2, "invalid flag\n", 13);
			free_in_pwd(args, temp);
			*exit_status = 2;
			return ;
		}
		else
			free_in_pwd(args, temp);
	}
	cwd = get_pwd2(en, exit_status);
	if (cwd != NULL)
		print_free_cwd(cwd);
	*exit_status = 0;
}
