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
		return (NULL);
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

//print an error message when no such file or directory
void	no_file(char *str, int *exit_status)
{
	*exit_status = 1;
	write(2, "cd: ", 4);
	ft_putstr_fd(str, 2);
	write(2, "No such file or directory\n", 26);
}

//the part of the no directory case where we remove the last dirs
void	rm_dr_helper(char **new_path, char *path, int *i, int *dd_len)
{
	(*new_path) = ft_strdup(path);
	*i = ft_strlen((*new_path)) - 1;
	*dd_len = *dd_len * 2;
	while (*dd_len > 0 && *i >= 0)
	{
		if ((*new_path)[*i] == '/')
		{
			(*i)--;
			continue ;
		}
		while (*i >= 0 && (*new_path)[*i] != '/')
			(*i)--;
		(*dd_len)--;
	}
	while (*i >= 0 && (*new_path)[*i] == '/')
		(*i)--;
	(*new_path)[*i + 1] = '\0';
}

//pt 2
char	*remove_last_dirs(char *path)
{
	int		len;
	int		i;
	int		dd_len;
	char	*new_path;

	len = ft_strlen(path);
	i = len - 1;
	dd_len = 0;
	while (i > 1)
	{
		if (path[i] == '.' && path[i - 1] == '.' && path[i - 2] == '/')
		{
			dd_len++;
			i -= 3;
		}
		else if (path[i] == '/')
			i--;
		else
			break ;
	}
	if (dd_len == 0)
		return (ft_strdup(path));
	rm_dr_helper(&new_path, path, &i, &dd_len);
	return (new_path);
}

//this function is for the case where we do "cd .." and 
//we are in a deleted directory, then we need to remove the
//last directories the number of times we did "cd .."
//to check if we did reach a true path
//pt 1
char	*rm_last_dir(char *path)
{
	char	*last_slash;
	char	*new_path;
	char	*concat_path;

	new_path = remove_last_dirs(path);
	if (!new_path)
		return (NULL);
	last_slash = ft_strrchr(new_path, '/');
	if (last_slash)
		*last_slash = '\0';
	if (access(new_path, F_OK) == 0 || ft_strlen(new_path) == 0)
		return (free(new_path), ft_strdup(path));
	else
	{
		free(new_path);
		printf("cd: error retrieving current directory: getcwd: ");
		printf("cannot access parent directories: No such file or directory\n");
		concat_path = malloc(ft_strlen(path) + 4);
		if (!concat_path)
			return (NULL);
		ft_strcpy(concat_path, path);
		ft_strcat(concat_path, "/..");
		return (concat_path);
	}
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
