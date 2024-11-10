/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils6.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 02:53:50 by marvin            #+#    #+#             */
/*   Updated: 2024/11/10 02:53:50 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sign_check(char **args, int *i, int *sf)
{
	if ((args[1][*i] == '+' || args[1][*i] == '-')
		&& *sf == 0 && args[1][*i + 1] != '\0')
	{
		*sf = 1;
		(*i)++;
	}
}

//checks if a variable if availble
void	check_availble(int idx, char ***en, char **args, int i)
{
	if (idx == -1)
		*en = export_enc(*en, args[i]);
	else
	{
		free((*en)[idx]);
		(*en)[idx] = ft_strdup(args[i]);
	}
}

int	is_num(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i] != '\0')
	{
		if (!(str[i] >= '0' && str[i] <= '9'))
			return (0);
		i++;
	}
	return (1);
}

void	ft_strncpy(char *str, char *src, int n)
{
	int	i;

	i = 0;
	while (src[i] != '\0' && i < n)
	{
		str[i] = src[i];
		i++;
	}
	str[i] = '\0';
}

char	*ft_strdup(char *src)
{
	char	*str2;
	int		slen;
	int		i;

	slen = ft_strlen(src);
	i = 0;
	str2 = (char *)malloc((slen + 1) * sizeof(char));
	if (str2 == NULL)
	{
		return (NULL);
	}
	while (src[i] != '\0')
	{
		str2[i] = src[i];
		i++;
	}
	str2[i] = '\0';
	return (str2);
}
