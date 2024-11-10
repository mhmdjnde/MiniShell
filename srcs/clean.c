/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 03:53:04 by marvin            #+#    #+#             */
/*   Updated: 2024/11/10 03:53:04 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tools(t_maintools *tools)
{
	if (tools->tmp.fd)
		free(tools->tmp.fd);
	if (tools->str)
		free(tools->str);
	if (tools->strs)
		free_args(&tools->strs);
	if (tools->en)
		free_args(&tools->en);
	if (tools->ex)
		free_args(&tools->ex);
	if (tools->cd)
		free(tools->cd);
}

void	clean(t_maintools *tools)
{
	if (tools->pf != 1)
		del_temp(tools->tmp.tmp, tools->en);
	if (tools->tmp.tmp && tools->pf != 1)
		free(tools->tmp.tmp);
	fix_after_red(tools->in, tools->out, tools->tmp.fd);
	edit_oldpwd(&tools->ex, &tools->en, tools);
	edit_pwd(&tools->ex, &tools->en, tools);
	if (tools->str)
		free(tools->str);
	if (tools->tmp.fd)
		free(tools->tmp.fd);
}
