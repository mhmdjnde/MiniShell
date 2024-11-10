/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   pwd.c											  :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: mjoundi <mjoundi@student.42.fr>			+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/08/23 19:02:57 by fdahouk		   #+#	#+#			 */
/*   Updated: 2024/08/27 19:20:28 by mjoundi		  ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "minishell.h"

//this is the case of '~' for the cd
//it will get the HOME directory from the env adn then go to HOME
//if HOME is not set wi will  print an error message
void	tildacase(char **env, char **old, int *exit_status)
{
	if (chdir(env_search("HOME", env)) == 0)
		*old = get_pwd2(env, exit_status);
	else
	{
		*exit_status = 1;
		write(2, "cd: ", 4);
		ft_putstr_fd(env_search("HOME", env), 2);
		write(2, "No such file or directory\n", 26);
	}
}

//print an error message when no such file or directory
void	no_file(char *str, int *exit_status)
{
	*exit_status = 1;
	write(2, "cd: ", 4);
	ft_putstr_fd(str, 2);
	write(2, "No such file or directory\n", 26);
}

//main func of the cd command with its cases
void	cd_func(t_maintools *tools, char **old, char **current_dir, char *str)
{
	char	*t;
	char	*t2;

	t = without_quotes_ret(str, 0);
	if (chdir(t) == 0)
	{
		if (ft_strcmp(t, "..") == 0)
		{
			t2 = rm_last_dir(*current_dir);
			add_pwd_2(t2, tools);
			free(t2);
		}
		free(*old);
		*old = ft_strdup(*current_dir);
		tools->exit_status = 0;
	}
	else
		no_file(t, &tools->exit_status);
	free(*current_dir);
	free(t);
}

//checking the parameter of the cd command
void	do_cd(char **env, char *str, char **old, t_maintools *tools)
{
	char	*current_dir;

	current_dir = get_pwd2(env, &tools->exit_status);
	if (ft_strlen(str) == 1 && str[0] == '~')
	{
		tildacase(env, old, &tools->exit_status);
		free(current_dir);
	}
	else if (ft_strlen(str) == 1 && str[0] == '-')
	{
		minuscase(old, &tools->exit_status, env);
		free(current_dir);
	}
	else
		cd_func(tools, old, &current_dir, str);
}
