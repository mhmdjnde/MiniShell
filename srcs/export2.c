/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 17:28:20 by fdahouk           #+#    #+#             */
/*   Updated: 2024/11/05 03:13:23 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//return the index of the variable in the env
int	ret_s_index(char *str, char **env)
{
	int	i;

	i = 0;
	while (env[i] != NULL)
	{
		if (ft_strncmp(env[i], str, ft_strlen(str)) == 0
			&& ft_strlen(env[i]) >= ft_strlen(str)
			&& (env[i][ft_strlen(str)] == '='
			|| env[i][ft_strlen(str)] == '\0'))
			return (i);
		i++;
	}
	return (-1);
}

//check the args of the export command
int	exp_arg_check(char *str)
{
	int		i;
	char	*temp;

	temp = without_quotes_ret(str, 0);
	if (!(temp[0] >= 'a' && temp[0] <= 'z')
		&& !(temp[0] >= 'A' && temp[0] <= 'Z') && temp[0] != '_')
	{
		free(temp);
		return (0);
	}
	i = 1;
	while (temp[i] != '=' && temp[i] != '\0')
	{
		if (!is_ok(temp[i]))
		{
			free(temp);
			return (0);
		}
		i++;
	}
	free(temp);
	return (1);
}

//return the variable name (to the '=')
char	*ret_to_equal(char *str)
{
	int		i;
	char	*r;

	i = 0;
	while (str[i] != '=' && str[i] != '\0')
		i++;
	r = (char *)malloc(sizeof(char) * (i + 1));
	if (!r)
		return (NULL);
	ft_strncpy(r, str, i);
	r[i] = '\0';
	return (r);
}

//checks if a variable if availble
void	check_availble(int idx, char ***en, char **args, int i)
{
	if (idx == -1)
		*en = export_enc(*en, args[i]);
	else
	{
		free((*en)[idx]);
		(*en)[idx] = ft_strdup(args[i]);
	}
}

//add to env functionality
void	func_add_exp(int i, char ***en, t_maintools *tools, char ***ex)
{
	char	*prefix;
	int		idx;

	if (check_equal(tools->strs[i]) == 0)
	{
		write(2, "Bash: export: ", 14);
		ft_putstr_fd(tools->strs[i], 2);
		write(2, " not valid idenfier\n", 20);
		tools->exit_status = 1;
	}
	else
	{
		prefix = ret_to_equal(tools->strs[i]);
		idx = ret_s_index(prefix, *en);
		check_availble(idx, en, tools->strs, i);
		free(prefix);
		tools->strs[i] = add_quotes(tools->strs[i]);
		prefix = ret_to_equal(tools->strs[i]);
		idx = ret_s_index(prefix, *ex);
		check_availble(idx, ex, tools->strs, i);
		free(prefix);
	}
}

int	p_exp_err(t_maintools *tools, int i, char *error, int ext)
{
	write(2, error, ft_strlen(error));
	ft_putstr_fd((tools->strs)[i], 2);
	tools->exit_status = ext;
	return (1);
}

void	check_if_minus(t_maintools *tools, int tab[2])
{
	if ((tools->strs)[tab[0]][0] == '-')
	{
		tab[1] = p_exp_err(tools, tab[0], "invalid option\n", 1);
		tools->exit_status = 2;
	}
	else
	{
		write(2, "Bash: export: ", 14);
		ft_putstr_fd((tools->strs)[tab[0]], 2);
		write(2, " not valid idenfier\n", 20);
		tools->exit_status = 1;
		tab[1] = 1;
	}
}

void	edited_exit_status(t_maintools *tools, int f, int sf)
{
	if (f == 0 && sf == 0)
		tools->exit_status = 0;
}

//add a variable to the environment
void	add_exp(t_maintools *tools, char ***ex, char ***en, int sf)
{
	int		tab[2];
	char	*temp;

	tab[0] = 1;
	tab[1] = 0;
	while ((tools->strs)[tab[0]] != NULL)
	{
		temp = without_quotes_ret((tools->strs)[tab[0]], 0);
		free((tools->strs)[tab[0]]);
		(tools->strs)[tab[0]] = temp;
		if (!exp_arg_check((tools->strs)[tab[0]]))
			check_if_minus(tools, tab);
		else
		{
			if (check_equal((tools->strs)[tab[0]]) == -1)
			{
				if (ret_s_index((tools->strs)[tab[0]], *ex) == -1)
					*ex = export_enc(*ex, (tools->strs)[tab[0]]);
			}
			else
				func_add_exp(tab[0], en, tools, ex);
		}
		tab[0]++;
	}
	edited_exit_status(tools, tab[1], sf);
}

void	deleted_pwd(t_maintools *tools, t_maintools *t_tools, int f)
{
	int	t;

	t = 0;
	if (f != 1 && tools->cdf == 1)
	{
		t_tools->strs = malloc(3 * sizeof(char *));
		t_tools->strs[0] = ft_strdup("jnde");
		t_tools->strs[1] = ft_strdup("OLDPWD");
		t_tools->strs[2] = NULL;
		rm_exp(t_tools->strs, &tools->ex, &tools->en, &t);
		add_exp(t_tools, &tools->ex, &tools->en, 1);
	}
}

//edit the current PWD
void	edit_pwd(char ***ex, char ***en, t_maintools *tools)
{
	char		*pwd;
	t_maintools	t_tools;
	static int	f;

	if (ret_s_index("PWD", *ex) != -1)
	{
		pwd = getcwd(NULL, 0);
		if (pwd == NULL)
			return ;
		t_tools.strs = malloc(3 * sizeof(char *));
		t_tools.strs[0] = malloc(5);
		ft_strcpy(t_tools.strs[0], "jnde");
		t_tools.strs[1] = malloc(5 + ft_strlen(pwd) + 1);
		ft_strcpy(t_tools.strs[1], "PWD=");
		ft_strncat(t_tools.strs[1], pwd, ft_strlen(pwd));
		t_tools.strs[2] = NULL;
		add_exp(&t_tools, ex, en, 1);
		free(pwd);
		free_args(&t_tools.strs);
		f = 0;
	}
	else
		deleted_pwd(tools, &t_tools, f);
}

//add the PWD to the env
void	   add_pwd(char ***ex, char ***en)
{
	char		*pwd;
	t_maintools	t_tools;

	pwd = getcwd(NULL, 0);
	if (pwd == NULL)
		return ;
	t_tools.strs = malloc(3 * sizeof(char *));
	t_tools.strs[0] = malloc(5);
	ft_strcpy(t_tools.strs[0], "jnde");
	t_tools.strs[1] = malloc(5 + ft_strlen(pwd) + 1);
	ft_strcpy(t_tools.strs[1], "PWD=");
	ft_strncat(t_tools.strs[1], pwd, ft_strlen(pwd));
	t_tools.strs[2] = NULL;
	add_exp(&t_tools, ex, en, 1);
	free(pwd);
	free_args(&t_tools.strs);
}

//edit the OLDPWD after changing dir
void	edit_oldpwd(char ***ex, char ***en, t_maintools *tools)
{
	char		*oldpwd;
	t_maintools	t_tools;

	if (env_search("OLDPWD", *en) != NULL)
	{
		oldpwd = tools->cd;
		if (oldpwd == NULL)
			return ;
		t_tools.strs = malloc(3 * sizeof(char *));
		t_tools.strs[0] = malloc(5);
		ft_strcpy(t_tools.strs[0], "jnde");
		t_tools.strs[1] = malloc(8 + ft_strlen(oldpwd) + 1);
		ft_strcpy(t_tools.strs[1], "OLDPWD=");
		ft_strncat(t_tools.strs[1], oldpwd, ft_strlen(oldpwd));
		t_tools.strs[2] = NULL;
		add_exp(&t_tools, ex, en, 1);
		free_args(&t_tools.strs);
	}
}
