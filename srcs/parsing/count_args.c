/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjoundi <mjoundi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 15:28:20 by mjoundi           #+#    #+#             */
/*   Updated: 2024/08/20 15:28:20 by mjoundi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//counting in case of quotes
int	process_quotes(char *str, int i, char quote)
{
	i++;
	while (str[i] != quote && str[i] != '\0')
		i++;
	if (str[i] != '\0')
		i++;
	return (i);
}

//counting when there is no quotes
int	process_non_quotes(char *str, int i, int *ac)
{
	while (str[i] != ' ' && str[i] != '\'' && str[i] != '"' && str[i] != '\0')
		i++;
	if (str[i] == ' ')
	{
		(*ac)++;
		while (str[i] == ' ' && str[i] != '\0')
			i++;
	}
	return (i);
}

//the counting proccess of the arguments in case of quotes and not
void	ac_helper(char *str, int *i, int *ac)
{
	if (str[*i] == '"')
		*i = process_quotes(str, *i, '"');
	else if (str[*i] == '\'')
		*i = process_quotes(str, *i, '\'');
	else
		*i = process_non_quotes(str, *i, ac);
}

//this function return  the number of arguments of the command
int	count_args(char *str, char *cmd)
{
	int	i;
	int	ac;
	int	f;

	ac = 0;
	f = 0;
	i = parse_cmd(str, cmd);
	if (i < 0 || i >= ft_strlen(str))
	{
		i = 0;
		f = 1;
	}
	while (str[i] != '\0' && str[i] == ' ')
		i++;
	while (str[i] != '\0')
	{
		ac_helper(str, &i, &ac);
		if (i > 0 && str[i] == '\0' && str[i - 1] != ' ')
			ac++;
		else if (i == 0 && str[i] == '\0')
			break ;
	}
	if (ac != 0 && f == 1)
		return (ac - 1);
	return (ac);
}
