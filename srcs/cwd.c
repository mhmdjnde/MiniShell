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
	cwd = getcwd(NULL, 0);
	if (cwd != NULL)
		print_free_cwd(cwd);
	else
	{
		temp = ft_strdup("$PWD");
		var_in_env(&temp, en, exit_status);
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
		{
			free(cur);
			return (NULL);
		}
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

void	minuscase(char **old, int *exit_status, char **env, t_maintools *tools)
{
	char	*current_dir;
	char	*en_old;
	static int	f;

	en_old = get_oldpwd(env);
	if (en_old == NULL && f == 0)
	{
		ft_putstr_fd("OLDPWD not set\n", 2);
		free(*old);
		*old = NULL;
		f = 1;
		tools->exit_status = 1;
		return ;
	}
	else if (en_old != NULL)
	{
		free(*old);
		*old = en_old;
		f = 0;
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
		tools->exit_status = 0;
	}
	else
	{
		ft_putstr_fd("OLDPWD not settt\n", 2);
		tools->exit_status = 1;
	}
}

void	no_file(char *str, int *exit_status)
{
	*exit_status = 1;
	write(2, "cd: ", 4);
	ft_putstr_fd(str, 2);
	write(2, "No such file or directory\n", 26);
	// free(current_dir);
}

char *remove_last_dirs(char *path) {
    int len = strlen(path);
    int i = len - 1, up_dirs = 0;

    // Count how many ".." are at the end
    while (i > 1) {
        if (path[i] == '.' && path[i-1] == '.' && path[i-2] == '/') {
            up_dirs++;
            i -= 3;  // Move past "/.."
        } else if (path[i] == '/') {
            i--;
        } else {
            break;
        }
    }

    if (up_dirs == 0) {
        return strdup(path); // No ".." at the end, return a copy of the original path
    }

    // Allocate a new string for the result
    char *new_path = strdup(path);
    if (!new_path) return NULL; // Check for allocation failure

    // Traverse backward to remove directories
    i = strlen(new_path) - 1;
	up_dirs = up_dirs * 2;
    while (up_dirs > 0 && i >= 0) {
        // Skip trailing slashes
        if (new_path[i] == '/') {
            i--;
            continue;
        }

        // Find the next directory separator from the end
        while (i >= 0 && new_path[i] != '/') {
            i--;
        }

        up_dirs--;
    }

    // Remove trailing slashes and truncate the string
    while (i >= 0 && new_path[i] == '/') {
        i--;
    }

    // Truncate the string at the new position
    new_path[i + 1] = '\0';

    return new_path;
}

char *rm_last_dir(char *path)
{
    char *last_slash;
    char *new_path;
    char *concat_path;

    new_path = remove_last_dirs(path);
    if (!new_path)
        return NULL;
    last_slash = strrchr(new_path, '/');
    if (last_slash)
        *last_slash = '\0';
    if (access(new_path, F_OK) == 0 || ft_strlen(new_path) == 0) {
        return (free(new_path), ft_strdup(path));
    } else {
        free(new_path);
		printf("cd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory\n");
        concat_path = malloc(strlen(path) + 4);
        if (!concat_path)
            return NULL;
        strcpy(concat_path, path);
        strcat(concat_path, "/..");
        return concat_path;
    }
}

void	add_pwd_2(char *pwd, t_maintools *tools)
{
	t_maintools t_tools;

	if (env_search("PWD", tools->en) != NULL)
	{
		t_tools.strs = malloc(3 * sizeof (char *));
		if (!t_tools.strs)
		exit(1);
		t_tools.strs[0] = ft_strdup("jnde");
		t_tools.strs[1] = malloc(5 + ft_strlen(pwd) + 1);
		ft_strcpy(t_tools.strs[1], "PWD=");
		strncat(t_tools.strs[1], pwd, ft_strlen(pwd));
		t_tools.strs[2] = NULL;
		add_exp(&t_tools, &tools->ex, &tools->en, 0);
		free_args(&t_tools.strs);
	}
}

void	do_cd(char **env, char *str, char **old, t_maintools *tools)
{
	char	*current_dir;
	char	*t;
	char	*t2;

	current_dir = get_pwd2(env, &tools->exit_status);
	if (ft_strlen(str) == 1 && str[0] == '~')
	{
		tildacase(env, old, &tools->exit_status);
		free(current_dir);
	}
	else if (ft_strlen(str) == 1 && str[0] == '-')
	{
		minuscase(old, &tools->exit_status, env, tools);
		free(current_dir);
	}
	else
	{
		t = without_quotes_ret(str, 0);
		if (chdir(t) == 0)
		{
			if (ft_strcmp(t, "..") == 0)
			{
				t2 = rm_last_dir(current_dir);
				add_pwd_2(t2, tools);
				free(t2);
			}
			free(*old);
			*old = ft_strdup(current_dir);
			tools->exit_status = 0;
		}
		else
			no_file(t, &tools->exit_status);
		free(current_dir);
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
