/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_red.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 04:34:29 by marvin            #+#    #+#             */
/*   Updated: 2024/11/10 04:34:29 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	*init_rtab(void)
{
	int	*rtab;

	rtab = malloc(3 * sizeof(int));
	rtab[0] = 0;
	rtab[2] = 0;
	return (rtab);
}

int	edit_red_ext(int *exit_status)
{
	*exit_status = 2;
	return (0);
}

void	print_cr(char cr)
{
	write(2, "minishell: syntax error near unexpected token `", 47);
	write(2, &cr, 1);
	write(2, "'\n", 2);
}

int	*init_int_tab(void)
{
	int	*tab;

	tab = malloc(2 * sizeof(int));
	tab[0] = -1;
	tab[1] = -1;
	return (tab);
}

t_redtools	*red_after_cmd(char **str, int *exit_status)
{
	t_redmain	r;
	int			t;

	r.tab = init_rtab();
	r.rf = -1;
	r.red = malloc((red_count(*str) + 1) * sizeof(t_redtools));
	while ((*str)[r.tab[0]] != '\0' && (*str)[r.tab[0]] == ' ')
		r.tab[0]++;
	while ((*str)[r.tab[0]] != '\0')
	{
		t = red_main_loop(&r, str, exit_status);
		if (t == 0)
		{
			r.red[r.tab[2]].file = NULL;
			return (free(r.tab), free_red(r.red), NULL);
		}
		else if (t == 1)
			continue ;
		else if (t == 2)
			break ;
		if (file_nl(&r, str) == 0)
			return (free(r.tab), r.red);
	}
	r.red[r.tab[2]].file = NULL;
	return (free(r.tab), r.red);
}
