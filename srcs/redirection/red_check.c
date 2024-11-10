/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   red_check.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 04:23:13 by marvin            #+#    #+#             */
/*   Updated: 2024/11/10 04:23:13 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	redname_check(char *str, int i)
{
	int	j;

	j = i;
	while (str[j] != '\0' && str[j] == ' ')
		j++;
	if (str[j] == '>' || str[j] == '<')
		return (0);
	else
		return (1);
}

int	check_red1(char *str, int i, char cr, char c)
{
	if (redname_check(str, i) == 0)
	{
		print_cr(cr);
		return (-4);
	}
	if ((cr == '>' && str[i] == '<') || (cr == '<' && str[i] == '>'))
	{
		print_cr(cr);
		return (-1);
	}
	if (c == 1 && str[i] != '\0')
	{
		if (cr == '>')
			return (1);
		if (cr == '<')
			return (2);
	}
	if (c == 2 && str[i] != '\0')
	{
		if (cr == '>')
			return (3);
		if (cr == '<')
			return (4);
	}
	return (10);
}

int	check_red2(char *str, int i, char cr, char c)
{
	if (c == 3)
	{
		write(2, "minishell: syntax error near unexpected token `", 47);
		write(2, &cr, 1);
		write(2, "'\n", 2);
		return (-2);
	}
	if (c > 3)
	{
		write(2, "minishell: syntax error near unexpected token `", 47);
		write(2, &cr, 1);
		write(2, &cr, 1);
		write(2, "'\n", 2);
		return (-3);
	}
	if (str[i] == '\0')
	{
		write(2, "syntax error near 'newline'\n", 28);
		return (0);
	}
	return (10);
}

int	check_red(char *str)
{
	int		c;
	int		i;
	char	cr;
	int		t;

	i = 0;
	c = 0;
	cr = str[i];
	while (str[i] != '\0' && str[i] == cr)
	{
		i++;
		c++;
	}
	t = check_red1(str, i, cr, c);
	if (t != 10)
		return (t);
	t = check_red2(str, i, cr, c);
	if (t != 10)
		return (t);
	return (0);
}

int	red_count(char *str)
{
	int	c;
	int	i;

	i = 0;
	c = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '>' || str[i] == '<')
			c++;
		i++;
	}
	return (c);
}
