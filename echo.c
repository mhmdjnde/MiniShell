/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 18:27:32 by mjoundi           #+#    #+#             */
/*   Updated: 2024/09/14 04:39:23 by marvin           ###   ########.fr       */
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

void	echo_args_check(char **strs)
{
	int	i;
	int	nf;

	i = 1;
	nf = 0;
	while (strs[i] != NULL && parse_flags(strs[i], "-n") >= 0)
	{
		if (n_check(strs[i]) == 0)
			break ;
		i++;
		nf = 1;
	}
	while (strs[i] != NULL)
	{
		without_quotes(strs[i], 0);
		i++;
		if (strs[i] != NULL)
			write(1, " ", 1);
	}
	if (nf == 0)
		printf("\n");
}
