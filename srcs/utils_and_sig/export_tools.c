/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_tools.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 03:06:14 by marvin            #+#    #+#             */
/*   Updated: 2024/11/10 03:06:14 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
