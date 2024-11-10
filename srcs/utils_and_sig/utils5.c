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

char	*ft_strcat(char *dest, char *src)
{
	char	*odest;
	int		i;
	int		a;

	odest = dest;
	i = 0;
	a = 0;
	while (dest[i] != '\0')
	{
		i++;
	}
	while (src[a] != '\0')
	{
		dest[i] = src[a];
		i++;
		a++;
	}
	dest[i] = '\0';
	return (odest);
}

char	*ft_strrchr(char *s, int c)
{
	int		i;
	int		t;
	char	*str;

	i = ft_strlen(s) - 1;
	t = ft_strlen(s);
	str = (char *) s;
	if (c == '\0')
		return (&str[t]);
	while (i >= 0)
	{
		if (str[i] == (char) c)
			return (&str[i]);
		i--;
	}
	return (NULL);
}

char	*ft_strncat(char *dest, char *src, unsigned int nb)
{
	char			*odest;
	int				i;
	unsigned int	a;

	odest = dest;
	i = 0;
	a = 0;
	while (dest[i] != '\0')
	{
		i++;
	}
	while (src[a] != '\0' && a < nb)
	{
		dest[i] = src[a];
		i++;
		a++;
	}
	dest[i] = '\0';
	return (odest);
}
