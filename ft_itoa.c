/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjoundi <mjoundi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 13:47:50 by mjoundi           #+#    #+#             */
/*   Updated: 2024/08/31 13:49:26 by mjoundi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	digcount(int n)
{
	int	count;

	count = 0;
	while (n != 0)
	{
		count++;
		n = n / 10;
	}
	return (count);
}

static char	*strrev(char *str)
{
	int		i;
	int		j;
	char	*s;
	char	c;

	i = 0;
	c = ' ';
	j = ft_strlen(str) - 1;
	s = str;
	if (s[0] == '-')
		i = 1;
	while (j > i)
	{
		c = s[i];
		s[i] = s[j];
		s[j] = c;
		i++;
		j--;
	}
	return (s);
}

static void	helper(int n, int *i, char *num)
{
	if (n < 0)
	{
		n = -n;
		num[(*i)++] = '-';
	}
	while (n != 0)
	{
		num[(*i)++] = n % 10 + '0';
		n = n / 10;
	}
	num[*i] = '\0';
}

char	*ft_itoa(int n)
{
	char	*num;
	int		i;

	if (n == -2147483648)
	{
		num = (char *)malloc(12 * sizeof(char));
		ft_strncpy(num, "-2147483648", 11);
		return (num);
	}
	if (n <= 0)
		num = (char *)malloc((digcount(n) + 2) * sizeof(char));
	else
		num = (char *)malloc((digcount(n) + 1) * sizeof(char));
	if (num == NULL)
		return (NULL);
	i = 0;
	if (n == 0)
	{
		num[0] = '0';
		num[1] = '\0';
		return (num);
	}
	helper(n, &i, num);
	return (strrev(num));
}
