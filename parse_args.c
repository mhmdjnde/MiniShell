/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   parse_args.c									   :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: marvin <marvin@student.42.fr>			  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/08/17 01:50:44 by marvin			#+#	#+#			 */
/*   Updated: 2024/08/17 01:50:44 by marvin		   ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "minishell.h"

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

int	q_bound(char *str, int i, int *in_quotes, char *quote_char)
{
	if (str[i] == '\'' || str[i] == '"')
	{
		if (*in_quotes == 0)
		{
			*quote_char = str[i];
			*in_quotes = 1;
		}
		else if (*in_quotes == 1 && str[i] == *quote_char)
			*in_quotes = 0;
	}
	else if (str[i] == ' ' && *in_quotes == 0)
		return (0);
	return (1);
}

void	find_arg_bounds(char *str, int *index, char **start, int *len)
{
	int		i;
	char	quote_char;
	int		in_quotes;

	*len = 0;
	*start = NULL;
	i = *index;
	while (str[i] == ' ')
		i++;
	if (str[i] == '\0')
	{
		*index = i;
		return ;
	}
	*start = &str[i];
	in_quotes = 0;
	while (str[i] != '\0')
	{
		if (q_bound(str, i, &in_quotes, &quote_char) == 0)
			break ;
		i++;
	}
	*len = &str[i] - *start;
	*index = i;
}

char	*extract_arg(char *str, int *index)
{
	char	*start;
	int		len;
	char	*arg;

	find_arg_bounds(str, index, &start, &len);
	if (len == 0)
		return (NULL);
	arg = (char *)malloc((len + 1) * sizeof(char));
	if (arg == NULL)
		return (NULL);
	ft_strncpy(arg, start, len);
	arg[len] = '\0';
	return (arg);
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

void	add_arg(char ***args, char **arg, int *k)
{
	if (*arg != NULL)
	{
		(*args)[(*k)++] = ft_strdup(*arg);
		free(*arg);
	}
}

char	**parse_args(char *str, char *cmd)
{
	int		i;
	int		ac;
	char	**args;
	int		k;
	char	*arg;

	i = parse_cmd(str, cmd);
	if (i == -1)
		i = get_index(str);
	ac = count_args(str, cmd);
	args = (char **)malloc((ac + 2) * sizeof(char *));
	args[0] = ft_strdup(cmd);
	k = 1;
	while (str[i] != '\0')
	{
		while (str[i] != '\0' && str[i] == ' ')
			i++;
		arg = extract_arg(str, &i);
		add_arg(&args, &arg, &k);
		while (str[i] != '\0' && str[i] == ' ')
			i++;
	}
	args[k] = NULL;
	return (args);
}
