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

void	get_pwd(char *str, int *exit_status)
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
			free(temp);
			free_args(&args);
			*exit_status = 2;
			return ;
		}
		else
		{
			free(temp);
			free_args(&args);
		}
	}
	cwd = getcwd(NULL, 0);
	if (cwd != NULL)
	{
		printf("%s\n", cwd);
		free(cwd);
	}
	*exit_status = 0;
}

void	tildacase(char **env, char **old, int *exit_status)
{
	if (chdir(env_search("HOME", env)) == 0)
		*old = getcwd(NULL, 0);
	else
	{
		*exit_status = 1;
		write(2, "cd: ", 4);
		ft_putstr_fd(env_search("HOME", env), 2);
		write(2, "No such file or directory\n", 26);
	}
}

void	minuscase(char **old, int *exit_status)
{
	char	*current_dir;

	if (*old != NULL)
	{
		current_dir = getcwd(NULL, 0);
		if (chdir(*old) == 0)
		{
			printf("%s\n", *old);
			free(*old);
			*old = current_dir;
		}
		else
		{
			*exit_status = 1;
			write(2, "cd: ", 4);
			ft_putstr_fd(*old, 2);
			write(2, "No such file or directory\n", 26);
			free(current_dir);
		}
	}
}

void	no_file(char *str, char *current_dir, int *exit_status)
{
	*exit_status = 1;
	write(2, "cd: ", 4);
	ft_putstr_fd(str, 2);
	write(2, "No such file or directory\n", 26);
	free(current_dir);
}

void	do_cd(char **env, char *str, char **old, int *exit_status)
{
	char	*current_dir;

	current_dir = getcwd(NULL, 0);
	if (ft_strlen(str) == 1 && str[0] == '~')
	{
		tildacase(env, old, exit_status);
		free(current_dir);
	}
	else if (ft_strlen(str) == 1 && str[0] == '-')
	{
		minuscase(old, exit_status);
		free(current_dir);
	}
	else
	{
		if (chdir(str) == 0)
		{
			free(*old);
			*old = current_dir;
			*exit_status = 0;
		}
		else
		{
			no_file(str, current_dir, exit_status);
		}
	}
}
