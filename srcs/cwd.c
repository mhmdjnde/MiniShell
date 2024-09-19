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

void	get_pwd(char *str)
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
			printf("invalid flag\n");
			free(temp);
			free_args(&args);
			exit_status = 2;
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
	exit_status = 0;
}

void	tildacase(char **env, char **old)
{
	if (chdir(env_search("HOME", env)) == 0)
		*old = getcwd(NULL, 0);
	else
	{
		exit_status = 1;
		printf("cd: %s : No such file or directory\n",
			env_search("HOME", env));
	}
}

void	minuscase(char **old)
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
			exit_status = 1;
			printf("cd: %s : No such file or directory\n", *old);
			free(current_dir);
		}
	}
}

void	no_file(char *str, char *current_dir)
{
	exit_status = 1;
	printf("cd: %s : No such file or directoryy\n", str);
	free(current_dir);
}

void	do_cd(char **env, char *str, char **old)
{
	char	*current_dir;

	current_dir = getcwd(NULL, 0);
	if (ft_strlen(str) == 1 && str[0] == '~')
	{
		tildacase(env, old);
		free(current_dir);
	}
	else if (ft_strlen(str) == 1 && str[0] == '-')
	{
		minuscase(old);
		free(current_dir);
	}
	else
	{
		if (chdir(str) == 0)
		{
			free(*old);
			*old = current_dir;
			exit_status = 0;
		}
		else
		{
			no_file(str, current_dir);
		}
	}
}
