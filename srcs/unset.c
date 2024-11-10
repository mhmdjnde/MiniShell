/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 03:12:30 by marvin            #+#    #+#             */
/*   Updated: 2024/08/28 03:12:30 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	complete_if(char *t, char *t2, char ***en, char ***ex)
{
	if (ret_s_indexx(t2, *en) != -1)
		*en = remove_at_s(*en, ret_s_indexx(t2, *en));
	if (ret_s_index2(t, *ex) != -1)
		*ex = remove_at_s(*ex, ret_s_index2(t, *ex));
}

void	rm_exp(char **args, char ***ex, char ***en, int *exit_status)
{
	int		i;
	char	*t;
	char	*t2;
	char	*t3;

	i = 1;
	if (args_len(args) >= 2 && flag_check(args[1]) == -1
		&& printing_with_return(exit_status) == 1)
		return ;
	while (args[i] != NULL)
	{
		t3 = without_quotes_ret(args[i], 0);
		t = ret_to_equal2(t3);
		t2 = add_equal(t);
		if (check_unset(args[i]) != -1)
		{
			inc_and_free(&i, t3, t2, t);
			continue ;
		}
		complete_if(t, t2, en, ex);
		i++;
		free_three(t, t2, t3);
	}
	*exit_status = 0;
}
