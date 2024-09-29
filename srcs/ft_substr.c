/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjoundi <mjoundi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 13:50:05 by mjoundi           #+#    #+#             */
/*   Updated: 2024/09/27 18:04:20 by mjoundi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*allocation(char *substr)
{
	char	*str;

	str = substr;
	str = (char *)malloc(1);
	if (!str)
		return (NULL);
	str[0] = '\0';
	return (str);
}

char	*ft_substr(char *s, int start, int len)
{
	char	*substr;
	int		i;
	int		j;
	int		lenn;

	i = start;
	j = 0;
	substr = "";
	lenn = (unsigned int)len;
	if (start >= ft_strlen(s))
		return (allocation(substr));
	if (ft_strlen(s) - start < lenn)
		substr = (char *)malloc((ft_strlen(s) - start + 1) * sizeof(char));
	else
		substr = (char *)malloc((lenn + 1) * sizeof(char));
	if (!substr)
		return (NULL);
	while (j < lenn && s[i] != '\0')
	{
		substr[j] = s[i];
		i++;
		j++;
	}
	substr[j] = '\0';
	return (substr);
}
