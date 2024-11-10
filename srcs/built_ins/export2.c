/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 17:28:20 by fdahouk           #+#    #+#             */
/*   Updated: 2024/11/10 03:06:05 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
void	add_pwd(char ***ex, char ***en)
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
