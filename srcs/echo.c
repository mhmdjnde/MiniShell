/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 18:27:32 by mjoundi           #+#    #+#             */
/*   Updated: 2024/09/29 16:21:08 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	n_check(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '-' && str[i] != '\0')
		i++;
	if (str[i] == '-')
		i++;
	while (str[i] != '\0' && (str[i] == 'n' || str[i] == '\'' || str[i] == '"'))
		i++;
	if (str[i] == '\0')
		return (1);
	return (0);
}

void	skip_n(int *i, char **strs, int *nf)
{
	while (strs[*i] != NULL && parse_flags(strs[*i], "-n") >= 0)
	{
		if (n_check(strs[*i]) == 0)
			break ;
		(*i)++;
		*nf = 1;
	}
}

void	echo_args_check(char **strs, t_maintools *tools)
{
	int		i;
	int		nf;
	char	*t;

	i = 1;
	nf = 0;
	skip_n(&i, strs, &nf);
	while (strs[i] != NULL)
	{
		t = without_quotes_ret(strs[i], 0);
		t = rm_dl(t);
		var_in_env(&t, tools->en, &tools->exit_status);
		t = rm_bs(t);
		printf("%s", t);
		i++;
		if (strs[i] != NULL && !empty(t))
			write(1, " ", 1);
		free(t);
	}
	if (nf == 0)
		printf("\n");
}
