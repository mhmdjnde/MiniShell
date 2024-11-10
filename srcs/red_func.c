/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   red_func.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 04:26:29 by marvin            #+#    #+#             */
/*   Updated: 2024/11/10 04:26:29 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	file_nl(t_redmain *r, char **str)
{
	if (r->rf == 0)
	{
		r->red[r->tab[2]].file = ft_substr((*str),
				r->tab[1], r->tab[0] - r->tab[1]);
		r->tab[1]--;
		while ((*str)[r->tab[1]] == ' ' && r->tab[1] >= 0)
			r->tab[1]--;
		if (r->check == 3 || r->check == 4)
			r->tab[1]--;
		(*str) = del_part((*str), r->tab[1], r->tab[0] - r->tab[1]);
		r->red[r->tab[2]].o = r->check;
		r->tab[0] = r->tab[1] + 1;
		r->tab[2]++;
		if (more_red_check((*str) + r->tab[0]) == 0)
		{
			r->red[r->tab[2]].file = NULL;
			return (0);
		}
		r->rf = 1;
		while ((*str)[r->tab[0]] == ' ')
			r->tab[0]++;
	}
	return (1);
}

int	file_l(t_redmain *r, char **str)
{
	if ((*str)[r->tab[0]] == '\0')
	{
		r->rf = 0;
		r->red[r->tab[2]].file = ft_substr((*str),
				r->tab[1], r->tab[0] - r->tab[1]);
		r->tab[1]--;
		while ((*str)[r->tab[1]] == ' ' && r->tab[1] >= 0)
			r->tab[1]--;
		if (r->check == 3 || r->check == 4)
			r->tab[1]--;
		(*str) = del_part((*str), r->tab[1], r->tab[0] - r->tab[1]);
		r->red[r->tab[2]].o = r->check;
		r->tab[0] = r->tab[1] + 1;
		r->tab[2]++;
		return (0);
	}
	return (1);
}

int	red_loop(t_redmain *r, char **str, int *exit_status)
{
	r->rf = 1;
	r->check = check_red((*str) + r->tab[0]);
	if (r->check <= 0)
		return (edit_red_ext(exit_status));
	if (r->check == 1 || r->check == 2)
		r->tab[0]++;
	else
		r->tab[0] += 2;
	while ((*str)[r->tab[0]] != '\0' && (*str)[r->tab[0]] == ' ')
		r->tab[0]++;
	r->tab[1] = r->tab[0];
	while ((*str)[r->tab[0]] != '\0' && (*str)[r->tab[0]] != '>'
		&& (*str)[r->tab[0]] != '<' && (*str)[r->tab[0]] != '\''
		&& (*str)[r->tab[0]] != '"' && (*str)[r->tab[0]] != ' ')
		r->tab[0]++;
	if ((*str)[r->tab[0]] == '>' || (*str)[r->tab[0]] == '<'
		|| (*str)[r->tab[0]] == ' ')
	{
		r->rf = 0;
		r->tab[0]--;
		return (1);
	}
	return (2);
}

void	red_q(t_redmain *r, char **str)
{
	r->q = (*str)[r->tab[0]];
	r->tab[0]++;
	while ((*str)[r->tab[0]] != '\0' && (*str)[r->tab[0]] != r->q)
		r->tab[0]++;
	if ((*str)[r->tab[0]] == r->q)
		r->tab[0]++;
	if (r->rf == 1 && ((*str)[r->tab[0]] == '\0' || (*str)[r->tab[0]] == ' '
		|| (*str)[r->tab[0]] == '>' || (*str)[r->tab[0]] == '<'))
		r->rf = 0;
}

int	red_main_loop(t_redmain *r, char **str, int *exit_status)
{
	int	t;

	if ((*str)[r->tab[0]] == '"' || (*str)[r->tab[0]] == '\'')
		red_q(r, str);
	else if ((*str)[r->tab[0]] == '>' || (*str)[r->tab[0]] == '<')
	{
		t = red_loop(r, str, exit_status);
		if (t == 0)
			return (0);
		else if (t == 1)
			return (1);
		if (file_l(r, str) == 0)
			return (2);
		if ((*str)[r->tab[0]] == '\'' || (*str)[r->tab[0]] == '"')
			return (1);
	}
	else
	{
		if ((*str)[r->tab[0]] != '\0')
			r->tab[0]++;
		if ((*str)[r->tab[0]] == '\0' && r->rf == 1)
			r->rf = 0;
	}
	return (-1);
}
