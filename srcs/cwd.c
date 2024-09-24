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

void	get_pwd(char *str, int *exit_status, char **ex)
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
	cwd = getcwd(NULL, 0);
	if (cwd != NULL)
		print_free_cwd(cwd);
	else
	{
		temp = ft_strdup("$PWD");
		var_in_env(&temp, ex, exit_status);
		if (!empty(temp))
			printf("%s\n", temp);
	}
	*exit_status = 0;
}

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
			return (NULL);
	}
}

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

char	*get_oldpwd(char **env)
{
	int		exit_status;
	char	*en_old;

	en_old = ft_strdup("$OLDPWD");
	var_in_env(&en_old, env, &exit_status);
	if (!empty(en_old))
		return (en_old);
	else
		return (NULL);
}

void	minuscase(char **old, int *exit_status, char **env)
{
	char	*current_dir;
	char	*en_old;

	en_old = get_oldpwd(env);
	if (en_old == NULL)
	{
		ft_putstr_fd("OLDPWD not set\n", 2);
		return ;
	}
	else
	{
		free(*old);
		*old = en_old;
	}
	if (*old != NULL)
	{
		current_dir = get_pwd2(env, exit_status);
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
	char	*t;

	current_dir = get_pwd2(env, exit_status);
	if (ft_strlen(str) == 1 && str[0] == '~')
	{
		tildacase(env, old, exit_status);
		free(current_dir);
	}
	else if (ft_strlen(str) == 1 && str[0] == '-')
	{
		minuscase(old, exit_status, env);
		free(current_dir);
	}
	else
	{
		t = without_quotes_ret(str, 0);
		if (chdir(t) == 0)
		{
			free(*old);
			*old = current_dir;
			*exit_status = 0;
		}
		else
			no_file(t, current_dir, exit_status);
		free(t);
	}
}

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
