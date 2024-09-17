/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils5.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 00:02:12 by marvin            #+#    #+#             */
/*   Updated: 2024/09/11 00:02:12 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*add_ch(char *str, char c)
{
	int		len;
	char	*new_str;

	len = 0;
	if (str != NULL)
		len = ft_strlen(str);
	new_str = malloc(len + 2);
	if (new_str == NULL)
		return (NULL);
	if (str != NULL)
		ft_strcpy(new_str, str);
	new_str[len] = c;
	new_str[len + 1] = '\0';
	free(str);
	return (new_str);
}

int	args_len(char **strs)
{
	int	i;

	i = 0;
	while (strs[i] != NULL)
		i++;
	return (i);
}
