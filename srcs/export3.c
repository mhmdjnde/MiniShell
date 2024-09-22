/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 17:33:35 by fdahouk           #+#    #+#             */
/*   Updated: 2024/09/21 01:27:14 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	shlvl_func(int *lvl, char **str, char **new_lvl_str, char ***args)
{
	if (!(*args)[1])
		(*lvl) = 1;
	else
	{
		(*lvl) = ft_atoi((*args)[1]);
		(*lvl)++;
		free((*args)[1]);
		(*args)[1] = NULL;
	}
	*new_lvl_str = ft_itoa((*lvl));
	if (!(*new_lvl_str))
	{
		free((*args)[0]);
		free((*args));
		return (0);
	}
	*str = malloc(7 + ft_strlen(*new_lvl_str) + 1);
	if (!(*str))
	{
		free(*new_lvl_str);
		free((*args)[0]);
		free((*args));
		return (0);
	}
	return (1);
}

void	inc_shlvl(char ***ex, char ***en)
{
	int			lvl;
	char		*new_lvl_str;
	char		*str;
	t_maintools	t_tools;

	t_tools.strs = malloc(3 * sizeof(char *));
	if (!t_tools.strs)
		return ;
	t_tools.strs[0] = malloc(6);
	ft_strcpy(t_tools.strs[0], "SHLVL");
	t_tools.strs[1] = ft_strdup("$SHLVL");
	t_tools.strs[2] = NULL;
	var_in_env(&t_tools.strs[1], *en, &lvl);
	if (shlvl_func(&lvl, &str, &new_lvl_str, &t_tools.strs) == 0)
		return ;
	ft_strcpy(str, "SHLVL=");
	strncat(str, new_lvl_str, ft_strlen(new_lvl_str));
	free(new_lvl_str);
	t_tools.strs[1] = str;
	add_exp(&t_tools, ex, en, 1);
	free_args(&t_tools.strs);
}
